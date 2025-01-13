#include <nodepp/nodepp.h>
#include <nodepp/data.h>

using namespace nodepp;

void onMain(){

    console::log( "now->", data::now() );

    console::log(" -- -- ");

    console::log( "day->", data::day() );

    console::log( "year->", data::year() );

    console::log( "month->", data::month() );

    console::log(" -- -- ");

    console::log( "hours->", data::hours() );

    console::log( "minutes->", data::minutes() );

    console::log( "seconds->", data::seconds() );

}