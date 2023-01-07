cc := cc
cflags := -O3 -lto -DBUILD_RELEASE
ldflags := -lcurl
strip := -s
ifneq ("$(wildcard config.mk)","")
	include config.mk
endif

sources := $(wildcard src/*.c src/**/*.c)
objects := $(patsubst src/%.c,out/%.o,${sources})
binary  := tiny-rens

vername := $(shell git rev-parse --abbrev-ref HEAD)

.PHONY: clean ${sources}
all: bin/${binary}

out/%.o: src/%.c
	@mkdir -p $(@D)
	${cc} ${cflags} -DBUILD_COMMIT="\"${vername}\"" -c -o $@ $<

bin/${binary}: ${objects}
	@mkdir -p $(@D)
	${cc} ${cflags} -o $@ $^ ${ldflags}
	strip ${strip} $@

clean:
	rm -rf bin/ out/

