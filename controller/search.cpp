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

namespace controller { express_tcp_t search_controller_app() {

    auto app = express::http::add();

    app.ALL([=]( express_http_t cli ){ try {

        if( !cli.query.has("value") ){ throw except_t( "invalid search value" ); }
        
        cli.header( "Transfer-Encoding", "chunked" );
        cli.header( "Content-Type", "text/html" );
        cli.send(); /*--------------------------*/

        auto msg = fs::read_file( "./view/search.html" );
        cli.write( encoder::hex::get( msg.size() ) + "\r\n" + msg + "\r\n" );

        auto lst = array_t<promise_t<object_t,except_t>>({
            controller::scrapper::aerospacesimplified_com_search( cli.query["value"] ),
            controller::scrapper::fulfillmentbyasap_com_search  ( cli.query["value"] ),
            controller::scrapper::aerospaceexchange_com_search  ( cli.query["value"] ),
            controller::scrapper::asap360unlimited_com_search   ( cli.query["value"] ),
            controller::scrapper::aircraftspruce_com_search     ( cli.query["value"] ),
            controller::scrapper::aerospaceorbit_com_search     ( cli.query["value"] ),
            controller::scrapper::afrenterprises_com_search     ( cli.query["value"] ),
            controller::scrapper::aogpurchasing_com_search      ( cli.query["value"] ),
            controller::scrapper::aerospaceaces_com_search      ( cli.query["value"] ),
            controller::scrapper::meteoricaero_com_search       ( cli.query["value"] ),
            controller::scrapper::gracoroberts_com_search       ( cli.query["value"] ),
            controller::scrapper::aerospheres_com_search        ( cli.query["value"] ),
            controller::scrapper::aerobase_store_search         ( cli.query["value"] ),
            controller::scrapper::asapbuying_com_search         ( cli.query["value"] ),
            controller::scrapper::aerobase_com_search           ( cli.query["value"] ),
            controller::scrapper::asapaxis_com_search           ( cli.query["value"] ),
            controller::scrapper::buynsn_com_search             ( cli.query["value"] ),
            controller::scrapper::silmid_com_search             ( cli.query["value"] ),
            controller::scrapper::conro_com_search              ( cli.query["value"] )
        });

        auto idx = type::bind( new int(0) ); for( auto x: lst ){
        auto prm = type::bind( x );

        process::add( coroutine::add( COROUTINE(){
            if( cli.is_closed() ){ prm->off(); return -1; }
        coBegin ; coWait( *idx >= 5 ); *idx += 1;

            prm->then([=]( object_t data ){ 
            if( !data.has_value() ){ goto DONE; } do { string_t msg;

                for( auto x: data.as<array_t<object_t>>() ){

                    msg += regex::format(R"(
                        <div class="uk-border uk-rounded uk-animation uk-padding
                                    uk-flex uk-flex-row uk-flex-between"
                            animation="fade" >
                        <p>${0}</p> <!-----> <p>${1}</p> 
                        <p class="uk-width-1-2">${2}</p>
                        <a class="uk-button uk-button-success" 
                           href="${3}" > Ver </a>
                        </div>
                    )", x["name"].as<string_t>(), x["mnf"].as<string_t>()
                      , x["info"].as<string_t>(), x["url"].as<string_t>() );

                } if( msg.empty() ){ continue; }

                cli.write( encoder::hex::get( msg.size() ) + "\r\n" + msg + "\r\n" );

            } while(0); DONE:; *idx -= 1; });

            prm->fail([=]( except_t ){ *idx -= 1; });

        coWait( *idx == 0 ) ; coFinish
        })); } 

    } catch(...) {} });

    app.ALL([=]( express_http_t cli ){ cli.redirect( "/" ); });

    return app;

}}