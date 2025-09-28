#define PAYLOAD "curl https://www.aerospheres.com/search-results/?searchterm=${0}&searchterm_submit=Go --connect-timeout 5 -Ls"
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/popen.h>
#include <xml/xml.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { namespace scrapper {

    promise_t<object_t,except_t> aerospheres_com_search( string_t search ) {
    return promise_t<object_t,except_t>([=]( res_t<object_t> res, rej_t<except_t> rej ){

        if( search.empty() ){ rej( except_t( "invalid item name" ) ); return; }

        auto uri = regex::format( PAYLOAD, search );
        auto pid = popen::async( uri ); auto blk = ptr_t<bool>( new bool(true) ); auto brr = ptr_t<string_t>( new string_t() );

        pid.onData ([=]( string_t data ){ *brr += data; });
        pid.onDrain([=](){ *blk = false; }); 

        process::add( coroutine::add( COROUTINE(){
        coBegin ; coWait( *blk ); do {

            auto raw = regex::match( *brr, "<div id=\"ctl00_Content_zneContent5_ctl02_FacetedResults1\"[^\n]+" );
            auto out = array_t<object_t>(); auto obj = xml::parse( raw );

            for( auto x: xml::find_element( obj, "[class*=product-display-box]" ) ){
            for( auto y: xml::find_element( x  , "a[product]" ) ){
            if ( !xml::has_attribute( y, "title" ) ){ continue; }

                 out.push( object_t({
                    { "name", xml::get_attribute( y, "title" ) },
                    { "url" , xml::get_attribute( y, "href" ) },
                    { "mnf" , "(null)" },{ "info", "(null)" },
                    { "qnt" , "(null)" },{ "stt" , "(null)" },
                    { "nsn" , "(null)" },{ "part", "(null)" },
                }) );

            }}

            res( out );

        } while(0) ; coFinish
        }));

    }); }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#undef PAYLOAD