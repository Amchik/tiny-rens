/* vim: ft=c
 * Logging header file
 */

#ifndef SJ6DIPA1m_r4kzqxvQaR99ZCHzVQ3DV_2rymbogQ_jVYm0Z6xZ1aCaFZisS5pfAn_VPrtwtLpR2p
#define SJ6DIPA1m_r4kzqxvQaR99ZCHzVQ3DV_2rymbogQ_jVYm0Z6xZ1aCaFZisS5pfAn_VPrtwtLpR2p

#define assert(expr, msg) \
	if (!(expr)) {\
		volatile int *__assert_void_p = 0;\
		logf(LERR, "%s (assertion `%s` failed)", msg, #expr);\
		*__assert_void_p = 42;\
	}

#ifdef DEBUG
	#define assert_debug(expr, msg) assert(expr, msg)
#else
	#define assert_debug(_1, _2)
#endif

#ifdef NO_DEBUG_LOG
	#define dlogf(...)
#else
	#define dlogf(...) logf(LDEBUG, __VA_ARGS__)
#endif

/* Defined in src/main.c
 */
extern int LOG_LEVEL;

#define LPRIM  0
#define LERR   1
#define LWARN  2
#define LINFO  3
#define LDEBUG 4

void logf(int log_level, const char *fmt, ...);

#endif

