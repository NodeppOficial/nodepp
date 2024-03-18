/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.nodepp.xyz/license.html
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_REGEX
#define NODEPP_REGEX

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class regex_t { 
protected:

    struct NODE {
        string_t data; queue_t<NODE> alt;
        ptr_t<int>rep; char flg = '\0';
    };  
    
    queue_t<NODE> queue; bool i;
    array_t<string_t> args;
    
    /*─······································································─*/

    string_t compile_r( string_t& nreg ) noexcept {
        string_t reg;

        for( ulong x=0; x<nreg.size(); x++ ){
            if ( nreg[x]=='\\' ){ x++;
            if ( x < nreg.size() ){ reg.push( nreg[x] ); }
            } elif ( nreg[x+1] == '-' && (x+2)<nreg.size() ){
                auto a = min( nreg[x], nreg[x+2] ); 
                auto b = max( nreg[x], nreg[x+2] );
                for( auto j=a; j<=b; j++ ){ reg.push(j); }
            } else { reg.push( nreg[x] ); }
        }

        return reg;
    }
    
    /*─······································································─*/

    ptr_t<int> compile_n( string_t& nreg ) noexcept {
        bool b=0; string_t num[2]; ptr_t<int> rep ({ 0, 0 }); 
        
        for ( auto &y: nreg ) {
          if(!string::is_digit(y) ){ b =! b; }
        elif( string::is_digit(y) ){ num[b].push(y); }
        }

        if( !num[0].empty() ) rep[0] = string::to_int( num[0] );
        if( !num[1].empty() ) rep[1] = string::to_int( num[1] );

        return ( rep[0] != rep[1] ) ? rep : nullptr;
    }
    
    /*─······································································─*/

    void node_push( NODE* node, string_t& str, NODE* nNode, NODE*& last ) noexcept {
        NODE sNode; sNode.data = str.copy();
        if( !str.empty() )  { node->alt.push( sNode ); }
        if( nNode!=nullptr ){ node->alt.push(*nNode ); } str.clear();
        last = node->alt.empty() ? nullptr : &node->alt.last()->data;
    }
    
    /*─······································································─*/

    queue_t<NODE> compile( const string_t& reg ) {

        NODE *node=nullptr, *last=nullptr; queue_t<NODE> sub; bool b=0; string_t str;

        for( ulong x=0; x<reg.size(); x++ ){ char y = reg[x];
        
        if ( y == ']' || y == '}' || y == ')' ){ _ERROR(string::format( "regex: %d %c", x, y )); }
        if ( node == nullptr ){ sub.push( NODE() ); node =& sub.last()->data; node->flg = '\3'; }
    
    /*─······································································─*/

            if ( y == '|' ){
                 node_push( node, str, nullptr, last ); sub.push( NODE() ); 
                 node =& sub.last()->data; node->flg = '\3'; continue; 
            }    last = node->alt.empty() ? nullptr : &node->alt.last()->data;
    
    /*─······································································─*/

            if( y == '(' ){ string_t nreg; uint k=1; x++; while( k != 0 ){
              if( reg[x]=='(' ){ k++; } if ( reg[x]==')' ){ k--; }
              if( k != 0 ){ nreg.push( reg[x] ); x++; }
            } if( reg[x]!=')' ){ _ERROR(string::format( "regex: %d (%s)", x, nreg.data() )); }
                  NODE nNode; nNode.flg = '\2';
                       nNode.alt = compile( nreg );
                       node_push( node, str, &nNode, last );
                  continue;
            }
    
    /*─······································································─*/

            if( y == '[' ){ string_t nreg; uint k=1; x++; while( k != 0 ){
              if( reg[x]=='[' ){ k++; } if ( reg[x]==']' ){ k--; }
              if( k != 0 ){ nreg.push( reg[x] ); x++; }
            } if( reg[x]!=']' ){ _ERROR(string::format( "regex: %d [%s]", x, nreg.data() )); }
                   NODE nNode; nNode.flg  = '\1';
                        nNode.data = compile_r( nreg );
                        node_push( node, str, &nNode, last );
                   continue;
            }
    
    /*─······································································─*/

            if( y == '{' ){ x++; string_t nreg; node_push( node, str, nullptr, last ); 
            while( reg[x]!='}' ){ nreg.push( reg[x] ); x++; }
               if( reg[x]!='}' || last == nullptr )
                 { _ERROR(string::format( "regex: %d {%s}", x, nreg.data() )); }
                   last->rep = compile_n( nreg ); continue;
            }
    
    /*─······································································─*/
            
            if( y == '?' || y == '*' || y == '+' ){ node_push( node, str, nullptr, last );
            if( last == nullptr ){ _ERROR(string::format( "regex: %d %c", x, y )); }
            if( y == '?' ){ last->rep = ptr_t<int>({ 0, 1 }); }
            if( y == '*' ){ last->rep = ptr_t<int>({ 0,-1 }); }
            if( y == '+' ){ last->rep = ptr_t<int>({ 1,-1 }); } continue;
            }
    
    /*─······································································─*/

            if( y == '^' || y == '$' || y == '.' ){ 
                NODE nNode; nNode.flg = y; 
                     node_push( node, str, &nNode, last );
                continue;
            }

            if( y == '\\' ){ 
                NODE nNode; nNode.flg = reg[x+1]; 
                     node_push( node, str, &nNode, last );
                x++; continue;
            }
    
    /*─······································································─*/

        str.push( y ); } node_push( node, str, nullptr, last ); return sub;
        
    }

    ptr_t<ulong> search( string_t& _str, ulong _off, NODE* node ) const noexcept {
        if( node->flg == '\3' )
        console::log( node->alt.last()->data.data );
        return nullptr;
    }

public:

    regex_t( const string_t& reg, bool icase=false ) : i(icase)
           { queue = compile( reg ); }

    ptr_t<ulong> search( string_t _str ) const noexcept {
        for( auto &x: queue.data() ){
             auto data = search( _str, 0, &x );
        if ( data != nullptr ) return data;
        }    return  nullptr;
    }

    regex_t() noexcept = default;

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif