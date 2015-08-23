#ifndef PHP_HTMLQUERY_H
#define PHP_HTMLQUERY_H 1
 
#define PHP_HTMLQUERY_VERSION "1.0"
#define PHP_HTMLQUERY_EXTNAME "htmlquery"
 
PHP_FUNCTION(html_parse);
PHP_FUNCTION(html_get);
extern zend_module_entry htmlquery_module_entry;
#define phpext_htmlquery_ptr &htmlquery_module_entry
 
#endif