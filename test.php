<?php
var_dump( html_parse('<a target="_blank" href="#" class="ico"><img src="#" width="30" height="30" title="" /></a>'));
exit;
    require '/var/www/sites/donorsearch.ru/release/init.php';

    
    $html = new HTMLQuery( '<input type="hidden" name=""  /><p><a></a></p>');
    
    //$html->find('t')->replace('<bb>22</bb>');
    
    //$html('a')->css('t','aa');
    
    
    var_dump($html->dom());
    //var_dump($html('h2')->css('t'));