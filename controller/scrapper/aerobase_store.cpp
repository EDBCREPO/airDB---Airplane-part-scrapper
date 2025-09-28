#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/https.h>
#include <nodepp/zlib.h>
#include <xml/xml.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { namespace scrapper {

    promise_t<object_t,except_t> aerobase_store_search( string_t search ) {
    return promise_t<object_t,except_t>([=]( res_t<object_t> res, rej_t<except_t> rej ){

        if( search.empty() ){ rej( except_t( "invalid item name" ) ); return; }

        fetch_t args; agent_t agent; agent.recv_timeout = 1000;
        /*---*/ args.method = "GET"; /*----------------------*/
        /*---*/ args.url    = regex::format( "https://aerobase.store/search?term=${0}", search );
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
            auto out = array_t<object_t>();
            auto dom = xml::parse( raw );

            for( auto x: xml::find_element( dom, "[class*=product-list-item]" ) ){
                 auto w= xml::find_element( x  , "a" );
                 auto z= array_t<string_t>();

            for( auto y: x["children"].as<array_t<object_t>>() ){
                 z.push( xml::get_text( y ) );
            }if( z.empty() || w.empty() ){ continue; }

                 out.push( object_t({
                    { "part", regex::match( z[0], "  [^ ]+" ) }, 
                    { "url" , w[0]["attr"]["href"].as<string_t>() },
                    { "mnf" , z[3] }, { "name", z[1] }, { "info", "(null)" },
                    { "qnt" , "(null)" },{ "stt", "(null)" },{ "nsn" , "(null)" },
                 }) );

            }

            res( out ); return;
        }   res( nullptr ); });

    }); }

} }

/*────────────────────────────────────────────────────────────────────────────*/