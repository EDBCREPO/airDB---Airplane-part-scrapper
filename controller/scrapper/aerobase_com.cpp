#define PAYLOAD "draw=1&columns%5B0%5D%5Bdata%5D=0&columns%5B0%5D%5Bname%5D=&columns%5B0%5D%5Bsearchable%5D=true&columns%5B0%5D%5Borderable%5D=false&columns%5B0%5D%5Bsearch%5D%5Bvalue%5D=&columns%5B0%5D%5Bsearch%5D%5Bregex%5D=false&columns%5B1%5D%5Bdata%5D=1&columns%5B1%5D%5Bname%5D=&columns%5B1%5D%5Bsearchable%5D=true&columns%5B1%5D%5Borderable%5D=false&columns%5B1%5D%5Bsearch%5D%5Bvalue%5D=&columns%5B1%5D%5Bsearch%5D%5Bregex%5D=false&columns%5B2%5D%5Bdata%5D=2&columns%5B2%5D%5Bname%5D=&columns%5B2%5D%5Bsearchable%5D=true&columns%5B2%5D%5Borderable%5D=false&columns%5B2%5D%5Bsearch%5D%5Bvalue%5D=&columns%5B2%5D%5Bsearch%5D%5Bregex%5D=false&start=0&length=25&search%5Bvalue%5D=&search%5Bregex%5D=false&term=${0}"
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/https.h>
#include <nodepp/zlib.h>
#include <xml/xml.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { namespace scrapper {

    promise_t<object_t,except_t> aerobase_com_search( string_t search ) {
    return promise_t<object_t,except_t>([=]( res_t<object_t> res, rej_t<except_t> rej ){

        if( search.empty() ){ rej( except_t( "invalid item name" ) ); return; }

        fetch_t args; agent_t agent ; agent.recv_timeout = 1000;
        /*---*/ args.method = "POST"; /*----------------------*/
        /*---*/ args.body   = regex::format( PAYLOAD, search );
        /*---*/ args.url    = "https://aerobasegroup.com/search/_page/";
        /*---*/ args.headers= header_t({
            { "User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:141.0) Gecko/20100101 Firefox/141.0" },
            { "Content-Type", "application/x-www-form-urlencoded; charset=UTF-8" },
            { "Accept", "application/json, text/javascript, */*; q=0.01" },
            { "Host", url::hostname( args.url ) },
            { "Accept-Encoding", "gzip" }
        });

        https::fetch( args, nullptr, &agent )

        .fail([=]( except_t ){ rej( except_t( "could not connect to the server" ) ); })

        .then([=]( https_t cli ){ if( cli.status == 200 ){

            auto raw = zlib::gunzip::await( cli );
            auto dom = json::parse( raw )["HTML"].as<string_t>(); 
                 dom = regex::remove_all( dom, "[\\\\](t|r|n)|\\\\" );

            auto html= xml::parse( dom ); 
            auto out = array_t<object_t>();

            for( auto x: xml::find_element( html, "tr" ) ){ 
                 auto w= xml::find_element( html, "a" );
                 auto z= array_t<string_t>();

            for( auto y: xml::find_element( x   , "td" ) ){
                 z.push( xml::get_text( y ) );
            }if( w.empty() || w.empty() ){ continue; }

                 out.push( object_t({
                    { "mnf" , "(null)" },
                    { "part", z[1] }, { "info", z[2] },
                    { "name", regex::match( z[0], " .+" ) },
                    { "qnt" , "(null)" },{ "stt", "(null)" },
                    { "nsn" , regex::match( z[0], "^[^ ]+" ) },
                    { "url" , w[0]["attr"]["href"].as<string_t>() },
                 }) );
            
            }

            res( out ); return;
        }   res( nullptr ); });

    }); }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#undef PAYLOAD