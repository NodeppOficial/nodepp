/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.nodepp.xyz/license.html
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_JSON
#define NODEPP_JSON

/*────────────────────────────────────────────────────────────────────────────*/

#include "object.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace json {

string_t format( const object_t& json ){
    return nullptr;
}

object_t parse( const string_t& json ){ 
    return nullptr;
}

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif