
#include <nodepp/timer.h>

namespace TEST { namespace TIMER {

    void TEST_RUNNER(){
        ptr_t<uint> totl = new uint(0);
        ptr_t<uint> done = new uint(0);
        ptr_t<uint> err  = new uint(0);
        ptr_t<uint> skp  = new uint(0);

        auto test = TEST_CREATE();

        TEST_ADD( test, "TEST 1 | timer::interval ", [](){
            try {
                ptr_t<uint> x = new uint(0);

                auto time = timer::interval([=](){
                     (*x)++;
                },100);

                while( *x < 10 ){ process::next(); }

                timer::clear( time ); 
                process::next();

                if( *x != 10 ){ TEST_FAIL(); }

                TEST_DONE();
            } catch ( ... ) {
                TEST_FAIL();
            }
        });

        TEST_ADD( test, "TEST 2 | timer::timeout ", [](){
            try {
                ptr_t<bool> x = new bool(0);

                auto time = timer::timeout([=](){
                     *x =! *x;
                },100);

                while( *x == false ){ process::next(); }

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