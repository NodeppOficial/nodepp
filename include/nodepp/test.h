/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.nodepp.xyz/license.html
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_TEST
#define NODEPP_TEST

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace test {

    struct NODE {
        bool                     enable;
        queue_t<function_t<int>> queue;
        string_t                 name;
    };  queue_t<NODE> group;

    template< class... T >
    void push( const string_t& name, const function_t<int,T...>& func ) {
        
    }

    void run() {

    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#define TEST_UNIGNORE( NAME ) nodepp::test::set(0);
    auto y = nodepp::test::group.get_index([]( NODE x ){ return x.name==NAME; }); \
    if ( y>= 0 ){ nodepp::test::group[y]->data.enable = 0; }

#define TEST_IGNORE( NAME ) nodepp::test::set(1);
    auto y = nodepp::test::group.get_index([]( NODE x ){ return x.name==NAME; }); \
    if ( y>= 0 ){ nodepp::test::group[y]->data.enable = 1; }

#define TEST_ADD(NAME) nodepp::test::add(NAME);
    auto y = nodepp::test::group.get_index([]( NODE x ){ return x.name==NAME; }); \
    if ( y < 0 ){ \
        nodepp::test::NODE item; item.name = NAME; \
        nodepp::test::group[y].push( NODE ); \
    }

#define TEST_SET(NAME,...) nodepp::test::push( NAME, __VA_ARGS__ );

#define TEST_ASSERT ( CONDITION ) if( !CONDITION ) TEST_FAIL();

#define TEST_END( NAME ) TEST_IGNORE( NAME )

#define TEST_RUN(NAME) nodepp::test::run();

#define TEST_FAIL() return -1;

#define TEST_DONE() return  1; 

/*────────────────────────────────────────────────────────────────────────────*/

#endif