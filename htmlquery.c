#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
 
#include "php.h"
#include "php_htmlquery.h"
#include "Zend/zend_alloc.h"

 
static zend_function_entry htmlquery_functions[] = {
    PHP_FE(html_parse, NULL)
    PHP_FE(html_get, NULL)
    {NULL, NULL, NULL}   
};

/*htmlquery_functions[1] = {
PHP_FE(html_get, NULL)
    {NULL, NULL, NULL}    
};*/
 
zend_module_entry htmlquery_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_HTMLQUERY_EXTNAME,
    htmlquery_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_HTMLQUERY_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
 
#ifdef COMPILE_DL_HTMLQUERY
ZEND_GET_MODULE(htmlquery)
#endif
void log_(char *s);
void errorstarttag(char *s, char *$stack, char *c, int $i);
void error_( char *str, int $i );
PHP_FUNCTION(html_get)
{
    char *$obj;
    int name_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &$obj, &name_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    } 
    
    printf("%i", (char)$obj[0]);    
}

int isLatinSymbol( char c )
{
    return ( (char)c > 96 && (char)c < 123 );
}
int isEmptySymbol( char c )
{
    return (c == ' ' || (char)c == 9 || (char)c == 13 || (char)c == 10);
}
int isStyleNameSymbol( char c )
{
    return ( ((char)c > 96 && (char)c < 123) || ( (char)c > 47 && (char)c < 58 ) || c == '-');
}

char* addSymbol( char* str, char c )
{
    
}
PHP_FUNCTION(html_parse)
{


 /*char *cBuffer;
  cBuffer = (char*)malloc(2048);
  memset(cBuffer, 0x00, 2048);

  strcpy(cBuffer, "echo A::B();");

  //execute decrypted php code via ZEND engine
  zend_try {
      zend_eval_string(cBuffer, NULL, "PHP Script");
      free(cBuffer);
  } zend_catch {
      free(cBuffer);
      php_error(E_ERROR, "Error occurred while executing PHP code, ERR - 101\0");
      RETURN_BOOL(0);
  } zend_end_try();
  return;*/



    
    char *$obj;
    int name_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &$obj, &name_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    }     

    
    int $i = 0;
    
    
    int $curel = -1;

    char * $stack = (char*) malloc(2000);
    
    $stack[0] = '~';
    $stack[1] = '\0';
   
    char *$tagname = (char*) malloc(170);
    
    int $tagnamelen = 0;
    
    char *$attrname = (char*) malloc(150);
    
    int $curattr = -1;
    char *$curattrvalue = (char*) malloc(1);
    
    char *$scriptclose = (char*) malloc(170);
    
    int $cursymtagclose = 0;
    
    
    int $openel[3000] = {};
    
    
    int $script = 0;
    
    int $curstack = 0;
    
    int $curopenel = -1;    
    
    int $curliteral = 0;
    
    
    //Переменные для стилей
    int $curstyle= -1;
    char *$stylename = (char*) malloc(50);
    char *$curstylevalue = (char*) malloc(450);
    int curstyleliteral = 0;
    ////////////////////
    
    int isAddAttrValue = 0;
    
    int script = 0;
    
    int $startattrname = -1;

    typedef struct
    {
        int startname;
        int startvalue;
        int endvalue;
        char *name;
        char *value;
         
        /**/
    } styles; 
    
    typedef struct
    {
        int startname;
        int startvalue;
        int endvalue;
        char *name;
        char *value;
         
        /**/
    } attributes;     
    typedef struct 
    {
        int start;
        int end;
        int closestarttag;
        char *tagname;
        attributes attrs[30];
        styles css[50];
        int count;
        int stylecount;
        
    } element;
    
    
    
    //element $elements[500];
    
    element * $elements;
    
    $elements= ( element * )calloc ( 100000, sizeof(element) );

    while ( $obj[$i] != '\0' )
    {
        isAddAttrValue = 0;
        
        char *c = (char*) malloc(2);
        
        c[0] = tolower( $obj[$i] );
        c[1] = '\0';        
         
        
        //printf("Current stack - %s\n", $stack );
         
        //printf("Current symbol - %c\n\n", c[0] );
        
         
        char *realc = (char*) malloc(2);        
        realc[0] = $obj[$i];
        realc[1] = '\0';        

        switch ( $stack[$curstack] )
        {
            case '>':

            case '~':  
                //if start tag in empty or closed tags
                if ( $obj[$i]  == '<' )
                {   
                    if ( script == 1 )
                    {
                        strcpy( $scriptclose, "" );
                        $curstack++;
                        $stack[$curstack] = 't';
                        break;    
                    }
                    //if ( $curel > -1 )
                    //printf("AFTER CLOSE %s %i, curel -%i\n" ,$elements[$curel].tagname, strlen( $elements[$curel].tagname), $curel);
                    
                    //Add new element
                    $curel++;
                    

                    
                    
                    $elements[$curel].start = $i;

                    
                    
                    $elements[$curel].end = 0;
                    
                    $curattr = -1;                        
                    
                    
                    $curopenel++;
                    //Add current element to elements stack
                    $openel[$curopenel] =$curel;
                    
                    
                    
                    $curstack++;
                    
                    
                
                    $stack[$curstack] = '<';
                    
                }
            break;
            
            

            
            case 't':
                strcat( $scriptclose, c ); 
                if ( strlen( $scriptclose ) == 8 )
                {
                    if ( strcmp($scriptclose, "/script>" ) == 0 )
                    {
                        $curstack = $curstack - 2;
                        $stack[$curstack+1] ='\0';
                        $stack[$curstack+2] ='\0';
                         
                        $elements[$openel[$curopenel]].end = $i;
                        
                        $curopenel--;
                        
                        if ( $curopenel > -1 )
                        {
                            strcpy($tagname, $elements[$openel[$curopenel]].tagname);
                            $tagnamelen = strlen($tagname);                        
                        }
                        else
                        {
                            strcpy($tagname,"");
                            $tagname[1]='\0';
                            $tagnamelen = 0;
                        }
                        
                        script = 0;                    
                        
                        break;
                    }
                    else
                    {
                        $stack[$curstack] = '\0';
                        $curstack--;
                    }
                }
                                           
            break; 
            case '!':
                if ( c[0] == '-' )
                {
                    $stack[$curstack] = '-';
                    break;
                }
                errorstarttag("Waiting comment continue", $stack, c, $i);
            break;
            case '-':
                if ( c[0] == '-' )
                {
                    $stack[$curstack] = 'w';
                    break;
                }
                errorstarttag("Waiting comment continue", $stack, c, $i);
            break;   
            case 'w':
                if ( c[0] == '-' )
                {
                    $stack[$curstack] = '1';
                }
            break;
            case '1':
                if ( c[0] == '-' )
                {
                    $stack[$curstack] = '2';
                    break;
                }
                $stack[$curstack] = 'w';
            break;  
            case '2':
                if ( c[0] == '>' )
                {
                        $elements[$curel].end = $i;                        
                        $curopenel--;
                        $stack[$curstack] = '\0';
                        $curstack--;
                        
                        $elements[$curel].tagname = (char*) malloc ( 8 );
                        
                        strcpy($elements[$curel].tagname, "comment");
                        $curel--;
                    break;
                }
                $stack[$curstack] = 'w';
            break;                 
            case '<':
                
                if ( c[0] == '!' )
                {
                    $stack[$curstack] = '!';
                    break;
                }
                if ( (char)c[0] > 96 && (char)c[0] < 123 )
                {


                    strcpy ( $tagname, c );
                    $stack[$curstack] = 'n';  
                    break;
                }
                
                //
                if ( $obj[$i] == '/' )
                {
                    $cursymtagclose = 0;
                    $stack[$curstack] ='s';
                    $curel--;
                    $curopenel--;
                    
                    
                    
                    if ( $curopenel > -1 )
                    {        
                        
                        strcpy($tagname, $elements[$openel[$curopenel]].tagname);     
                                                           
                        $tagnamelen = strlen($tagname);
                    }
                    else
                    {
                        strcpy($tagname, "");
                        $tagname[1]='\0';
                        $tagnamelen = 0;
                    }               
                    
                    
                    break;
                }

                errorstarttag("Destroy start tag, waiting tag name start", $stack, c, $i );              
            break;
            case 's':
                if ( $tagnamelen == 0 )
                {
                    $curstack--;
                    break;
                }
                if (  $cursymtagclose < $tagnamelen && tolower($obj[$i]) == $tagname[$cursymtagclose] )
                {
                    log_("Current tagname symbol and closetag symbol is identity and is not and");
                    $cursymtagclose++;
                    break;
                }
                if ( $tagnamelen == $cursymtagclose && $obj[$i] == '>' )
                {
                    log_("Tagname and closetagname is identity and symbol >");
                    $curstack = $curstack - 2;
                    $stack[$curstack+1] ='\0';
                    $stack[$curstack+2] ='\0';
                     
                    $elements[$openel[$curopenel]].end = $i;
                    
                    $curopenel--;
                    
                    if ( $curopenel > -1 )
                    {
                        strcpy($tagname, $elements[$openel[$curopenel]].tagname);
                        $tagnamelen = strlen($tagname);                        
                    }
                    else
                    {
                        strcpy($tagname,"");
                        $tagname[1]='\0';
                        $tagnamelen = 0;
                    }
                    
                    
                    
                    
                    break;    
                }
                //printf("HZ for close tagname, tagnamelen- %i, cursymtagclose - %i", $tagnamelen,$cursymtagclose);                 
                $curstack--;
                $stack[$curstack+1] ='\0';
            break;
            case 'n':
                if ( ((char)c[0] > 96 && (char)c[0] < 123) || ( (char)c[0] > 47 && (char)c[0] < 58 ) )
                {
                    strcat($tagname, c);
                    break;
                }
                if (  $obj[$i] == ' ' || (char)$obj[$i] == 9  )
                {
                    
                    $elements[$curel].tagname = (char*) malloc ( strlen($tagname) + 1 );
                    strcpy( $elements[$curel].tagname, $tagname );
                    
                    
                    
                    
                    
                    $tagnamelen = strlen($tagname);
                    
                    $stack[$curstack] = 'e';
                    break;
                }
                if ( $obj[$i] == '>' )
                {
                    
                    $elements[$curel].tagname = (char*) malloc ( strlen($tagname)+1);
                    strcpy($elements[$curel].tagname, $tagname);
                    
                    //$elements[$curel].tagname = strnew( $tagname );
                    
                    $tagnamelen = strlen($tagname);

                    
                    $stack[$curstack] = '>';
                    
                    $elements[$curel].closestarttag = $i;
                    
                    if ( strcmp( $tagname, "script" ) == 0 )
                    {
                        script = 1;    
                    }
                    
                    //printf("START TAG CLOSE, %s %i, curel -%i", $elements[$curel].tagname, strlen($elements[$curel].tagname), $curel );
                    
                    break;
                }
                if ( $obj[$i] == '/' && ( strcmp($tagname, "embed") || strcmp($tagname, "param") || strcmp($tagname, "base") || strcmp($tagname, "hr") || strcmp($tagname, "element") || strcmp($tagname, "img") || strcmp($tagname, "link") || strcmp($tagname, "br") || strcmp($tagname, "meta") || strcmp($tagname, "input")) )
                {
                    $stack[$curstack] = 'h';
                    break;                    
                }
                errorstarttag("Destroy start tag, waiting tag name continue", $stack, c, $i);
            break;
            case 'e':
                if (  $obj[$i] == ' ' || (char)$obj[$i] == 9 || (char)$obj[$i] == 13 || (char)$obj[$i] == 10  )
                {
                    break;
                }
                if ( (char)$obj[$i] > 96 && (char)$obj[$i] < 123 )
                {
                    $startattrname = $i;
                    strcpy($attrname, c );
                    $stack[$curstack] = 'a';
                    break;
                }
                if ( $obj[$i] == '/' && ( strcmp($tagname, "embed") == 0 || strcmp($tagname, "param") == 0 || strcmp($tagname, "base") == 0 || strcmp($tagname, "hr") == 0 || strcmp($tagname, "element") == 0 || strcmp($tagname, "img") == 0 || strcmp($tagname, "link") == 0 || strcmp($tagname, "br") == 0 || strcmp($tagname, "meta") == 0 || strcmp($tagname, "input") == 0 ) )
                {
                    $stack[$curstack] = 'h';
                    break;
                }
                if ( $obj[$i] == '>' )
                {
                    $elements[$curel].closestarttag = $i;
                    $stack[$curstack] = '>';
                    
                    if ( strcmp( $tagname, "script" ) == 0 )
                    {
                        script = 1;    
                    }                    
                    
                    break;
                }           
                
                errorstarttag("Destroy start tag, waiting tag attributes or and", $stack, c, $i);
            break;
            case 'h':
                if (  $obj[$i] != '>' )
                {                    
                    errorstarttag("Destroy start tag, waiting >", $stack, c, $i);
                }
                else
                {
                    $elements[$curel].closestarttag = 0;
                    $elements[$curel].end = $i;
                    $curopenel--;
                    
                    if ( $curopenel > -1 )
                    {
                        strcpy($tagname, $elements[$curopenel].tagname);
                        $tagnamelen = strlen($tagname);                        
                    }
                    else
                    {
                        strcpy($tagname, "");
                        $tagname[1]='\0';
                        $tagnamelen = 0;
                    }
                    $curstack--;
                }
            break;
            case 'a':
                if ( ((char)c[0] > 96 && (char)c[0] < 123) || ( (char)c[0] > 47 && (char)c[0] < 58 ) || $obj[$i] == '-' )
                {
                    
                    strcat( $attrname, c );
                    break;
                }
                if ( $obj[$i] == '=' )
                {
                    $curattr++;
                    
                    //printf("1");return;
                    $elements[$curel].attrs[$curattr].startname = $startattrname; 
                    $elements[$curel].attrs[$curattr].name = (char*) malloc ( strlen($attrname) + 1 );
                    
                    strcpy($elements[$curel].attrs[$curattr].name, $attrname);
                    
                    $elements[$curel].attrs[$curattr].value = (char*) malloc ( 1 );
                    
                    strcpy($elements[$curel].attrs[$curattr].value, "");
                    
                    
                    
                    $elements[$curel].count++;
                    
                    $stack[$curstack] = '=';
                    
                    break;
                }
                if ( $obj[$i] == '/' )
                {
                    $curattr++;
                    $elements[$curel].attrs[$curattr].startname = $startattrname;
                    $elements[$curel].attrs[$curattr].name = (char*) malloc ( strlen($attrname) + 1 );
                    
                    strcpy($elements[$curel].attrs[$curattr].name, $attrname);
                    
                    $elements[$curel].attrs[$curattr].value = (char*) malloc ( 1 );
                    
                    strcpy($elements[$curel].attrs[$curattr].value, "");                    
                    
                    
                    $elements[$curel].count++;
                    
                    $stack[$curstack] = 'h';
                    break;
                }
                if( $obj[$i] == ' ' || (char)$obj[$i] == 9 )
                {
                    
                    $curattr++;
                    //printf("3");return;
                    $elements[$curel].attrs[$curattr].startname = $startattrname;
                    $elements[$curel].attrs[$curattr].name = (char*) malloc ( strlen($attrname) + 1 );
                    
                    strcpy($elements[$curel].attrs[$curattr].name, $attrname);
                    
                    
                    $elements[$curel].attrs[$curattr].value = (char*) malloc ( 1 );
                    
                    strcpy($elements[$curel].attrs[$curattr].value, "");                    
                    
                    $elements[$curel].count++;
                    
                    $stack[$curstack] = 'c';
                    break;
                }
                errorstarttag("Destroy start tag, waiting attribute name", $stack, c, $i);
            break;
            case 'c':
                if( $obj[$i] == ' ' || (char)$obj[$i] == 9 )
                {
                    break;
                }
                if ( $obj[$i] == '=' )
                {
                    $stack[$curstack] = '=';
                    break;
                }
                if ( $obj[$i] == '/' )
                {
                    $stack[$curstack] = 'h'; 
                    break;                 
                }
                if ( $obj[$i] == '>' )
                {
                    
                    if ( strcmp( $tagname, "script" ) == 0 )
                    {
                        script = 1;    
                    }               
                    $elements[$curel].closestarttag = $i;     
                    $stack[$curstack] = '>';                        
                    break;
                }                                                
                if ( (char)$obj[$i] > 96 && (char)$obj[$i] < 123 )
                {
                    $startattrname = $i;
                    strcpy($attrname, c);
                    $stack[$curstack] = 'a';
                    break;                  
                }
                errorstarttag("Destroy start tag, waiting attribute value or and tag", $stack, c, $i);
            break;
            case '=':
                if ( $obj[$i] == '"' )
                {
                    strcpy($curattrvalue, "");
                    
                    $stack[$curstack] = '"';
                    
                    break;                    
                }                
                if( $obj[$i] == ' ' || (char)$obj[$i] == 9 )
                {
                    break;
                }

                errorstarttag("Destroy tag in stack = ", $stack, c, $i);
                return;  
            break;
            
            

            case '5':
                isAddAttrValue = 1;
                //Ждем начала значения стиля
                if ( isEmptySymbol( c[0] ) )
                {                    
                    break;
                }
                else
                {
                    $elements[$curel].css[$curstyle].startvalue = $i;
                    
                    strcpy($curstylevalue, "");
                    strcat( $curstylevalue, c );
                    curstyleliteral = 1;
                    
                    $stack[$curstack] = '6';
                    break;
                }
            break;
            case '4':
                isAddAttrValue = 1;
                //Начато имя стиля
                if ( isStyleNameSymbol( c[0] ) )
                {
                    strcat( $stylename, c );
                    break;
                }
                if ( c[0] == ':' )
                {
                    $elements[$curel].css[$curstyle].name = (char* )malloc( strlen($stylename) + 1 );
                    strcpy( $elements[$curel].css[$curstyle].name, $stylename );
                    
                    
                    
                    $stack[$curstack] = '5';
                    break;
                }
                if ( isEmptySymbol( c[0] ) )
                {                    
                    break;
                }

                errorstarttag("Invalid style, wait continue style name or : ", $stack, c, $i);
                return;  
            break;
            case '6':
                isAddAttrValue = 1;
                if ( c[0] == ';' || c[0] == '"' )
                {
                    
                    $elements[$curel].css[$curstyle].value = (char* )malloc( strlen($curstylevalue) + 1 );
                    strcpy( $elements[$curel].css[$curstyle].value, $curstylevalue );
                    $elements[$curel].css[$curstyle].endvalue = $i - 1;
                    
                    $elements[$curel].stylecount++;
                    
                    if ( c[0] == ';' )
                    {
                        $stack[$curstack] = '3';
                        break; 
                    }
                    
                    
                    
                    

                }
                if ( c[0] == '"' )
                {
                    $stack[$curstack] = '"';
                }
                else
                {
                    strcat( $curstylevalue, c );
                    //$curstylevalue[curstyleliteral] = realc[0];
                    //$curstylevalue[curstyleliteral+1]='\0';
                    //curstyleliteral++;
                    break;   
                }                
                        
            
            case '3':
                isAddAttrValue = 1;
                if ( $stack[$curstack] != '"' )
                {
                    if ( isLatinSymbol( c[0] ) )
                    {
                        strcpy( $stylename, "" );
                        strcat( $stylename, c );
                        
                        $curstyle++;
                        
                        $elements[$curel].css[$curstyle].startname = $i;
                        
                        $stack[$curstack] = '4';
                        break;
                    }                
                    if ( isEmptySymbol( c[0] ) )
                    {
                        break;
                    }
                    if ( c[0] == '"' )
                    {
                        $stack[$curstack] = '"';
                    }
                    else
                    {
                        if ( c[0] == ';' )
                        {
                            break;
                        }
                        errorstarttag("Invalid style, wait start style name ", $stack, c, $i);
                        return;                       
                    }
                }           
            //Значение аттрибута
            case '"':
                
                if ( $obj[$i] == '"' )
                {
                    //Закрываем текущий аттрибут
                        
                    //Присваиваем значение аттрибута
                    $elements[$curel].attrs[$curattr].value = (char*) malloc ( strlen($curattrvalue) + 1 );
                    strcpy($elements[$curel].attrs[$curattr].value, $curattrvalue);
                    
	                //Если не было задано начало значения аттрибута - задаем текущим
	                if ( $elements[$curel].attrs[$curattr].startvalue == 0 )
	                {
	                    $elements[$curel].attrs[$curattr].startvalue = $i; 
	                }                    
                    
                    //Заполняем конец аттрибута
                    $elements[$curel].attrs[$curattr].endvalue = $i - 1;

                    //Состояние - пустота после имени тега
                    $stack[$curstack] = 'e';
                    
                    //Обнуляем текущее значение аттрибута
                    $curattrvalue = (char *)malloc(1);                     
                    $curliteral = 0;
                    
                    isAddAttrValue = 0;
                    
                    break;
                }
                
                //Если не было задано начало значения аттрибута - задаем текущим
                if ( $elements[$curel].attrs[$curattr].startvalue == 0 )
                {
                    $elements[$curel].attrs[$curattr].startvalue = $i; 
                }
                

                //Добавляем текущий символ к значению аттрибута    
                isAddAttrValue = 1;
                //printf("Curliteral - %i", $curliteral );
                
                if ( $curliteral == 0 && strcmp($elements[$curel].attrs[$curattr].name, "style") == 0 )
                {
                    
                    if ( isLatinSymbol( c[0] ) )
                    {
                        strcpy( $stylename, "" );
                        strcat( $stylename, c );
                        //printf("START STYLE\n");
                        $curstyle=0;
                        
                        $elements[$curel].css[$curstyle].startname = $i;
                        
                        $stack[$curstack] = '4';
                        break;
                    }
                    if ( isEmptySymbol( c[0] ) )
                    {
                        $stack[$curstack] = '3';
                    }
                    errorstarttag("Invalid style, wait start style name ", $stack, c, $i);
                    return;                    
                }

            break;            
        }
        if ( isAddAttrValue == 1 )
        {
                //Переопределяем размер текущего значение аттрибута и добавляем к нему текущий символ
                    char * tmp = (char *)malloc ( ($curliteral + 1) *  sizeof(char) );
                    strcpy( tmp, $curattrvalue );
                    $curattrvalue = (char *)malloc ( ($curliteral + 2) *  sizeof(char) );                
                    strcpy( $curattrvalue, tmp  );
                    strcat( $curattrvalue, realc );
                    $curliteral++;            
        }
        free(c);
        $i++;
     
    }
    
    //$obj = NULL;
    
    
      

    $stack[$curstack+1] = '\0';


    
    if ( strlen( $stack ) > 1 )
    {
        errorstarttag("Stack is not empty", $stack,"",$i);
        return;
    }
    
    

    
    //ALLOC_INIT_ZVAL(return_value);
    
    array_init(return_value);
    
    for( $i=0; $i < $curel + 1; $i++ )
    {
        //printf("ss=%s", $elements[$i].tagname );
        if ( strcmp( $elements[$i].tagname, "comment" ) == 0 )
        {
            continue;
        }
        zval *arr;
        ALLOC_INIT_ZVAL(arr);
        array_init(arr);


        
        add_index_long(arr, 0, $elements[$i].start);
        add_index_long(arr, 1, $elements[$i].closestarttag);
        add_index_long(arr, 2, $elements[$i].end);
        add_index_string(arr, 3, $elements[$i].tagname , 1);
        
        
        
        zval *as;
        ALLOC_INIT_ZVAL(as);
        array_init(as);        
        
        
        int $k=0;
        
        //printf("tagname %s, attr count - %i\r\n",$elements[$i].tagname, $elements[$i].count);
        for( $k = 0; $k < $elements[$i].count; $k++ )
        {
            zval *at;
            ALLOC_INIT_ZVAL(at);
            array_init(at); 
            
            
            
            add_index_long( at, 0, $elements[$i].attrs[$k].startname );
            add_index_long( at, 1, $elements[$i].attrs[$k].startvalue );
            add_index_long( at, 2, $elements[$i].attrs[$k].endvalue );
           
            
            add_index_string( at, 3, $elements[$i].attrs[$k].value, 1 );
            
            
            add_assoc_zval( as, $elements[$i].attrs[$k].name, at  );
            
            //free ( $elements[$i].attrs[$k].value );
            //free ( $elements[$i].attrs[$k].name );
                        
        }
        
        add_index_zval(arr, 4, as );






        zval *sts;
        ALLOC_INIT_ZVAL(sts);
        array_init(sts);        
        

        
        $k=0;
        
        for( $k = 0; $k < $elements[$i].stylecount; $k++ )
        {
            zval *st;
            ALLOC_INIT_ZVAL(st);
            array_init(st); 
            
            
            
            add_index_long( st, 0, $elements[$i].css[$k].startname );
            add_index_long( st, 1, $elements[$i].css[$k].startvalue );
            add_index_long( st, 2, $elements[$i].css[$k].endvalue );
            
            add_index_string( st, 3, $elements[$i].css[$k].value, 1 );
            
            
            add_assoc_zval( sts, $elements[$i].css[$k].name, st );
        }
        
        add_index_zval(arr, 5, sts );















                
        add_index_zval(return_value, $i, arr );

    }
    
    
    
   free ($elements); 
}

void addstr( char *str, char s )
{
    int g = strlen(str);
    str[g-1] = s;
    str[g] = '\0';
}
void log_(char *s)
{
    //printf("%s\n", s );
}
void error_( char *str, int $i )
{
    zend_throw_exception(zend_exception_get_default(TSRMLS_C),str, 0 TSRMLS_CC);
    return;
}
void errorstarttag(char *s, char *$stack, char *c, int $i )
{
    char *str = (char*) malloc( strlen(s) + strlen($stack) + strlen(c) + 100 );
    strcpy( str, "Parse error - " );
    strcat( str, s );    
    strcat( str, ", current stack -«" );
    strcat( str, $stack );
    strcat( str, "», current symbol - «" );
    strcat( str, c );
    strcat( str, "» current num - " );
    char stri[10]; 
    sprintf(stri, "%d", $i); // преобразование    
    strcat( str, stri);
    zend_throw_exception(zend_exception_get_default(TSRMLS_C),str, $i TSRMLS_CC);
    return;    

    php_error_docref(NULL TSRMLS_CC, E_ERROR, str );
}