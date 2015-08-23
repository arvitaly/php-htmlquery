PHP_ARG_ENABLE(htmlquery, whether to enable HTML Query support,
[ --enable-htmlquery   Enable HTML Query support])
 
if test "$PHP_HTMLQUERY" = "yes"; then
  AC_DEFINE(HAVE_HTMLQUERY, 1, [Whether you have HTML Query])
  PHP_NEW_EXTENSION(htmlquery, htmlquery.c, $ext_shared)
fi