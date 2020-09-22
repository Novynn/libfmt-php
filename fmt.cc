#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_fmt.h"
#include "fmt_arginfo.h"

#include "ext/spl/spl_exceptions.h"
}

#define FMT_HEADER_ONLY
#include "fmt/format.h"

/* If you declare any globals in php_fmt.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(fmt)
*/

/* True global resources - no need for thread safety here */
static int le_fmt;

PHP_MINIT_FUNCTION(fmt)
{
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(fmt)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(fmt)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "fmt support", "enabled");
    php_info_print_table_end();
}

PHP_FUNCTION(fmt)
{
    char* format = nullptr;
    size_t format_length = 0;

    zval *args = nullptr;
    int argc = 0;

    ZEND_PARSE_PARAMETERS_START(1, -1)
        Z_PARAM_STRING(format, format_length)
        Z_PARAM_VARIADIC('*', args, argc)
    ZEND_PARSE_PARAMETERS_END();

    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.reserve(argc, 0);

    for (int i = 0; i < argc; ++i)
    {
        zval* tmp = &args[i];

loop:
        ZVAL_DEREF(tmp);

        switch (Z_TYPE_P(tmp)) {
            case IS_UNDEF:
            case IS_NULL:
                store.push_back("");
            case IS_FALSE:
                store.push_back(false);
            case IS_TRUE:
                store.push_back(true);
            case IS_LONG:
                store.push_back(Z_LVAL_P(tmp));
            break;
            case IS_DOUBLE:
                store.push_back(Z_DVAL_P(tmp));
            break;
            case IS_STRING:
                store.push_back(std::cref(Z_STRVAL_P(tmp)));
            break;
            case IS_ARRAY:
                store.push_back("array");
            break;
            case IS_OBJECT:
                store.push_back("object");
            break;
            case IS_RESOURCE:
                store.push_back("resource");
            break;
            case IS_REFERENCE:
                goto loop;
            break;
            default:
                store.push_back("unknown");
            break;
        }
    }

    try {
        std::string result = fmt::vformat(format, store);
        RETURN_STRING(result.c_str());
    } catch (const fmt::format_error& ex) {
        zend_throw_error(spl_ce_InvalidArgumentException, ex.what());
        return;
    }
}

static const zend_module_dep fmt_deps[] = {
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_END
};

zend_module_entry fmt_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	fmt_deps,
    "fmt",
    ext_functions,
    PHP_MINIT(fmt),
    PHP_MSHUTDOWN(fmt),
    NULL,
    NULL,
    PHP_MINFO(fmt),
    PHP_FMT_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_FMT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(fmt)
#endif
