#pragma once

#include "./scrapper/conro_com.cpp"
#include "./scrapper/silmid_com.cpp"
#include "./scrapper/buynsn_com.cpp"
#include "./scrapper/fast_search.cpp"
#include "./scrapper/asapaxis_com.cpp"
#include "./scrapper/aerobase_com.cpp"
#include "./scrapper/aerobase_store.cpp"
#include "./scrapper/asapbuying_com.cpp"
#include "./scrapper/aerospheres_com.cpp"
#include "./scrapper/meteoricaero_com.cpp"
#include "./scrapper//gracoroberts_com.cpp"
#include "./scrapper/aogpurchasing_com.cpp"
#include "./scrapper/aerospaceaces_com.cpp"
#include "./scrapper/aerospaceorbit_com.cpp"
#include "./scrapper/afrenterprises_com.cpp"
#include "./scrapper/aircraftspruce_com.cpp"
#include "./scrapper/asap360unlimited_com.cpp"
#include "./scrapper/aerospaceexchange_com.cpp"
#include "./scrapper/fulfillmentbyasap_com.cpp"
#include "./scrapper/aerospacesimplified_com.cpp"

namespace controller { express_tcp_t api_controller_app() {

    auto app = express::http::add();

    app.GET( "/search/:value", [=]( express_http_t cli ){ try {
        
        cli.header( "Transfer-Encoding", "chunked" );
        cli.header( "Content-Type", "text/plain" );
        cli.send(); /*---------------------------*/

        auto lst = array_t<promise_t<object_t,except_t>>({
            controller::scrapper::aerospacesimplified_com_search( cli.params["value"] ),
            controller::scrapper::fulfillmentbyasap_com_search  ( cli.params["value"] ),
            controller::scrapper::aerospaceexchange_com_search  ( cli.params["value"] ),
            controller::scrapper::asap360unlimited_com_search   ( cli.params["value"] ),
            controller::scrapper::aircraftspruce_com_search     ( cli.params["value"] ),
            controller::scrapper::aerospaceorbit_com_search     ( cli.params["value"] ),
            controller::scrapper::afrenterprises_com_search     ( cli.params["value"] ),
            controller::scrapper::aogpurchasing_com_search      ( cli.params["value"] ),
            controller::scrapper::aerospaceaces_com_search      ( cli.params["value"] ),
            controller::scrapper::meteoricaero_com_search       ( cli.params["value"] ),
            controller::scrapper::gracoroberts_com_search       ( cli.params["value"] ),
            controller::scrapper::aerospheres_com_search        ( cli.params["value"] ),
            controller::scrapper::aerobase_store_search         ( cli.params["value"] ),
            controller::scrapper::asapbuying_com_search         ( cli.params["value"] ),
            controller::scrapper::aerobase_com_search           ( cli.params["value"] ),
            controller::scrapper::asapaxis_com_search           ( cli.params["value"] ),
            controller::scrapper::buynsn_com_search             ( cli.params["value"] ),
            controller::scrapper::silmid_com_search             ( cli.params["value"] ),
            controller::scrapper::conro_com_search              ( cli.params["value"] )
        });

        auto idx = type::bind( new int(0) ); for( auto x: lst ){
        auto prm = type::bind( x );

        process::add( coroutine::add( COROUTINE(){
            if( cli.is_closed() ){ prm->off(); return -1; }
        coBegin ; coWait( *idx >= 5 ); *idx += 1;

            prm->then([=]( object_t data ){ 
            if( !data.has_value() ){ goto DONE; } do {

                auto msg = json::stringify( data );
                cli.write( encoder::hex::get( msg.size() ) + "\r\n" + msg + "\r\n" );

            } while(0); DONE:; *idx -= 1; });

            prm->fail([=]( except_t ){ *idx -= 1; });

        coWait( *idx == 0 ) ; coFinish
        })); } 

    } catch(...) {} });

    app.GET( "/match/:value", [=]( express_http_t cli ){ try {

        cli.send( json::stringify( object_t({
            { "status", 200 }, /*-----------*/
            { "msg"   , cli.params["value"] }
        }) ));

    } catch(...) {} });

    app.ALL([=]( express_http_t cli ){

        cli.status(404).send( json::stringify( object_t({
            { "status", 404 }, /*-----------*/
            { "msg"   , "invalid api module" }
        }) ));

    });

    return app;

}}