#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/https.h>
#include <nodepp/zlib.h>
#include <xml/xml.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { namespace scrapper {

    promise_t<object_t,except_t> fast_search( string_t search ) {
    return promise_t<object_t,except_t>([=]( res_t<object_t> res, rej_t<except_t> rej ){

        if( search.empty() ){ rej( except_t( "invalid item name" ) ); return; }

        fetch_t args; agent_t agent; agent.recv_timeout = 1000;
        /*---*/ args.method = "GET"; /*----------------------*/
        /*---*/ args.url    = regex::format( "https://www.boeingdistribution.com/en/search/typeAheadSearchService.jsp?query=${0}", search );
        /*---*/ args.headers= header_t({
            { "User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:141.0) Gecko/20100101 Firefox/141.0" },
            { "Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" },
            { "Accept-Language", "en-US,en;q=0.5" },
            { "Host", url::hostname( args.url ) },
            { "Accept-Encoding", "gzip" },
        });

        https::fetch( args, nullptr, &agent )

        .fail([=]( except_t ){ rej( except_t( "could not connect to the server" ) ); })

        .then([=]( https_t cli ){ if( cli.status == 200 ){

            auto raw = zlib::gunzip::await( cli );
            auto obj = json::parse( regex::replace_all( raw, "&quot;", "\"" ) );
            auto out = array_t<object_t>(); /*-------------------------------*/
            
            for( auto x: obj["results"].as<array_t<object_t>>() ){
            if ( !regex::test( x.as<string_t>(), search ) ){ continue; }
                 out.push( x );
            }

            res( out ); return;
        }   res( nullptr ); });

    }); }

} }

/*────────────────────────────────────────────────────────────────────────────*/