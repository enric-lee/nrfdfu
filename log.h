#ifndef LOG_H
#define LOG_H

/* these conincide with syslog levels for convenience */
enum loglevel { LL_CRIT = 2, LL_ERR, LL_WARN, LL_NOTICE, LL_INFO, LL_DEBUG };

void __attribute__ ((format (printf, 2, 3)))
log_out(enum loglevel ll, const char *fmt, ...);

#ifndef DEBUG
#define DEBUG 0
#endif

#define LOG_CRIT(...) log_out(LL_CRIT, __VA_ARGS__)
#define LOG_ERR(...) log_out(LL_ERR, __VA_ARGS__)
#define LOG_WARN(...) log_out(LL_WARN, __VA_ARGS__)
#define LOG_NOTI(...) log_out(LL_NOTICE, __VA_ARGS__)
#define LOG_INF(...) log_out(LL_INFO, __VA_ARGS__)
#define LOG_DBG(...) do { if (DEBUG) log_out(LL_DEBUG, __VA_ARGS__); } while (0)
#define LOG_DBGL(lvl, ...) do { if (DEBUG && conf.debug >= lvl) log_out(LL_DEBUG, __VA_ARGS__); } while (0)

#endif