
#include <nodepp/timer.h>

namespace TEST { namespace COROUTINE {

    void TEST_RUNNER(){
        ptr_t<uint> totl = new uint(0);
        ptr_t<uint> done = new uint(0);
        ptr_t<uint> err  = new uint(0);
        ptr_t<uint> skp  = new uint(0);

        auto test = TEST_CREATE();

        TEST_ADD( test, "TEST 1 | process::add ", [](){
            try {
                ptr_t<uint> x = new uint(0);

                auto time = process::add([=](){
                coStart
                     (*x)++; coGoto(0);
                coStop
                });

                while( *x < 10 ){ process::next(); }

                process::clear( time ); 
                process::next();

                if( *x != 10 ){ TEST_FAIL(); }

                TEST_DONE();
            } catch ( ... ) {
                TEST_FAIL();
            }
        });

        TEST_ADD( test, "TEST 2 | process::add ", [](){
            try {
                ptr_t<uint> x = new uint(0);

                auto time = process::add([=](){
                coStart
                     (*x)++; // coGoto(0);
                coStop
                });

                while( *x != 0 ){ process::next(); }

                TEST_DONE();
            } catch ( ... ) {
                TEST_FAIL();
            }
        });

        test.onClose.once([=](){
            console::log(":> RESULT | total:", *totl, "| passed:", *done, "| error:", *err, "| skipped:", *skp );
        });

        test.onDone([=](){ (*done)++; (*totl)++; });
        test.onFail([=](){ (*err)++;  (*totl)++; });
        test.onSkip([=](){ (*skp)++;  (*totl)++; });

        TEST_AWAIT( test );

    }

}}