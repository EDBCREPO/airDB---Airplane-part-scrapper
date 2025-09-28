#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/https.h>
#include <nodepp/zlib.h>
#include <xml/xml.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { namespace scrapper {

    promise_t<object_t,except_t> asap360unlimited_com_search( string_t search ) {
    return promise_t<object_t,except_t>([=]( res_t<object_t> res, rej_t<except_t> rej ){

        if( search.empty() ){ rej( except_t( "invalid item name" ) ); return; }

        fetch_t args; agent_t agent; agent.recv_timeout = 1000;
        /*---*/ args.method = "GET"; /*----------------------*/
        /*---*/ args.url    = regex::format( "https://www.asap360unlimited.com/partno-search?searchby=partno&searchkey=${0}", search );
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
            auto out = array_t<object_t>();
            auto dom = xml::parse( raw );

            for( auto x: xml::find_element( dom, "[class*=civil-aviation-search-table]" ) ){
            for( auto y: xml::find_element( x  , "tr" ) ){
                 auto w= xml::find_element( y  , "a[href][rel]" );
                 auto z= xml::find_element( y  , "td" );
            if ( w.empty() || z.empty() ) { continue; }

                out.push( object_t({
                    { "name", xml::get_text( z[0] ) },
                    { "mnf" , xml::get_text( z[1] ) },
                    { "info", xml::get_text( z[2] ) },
                    { "qnt" , "(null)" },{ "stt" , "(null)" },
                    { "nsn" , "(null)" },{ "part", "(null)" },
                    { "url" , w[0]["attr"]["href"].as<string_t>() },
                }) );

            }}

            res( out ); return;
        }   res( nullptr ); });

    }); }

} }

/*────────────────────────────────────────────────────────────────────────────*/