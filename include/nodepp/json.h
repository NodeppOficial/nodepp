/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_JSON
#define NODEPP_JSON

/*────────────────────────────────────────────────────────────────────────────*/

#include "object.h"
#include "regex.h"
#include "map.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class json_t {
protected:

    long get_next_sec( ulong _pos, const string_t& str ) const noexcept {
        uchar k=0; while( _pos < str.size() ){     
            switch( str[_pos] ){
                case ':': k += 6; break; case ',': k -= 6; break;
                case '{': _pos =get_next_key( _pos, str ); break;
                case '[': _pos =get_next_key( _pos, str ); break;
                case '"': _pos =get_next_key( _pos, str ); break;
                case '\\':_pos =get_next_key( _pos, str ); break;
            }
            if( k == 0 ){ break; } _pos++;
        }   return _pos >= str.size() ? -1 : _pos;
    }

    long get_next_key( ulong _pos, const string_t& str ) const noexcept {
        bool x=1; uchar k=0; while( _pos < str.size() ){   
            switch( str[_pos] ){
                case '[': k += 1; break; case ']': k -= 1; break; 
                case '{': k += 3; break; case '}': k -= 3; break;
                case '\\':_pos++; break; case '"':
                    if( x ){ k+=5; x=!x; }
                    else   { k-=5; x=!x; }
                break; 
            }
            if( k == 0 ){ break; } _pos++;
        }   return _pos >= str.size() ? -1 : _pos;
    }

    object_t get_data( const string_t& data ) const noexcept {
        ulong x=0; while( x < data.size() && data[x]==' ' ){ x++; }
          if( data.empty() || data[x] == ',' )       { return nullptr; }
        elif( data[x] == '"'     )                   { return regex::match(data,"\"[^\"]+\"").slice(1,-1); }
        elif( data[x] == '{'     )                   { return parse( data ); }
        elif( data[x] == '['     )                   { return parse( data ); }
        elif( data.find("false") )                   { return (bool) 0; }
        elif( data.find("true")  )                   { return (bool) 1; }
        elif( data.find("null")  )                   { return nullptr;  }
        elif( regex::test(data,"[a-z]") )            { return (string_t) data; }
        elif( data.find('.')     ){
            if( regex::match(data,".\\d+").size()>5 ){ return string::to_double(data); }
            else                                     { return string::to_float(data);  }
        }   elif( data.size() > 9 )                  { return string::to_long(data);   }
            else                                     { return string::to_int(data);    } 
    }

    object_t get_object( ulong x, ulong y, const string_t& str ) const {
        object_t result; do { type::pair<string_t,string_t> data;
           if( string::is_space(str[x]) ){ continue; }
           if( str[x] == '"' ){
               auto z = get_next_sec( x, str ); 
               data.first = str.slice( x+1,z );
            while( str[x]!=':' && x<y ){ x++; }
               auto w = get_next_sec( x, str ); 
                    w = w<0 ? str.size()-1 : w;
               data.second = str.slice( x+1, w ); x=w;
               result[data.first] = get_data( data.second );
            }
        } while( x++<y ); return result.keys().empty() ? nullptr : result;
    }

    array_t<object_t> get_array( ulong x, ulong y, const string_t& str ) const {
        array_t<object_t> data; do {
           if( string::is_space(str[x]) || str[x]==',' ){ continue; }
           if( str[x] == '{' || str[x] == '[' ){
               auto z = get_next_key( x, str ); 
           if( z < 0 ){ process::error("Invalid JSON Format"); }
               data.push( parse(str.slice( x,z+1 )) ); x=z+1; 
           } elif( str[x] == '"' ) { 
               auto z = get_next_sec( x, str ); 
           if( z < 0 ){ process::error("Invalid JSON Format"); }
               data.push( get_data(str.slice( x,z+1 )) ); x=z+1;
           } elif( x != y ) { 
               ulong z=x; while( str[z]!=',' && z<y ){ z++; }
               data.push( get_data(str.slice( x, z )) ); x=z;
           }
        } while( x++<y ); return data;
    }
    
public: json_t () noexcept = default;

    object_t parse( const string_t& str ) const {
        if( str.empty() ){ return nullptr; }
        ulong x = 0; string_t data; /*process::next();*/ do {

            if ( str[x] == '[' || str[x] == '{' || str[x] == '"' ){
                 auto pos = get_next_key( x, str );
            if ( pos < 0 ){ process::error("Invalid JSON Format"); }

                if( str[x] == '[' ) {
                    return get_array( x+1, pos, str );
                } elif( str[x] == '{' ) {
                    return get_object( x+1,pos, str );
                } else {
                    data = str.slice( x+1, pos-1 ); break;
                }   x = pos + 1;               

            } elif( str[x] == ']' || str[x] == '}' || str[x] == ')' ){ 
                process::error("Invalid JSON Format"); 
            } else {
                if( string::is_space( str[x] ) )
                  { continue; } data.push( str[x] );
            }

        } while ( x++<str.size() ); return get_data(data);
    }

    string_t stringify( const object_t& obj ) const { 
        if( !obj.has_value() ){ return nullptr; }
        string_t result; /*process::next();*/

        if( obj.get_type_id() == 20 ){
            result.push('{');

            for( auto &item: obj.as<queue_t<type::pair<string_t,object_t>>>().data() ){
                 result += string::format("\"%s\":",item.first.get());
                 result += stringify( item.second ); result.push(',');
            }    result.pop();

            result.push('}'); goto END;
        } elif( obj.get_type_id() == 21 ){
            result.push('[');

            for( auto &item: obj.as<array_t<object_t>>() )
               { result += stringify( item ); result.push(','); }
            if ( result[ result.size()-1 ] == ',' ){ result.pop(); }

            result.push(']'); goto END;
        }

        switch( obj.get_type_id() ){

            case 0x0001: return string::to_string( obj.as<int>() );                              break;
            case 0x0002: return string::to_string( obj.as<uint>() );                             break;
            case 0x0003: return obj.as<bool>() ? "true" : "false";                               break;
            case 0x0004: return string::format("\"%c\"",obj.as<char>());                         break;
            case 0x0005: return string::to_string( obj.as<long>() );                             break;
            case 0x0006: return string::to_string( obj.as<short>() );                            break;
            case 0x0007: return string::to_string( obj.as<uchar>() );                            break;
            case 0x0008: return string::to_string( obj.as<llong>() );                            break;
            case 0x0009: return string::to_string( obj.as<ulong>() );                            break;
            case 0x000a: return string::to_string( obj.as<ushort>() );                           break;
            case 0x000b: return string::to_string( obj.as<ullong>() );                           break;
            case 0x000c: return string::to_string( obj.as<wchar_t>() );                          break;
            case 0x000d: return string::to_string( obj.as<char16_t>() );                         break;
            case 0x000e: return string::to_string( obj.as<char32_t>() );                         break;
            case 0x000f: return string::to_string( obj.as<float>() );                            break;
            case 0x0010: return string::to_string( obj.as<double>() );                           break;
            case 0x0011: return string::to_string( obj.as<ldouble>() );                          break;
            case 0x0012: return string::format("\"%s\"",obj.as<string_t>().get());               break;

            case 0xfA03: do { result.push('[');
             for( auto &x: obj.as<array_t<bool>>() )
                { result += string::format("\"%s\",",x ? "true":"false" ); }   
             if ( result[ result.size()-1 ] == ',' ){ result.pop(); } 
            result.push(']'); } while(0); break;

            case 0xfA04: do { result.push('[');
             for( auto &x: obj.as<array_t<char>>() )
                { result += string::format("\"%c\",", x ); }
             if ( result[ result.size()-1 ] == ',' ){ result.pop(); } 
            result.push(']'); } while(0); break;
            
            case 0xfA01: return string::format("[%s]",obj.as<array_t<int>>().join().get());      break;
            case 0xfA02: return string::format("[%s]",obj.as<array_t<uint>>().join().get());     break;
            case 0xfA05: return string::format("[%s]",obj.as<array_t<long>>().join().get());     break;
            case 0xfA06: return string::format("[%s]",obj.as<array_t<short>>().join().get());    break;
            case 0xfA07: return string::format("[%s]",obj.as<array_t<uchar>>().join().get());    break;
            case 0xfA08: return string::format("[%s]",obj.as<array_t<llong>>().join().get());    break;
            case 0xfA09: return string::format("[%s]",obj.as<array_t<ulong>>().join().get());    break;
            case 0xfA0a: return string::format("[%s]",obj.as<array_t<ushort>>().join().get());   break;
            case 0xfA0b: return string::format("[%s]",obj.as<array_t<ullong>>().join().get());   break;
            case 0xfA0c: return string::format("[%s]",obj.as<array_t<wchar_t>>().join().get());  break;
            case 0xfA0d: return string::format("[%s]",obj.as<array_t<char16_t>>().join().get()); break;
            case 0xfA0e: return string::format("[%s]",obj.as<array_t<char32_t>>().join().get()); break;
            case 0xfA0f: return string::format("[%s]",obj.as<array_t<float>>().join().get());    break;
            case 0xfA10: return string::format("[%s]",obj.as<array_t<double>>().join().get());   break;
            case 0xfA11: return string::format("[%s]",obj.as<array_t<ldouble>>().join().get());  break;
            case 0xfA12: return string::format("[%s]",obj.as<array_t<string_t>>().join().get()); break; 

            default: return "{}"; break;
        }

        END:; return result;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace json {
    object_t     parse( const string_t& str ){ json_t json; return json.parse( str );     }
    string_t stringify( const object_t& obj ){ json_t json; return json.stringify( obj ); }
}} 

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace json {

    template<class T, class V>
    object_t parse( const map_t<T,V>& map ){
        object_t obj; for( auto &x: map.keys() )
          { obj[x] = map[x]; } return obj;
    }

    template<class T, class V>
    string_t stringify( const map_t<T,V>& map ){
        object_t obj; for( auto &x: map.data() ){
            obj[ x.first ] = x.second;
        }   return stringify( obj );
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace json {

    template<class T, class V>
    array_t<object_t> parse( const array_t<map_t<T,V>>& map ){
        array_t<object_t> obj; for( auto &x: map )
          { obj.push( parse(x) ); } return obj;
    }

    template<class T, class V>
    string_t stringify( const array_t<map_t<T,V>>& map ){
        auto obj = json::parse( map );
        json::stringify( obj );
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
