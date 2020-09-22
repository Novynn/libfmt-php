#ifndef PHP_FMT_H
#define PHP_FMT_H

extern zend_module_entry fmt_module_entry;
#define phpext_fmt_ptr &fmt_module_entry

#define PHP_FMT_VERSION "0.1.0"

#if defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_FMT_API __attribute__ ((visibility("default")))
#else
#	define PHP_FMT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if defined(ZTS) && defined(COMPILE_DL_FMT)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_FMT_H */
