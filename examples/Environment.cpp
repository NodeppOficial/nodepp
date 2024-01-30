#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/http.h>
#include <nodepp/path.h>
#include <nodepp/date.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    process::env::init("file.env");
    console::log( process::env::get("VARA") );
    console::log( process::env::get("VARB") );

}

/*────────────────────────────────────────────────────────────────────────────*/