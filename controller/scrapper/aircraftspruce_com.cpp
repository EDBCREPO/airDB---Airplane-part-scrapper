#define PAYLOAD "s=${0}"
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/https.h>
#include <nodepp/zlib.h>
#include <xml/xml.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { namespace scrapper {

    promise_t<object_t,except_t> aircraftspruce_com_search( string_t search ) {
    return promise_t<object_t,except_t>([=]( res_t<object_t> res, rej_t<except_t> rej ){

        if( search.empty() ){ rej( except_t( "invalid item name" ) ); return; }

        fetch_t args; agent_t agent ; agent.recv_timeout = 1000;
        /*---*/ args.method = "POST"; /*----------------------*/
        /*---*/ args.body   = regex::format( PAYLOAD, search );
        /*---*/ args.url    = "https://www.aircraftspruce.com/search/search.php";
        /*---*/ args.headers= header_t({
            { "User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:141.0) Gecko/20100101 Firefox/141.0" },
            { "Referer", "https://www.aircraftspruce.com/catalog/hapages/planeparts_06-04538.php" },
            { "Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" },
            { "Content-Type", "application/x-www-form-urlencoded" },
            { "Origin", "https://www.aircraftspruce.com" },
            { "Accept-Language", "en-US,en;q=0.5" },
            { "Host", url::hostname( args.url ) },
            { "Accept-Encoding", "gzip" },
        });

        https::fetch( args, nullptr, &agent )

        .fail([=]( except_t ){ rej( except_t( "could not connect to the server" ) ); })

        .then([=]( https_t cli ){ if( cli.status == 200 ){

            auto raw = zlib::gunzip::await( cli );
            auto out = array_t<object_t>();

            for( auto x: ptr_t<string_t>({
                 "\\$\\(document\\)\\.search[^\\[]+(\\[[^\\]]+\\])",
                 "\\$\\.thumbitems[^\\[]+(\\[[^\\]]+\\])"
            }) ){

                auto data= regex::get_memory( raw, x );
                if ( data.empty() ){ continue; }

                auto obj = json::parse( regex::remove_all( data.join(""), "\\\\" ) );

                for( auto x: obj.as<array_t<object_t>>() ){
                    out.push( object_t({
                        { "url" , x["url"]  .as<string_t>() },
                        { "name", x["title"].as<string_t>() },
                        { "qnt" , "(null)" },{ "stt" , "(null)" },
                        { "mnf" , "(null)" },{ "nsn" , "(null)" },
                        { "part", "(null)" },{ "info", "(null)" },
                    }) );
                }

            }
            
            res( out ); return;
        }   res( nullptr ); });

    }); }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#undef PAYLOAD