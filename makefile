config := $(shell test -e config.mk && echo config.mk || echo config-default.mk)
include ${config}

sources := $(wildcard src/*.c src/**/*.c)
objects := $(patsubst src/%.c,out/%.o,${sources})
binary  := tiny-rens

.PHONY: clean ${sources}
all: bin/${binary}

out/%.o: src/%.c
	@mkdir -p $(@D)
	${cc} ${cflags} -c -o $@ $<

bin/${binary}: ${objects}
	@mkdir -p $(@D)
	${cc} ${cflags} -o $@ $^ ${ldflags}
	strip ${strip} $@

clean:
	rm -rf bin/ out/

