#include <nodepp/nodepp.h>
#include <nodepp/matrix.h>

using namespace nodepp;

onMain([](){

    matrix_t<int> A ( 3, 3 );
    A.set({
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    });

    matrix_t<int> B ( 2, 2 );
    B.set({
        0, 1,
       -1, 0
    });

    auto C = matrix::conv( A, B );

    for( auto &x: C ){ console::log(x); }

})