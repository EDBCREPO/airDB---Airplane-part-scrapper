/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h> 
#include <express/http.h>
#include <nodepp/json.h>

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

#include "../controller/scrapper/conro_com.cpp"
#include "../controller/scrapper/silmid_com.cpp"
#include "../controller/scrapper/buynsn_com.cpp"
#include "../controller/scrapper/fast_search.cpp"
#include "../controller/scrapper/asapaxis_com.cpp"
#include "../controller/scrapper/aerobase_com.cpp"
#include "../controller/scrapper/aerobase_store.cpp"
#include "../controller/scrapper/asapbuying_com.cpp"
#include "../controller/scrapper/aerospheres_com.cpp"
#include "../controller/scrapper/meteoricaero_com.cpp"
#include "../controller/scrapper//gracoroberts_com.cpp"
#include "../controller/scrapper/aogpurchasing_com.cpp"
#include "../controller/scrapper/aerospaceaces_com.cpp"
#include "../controller/scrapper/aerospaceorbit_com.cpp"
#include "../controller/scrapper/afrenterprises_com.cpp"
#include "../controller/scrapper/aircraftspruce_com.cpp"
#include "../controller/scrapper/asap360unlimited_com.cpp"
#include "../controller/scrapper/aerospaceexchange_com.cpp"
#include "../controller/scrapper/fulfillmentbyasap_com.cpp"
#include "../controller/scrapper/aerospacesimplified_com.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

void onMain() {

    auto idx = type::bind( new int(0) );

    for( auto x: array_t<promise_t<object_t,except_t>>({
         controller::scrapper::aerospacesimplified_com_search( process::env::get("value") ),
         controller::scrapper::fulfillmentbyasap_com_search  ( process::env::get("value") ),
         controller::scrapper::aerospaceexchange_com_search  ( process::env::get("value") ),
         controller::scrapper::asap360unlimited_com_search   ( process::env::get("value") ),
         controller::scrapper::aircraftspruce_com_search     ( process::env::get("value") ),
         controller::scrapper::aerospaceorbit_com_search     ( process::env::get("value") ),
         controller::scrapper::afrenterprises_com_search     ( process::env::get("value") ),
         controller::scrapper::aogpurchasing_com_search      ( process::env::get("value") ),
         controller::scrapper::aerospaceaces_com_search      ( process::env::get("value") ),
         controller::scrapper::meteoricaero_com_search       ( process::env::get("value") ),
         controller::scrapper::gracoroberts_com_search       ( process::env::get("value") ),
         controller::scrapper::aerospheres_com_search        ( process::env::get("value") ),
         controller::scrapper::aerobase_store_search         ( process::env::get("value") ),
         controller::scrapper::asapbuying_com_search         ( process::env::get("value") ),
         controller::scrapper::aerobase_com_search           ( process::env::get("value") ),
         controller::scrapper::asapaxis_com_search           ( process::env::get("value") ),
         controller::scrapper::buynsn_com_search             ( process::env::get("value") ),
         controller::scrapper::silmid_com_search             ( process::env::get("value") ),
         controller::scrapper::conro_com_search              ( process::env::get("value") )
    }) ){

        console::log("AAA", process::env::get("value"), *idx );

        x.then([=]( object_t data ){

            console::done( "->>>>", json::stringify( data ) );

        *idx -= 1; });

        x.fail([=]( except_t err ){

            console::done( "->>>>", *idx );

        *idx -= 1; });

    *idx += 1; }

    process::add( coroutine::add( COROUTINE(){
    coBegin

        while( *idx > 0 ){
            console::log("hello world!");
        coDelay(1000); }

    coFinish
    }));

}

/*────────────────────────────────────────────────────────────────────────────*/