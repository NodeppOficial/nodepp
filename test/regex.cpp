
namespace TEST { namespace REGEX {

    void TEST_RUNNER(){
        ptr_t<uint> totl = new uint(0);
        ptr_t<uint> done = new uint(0);
        ptr_t<uint> err  = new uint(0);
        ptr_t<uint> skp  = new uint(0);

        auto test = TEST_CREATE();

        TEST_ADD( test, "TEST 1 | regex 1", [](){
            try {
                regex_t reg ( "\\w+|\\d+" );
                auto all = reg.match_all(" 0123 hola ");
                if ( all[0] != "0123" || all[1] != "hola" ) 
                   { TEST_FAIL(); } TEST_DONE();
            } catch ( ... ) {
                TEST_FAIL();
            }
        });

        TEST_ADD( test, "TEST 2 | regex 2", [](){
            try {
                regex_t reg ( "\\d{0,3}" );
                auto all = reg.match("0123456789");
                if ( all!= "012" ) 
                   { TEST_FAIL(); } TEST_DONE();
            } catch ( ... ) {
                TEST_FAIL();
            }
        });

        TEST_ADD( test, "TEST 3 | regex 3", [](){
            try {
                regex_t reg ( "\\d{2,3}" );
                auto all = reg.match("0123456789");
                if ( all.size() < 2 || all.size() > 3 ) 
                   { TEST_FAIL(); } TEST_DONE();
            } catch ( ... ) {
                TEST_FAIL();
            }
        });

        TEST_ADD( test, "TEST 4 | regex 4", [](){
            try {
                regex_t reg ( "\\w" );
                auto all = reg.match_all("hola");
                if ( all.size() != 4 ) 
                   { TEST_FAIL(); } TEST_DONE();
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