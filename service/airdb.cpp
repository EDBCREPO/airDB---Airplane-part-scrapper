/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h> 
#include <express/http.h>
#include <nodepp/json.h>
#include <xml/xml.h>

using namespace nodepp;

#include "../controller/api.cpp"
#include "../controller/search.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

void onMain() {

    auto app = express::http::add();

    app.USE( "/search", controller::search_controller_app() );
    app.USE( "/api"   , controller::api_controller_app() );
    app.USE( express::http::file( "./view" ) );

    app.listen( "localhost", 8000, [=]( socket_t ){
        console::log( "-> http://localhost:8000" );
    });

}

/*────────────────────────────────────────────────────────────────────────────*/