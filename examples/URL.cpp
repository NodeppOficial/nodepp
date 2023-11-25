#include <node++/node++.h>
#include <node++/url.h>

using namespace nodepp;

void _Ready() {
	
    string_t uri = "http://becerra:enmanuel@www.google.com/mojon#done?mojon=10";

    auto ppt = url::parse( uri ); 

    console::log( "hostname:", ppt.hostname );
    console::log( "protocol:", ppt.protocol );
    console::log( "pathname:", ppt.pathname );
    console::log( "search:",   ppt.search );
    console::log( "origin:",   ppt.origin );
    console::log( "port:",     ppt.port );
    console::log( "host:",     ppt.host );
    console::log( "hash:",     ppt.hash );
    console::log( "user:",     ppt.user );
    console::log( "pass:",     ppt.pass );
    console::log( "auth:",     ppt.auth );
    console::log( "href:",     ppt.href );

}