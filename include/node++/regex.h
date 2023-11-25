#ifndef NODEPP_REGEX
#define NODEPP_REGEX

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace _regex_ { struct str { public:

    char data = 0; bool f=0, r=0, n=0; ulong idx = 0;
    str* alt = nullptr; array_t<str*> nxt;
    ulong rep[2] = { 0, 0 };

   ~str(){ for( auto x:nxt ){ 
        if( x!=nullptr ){ delete x; x = nullptr; }
    }   if( alt!=nullptr ){ delete alt; alt = nullptr; } }

    void pipe( function_t<str*,str*> func ){
        str* n = this; while( n!=nullptr ){
            idx = 0; n = func( n );
        }
    }
};}

/*────────────────────────────────────────────────────────────────────────────*/

class regex_t { 
protected:

    bool icase=false, multi=false, dotl=false;
    ptr_t<_regex_::str> root;

    void add_new( _regex_::str*& act, _regex_::str*& prv, char data ) const noexcept {
        _regex_::str* nw = new _regex_::str; nw->data = data; 
        act->nxt.push( nw ); prv = act; act = nw;
    }

    int can_repeat( _regex_::str* NODE ) const noexcept { 
        if( !NODE->r ){ return -1; } NODE->idx++; 

             if( (long)NODE->rep[1] == 0 ){ return ( NODE->idx < NODE->rep[0] ) ? 0 :-1; } 
        else if( (long)NODE->rep[1] ==-1 ){ return ( NODE->idx < NODE->rep[0] ) ? 0 : 1; } 
        
        else {
            if( NODE->idx>=NODE->rep[0] && NODE->idx<=(NODE->rep[1]+1) ){ return  1; }
            else if( NODE->idx>=(NODE->rep[1]+1) )                      { return -1; }
            else                                                        { return  0; }
        }
        
    }

    _regex_::str* null() const noexcept { 
        _regex_::str* nw = new _regex_::str; nw->data = 1; return nw; 
    }

    void add_flag( const char& flag, _regex_::str* act ) const noexcept {
        switch( string::to_lower(flag) ){
            case 'b': act->f = 1; act->data = flag; break;
            case 'w': act->f = 1; act->data = flag; break;
            case 'd': act->f = 1; act->data = flag; break;
            case 's': act->f = 1; act->data = flag; break;
            default:              act->data = flag; break;
        }
    }

    _regex_::str* compile( string_t* _reg ) const { 
        
        _regex_::str* root = new _regex_::str;
        _regex_::str* prv = nullptr;
        _regex_::str* act = root;

        for( ulong i=0; i<_reg->size(); i++ ){ char x = (*_reg)[i];

            if( x == ']' || x == '}' || x == ')' ){
                _Error(string::format( "regex at character: %d", i ));
            }
            
            if( x == '?' || x == '*' || x == '+' ){ 
                if( prv == nullptr ){ _Error(string::format( "regex at character: %d", i )); }
                _regex_::str* nw = new _regex_::str; switch(x){
                    case '?': nw->r = 1; nw->rep[0] = 0; nw->rep[1] = 1; break;
                    case '*': nw->r = 1; nw->rep[0] = 1; nw->rep[1] =-1; break;
                    case '+': nw->r = 1; nw->rep[0] = 2; nw->rep[1] =-1; break;
                }   nw->alt = new _regex_::str; nw->alt->nxt.push(act);
                    prv->nxt[ prv->nxt.size()-1 ] = nw; 
                    act->nxt.push( null() ); act  = nw;
                continue;
            }

            if( x == '^' || x == '$' ){ add_new( act, prv, 0 ); switch(x){
                    case '^': act->f = 1; act->data = '^'; break;
                    case '$': act->f = 1; act->data = '$'; break;
                }   continue;
            }
    
    /*─······································································─*/

            if( x == '\\' || x == '.' ){ add_new( act, prv, 0 );
                if( x == '\\' ){ add_flag( (*_reg)[i+1], act ); i++; }
                if( x == '.' ){ act->f = 1; act->data = '.'; }                                       
                continue;
            }
    
    /*─······································································─*/

            if( x == '{' ){ i++; string_t s; ulong j=0; int k=0; _regex_::str* nw = new _regex_::str;
                if( prv == nullptr ){ _Error(string::format( "regex at character: %d", i )); }

                while( i<_reg->size() ){ char y = (*_reg)[i];
                    if( y == ',' ){ nw->rep[j] = string::to_ulong(s); s.clear(); i++; j++; continue; }
                    if( y == '}' ){ if( !s.empty() ){ nw->rep[j] = string::to_ulong(s); s.clear(); } k--; break; } 
                    if( !string::is_digit(y) ){ _Error(string::format( "regex at character: %d", i )); } s.push(y); i++;
                }   if( k!=-1 ){ _Error(string::format( "regex at character: %d", i )); }

                    nw->alt = new _regex_::str; nw->alt->nxt.push(act);
                    prv->nxt[ prv->nxt.size()-1 ] = nw; 
                    act->nxt.push( null() ); act  = nw;
                    act->r = 1;

                continue;
            }

            if( x == '[' ){ i++; string_t s; ulong j=0; int k=0, n=0; add_new( act, prv, 0 );

                while( i<_reg->size() ){ char y = (*_reg)[i];
                    if( j == 0 && y == '^' ){ n = true; i++; j++; continue; }
                    if( y == ']' ){ k--; break; } s.push(y); i++; j++;
                }   if( k!=-1 ){ _Error(string::format( "regex at character: %d", i )); }
                
                act->alt = new _regex_::str; for( ulong i=0; i<s.size(); i++ ){ 
                    if( s[i+1] == '-' && (i+2)<s.size() ){
                        auto a = min( s[i], s[i+2] ); 
                        auto b = max( s[i], s[i+2] );
                        for( auto j=a; j<=b; j++ ){
                            _regex_::str* nw = new _regex_::str; nw->data = j; nw->n = n;
                            act->alt->nxt.push(nw); nw->nxt.push(null());
                        }   i+=2;
                    } else {
                        _regex_::str* nw = new _regex_::str; nw->n = n;
                        if( s[i] == '\\' ){ add_flag( s[i+1], nw ); i++; }
                        else              { nw->data = s[i]; }
                        act->alt->nxt.push(nw); nw->nxt.push(null());
                    }
                }   continue;
            }

            if( x == '(' ){ i++; string_t s; add_new( act, prv, 0 ); long j=0;
                while( i<_reg->size() ){ char y = (*_reg)[i]; 
                    if( y == '(' ){ j++; } else if( y == ')' ){ j--; }
                    if( j<0 && y == ')' ){ break; } s.push(y); i++; 
                } if( j!=-1 ) { 
                    _Error(string::format( "regex at character: %d", i )); }
                if( !s.empty() ){ act->alt = compile(&s); } continue;
            }
    
    /*─······································································─*/

            if( x == '|' ){ add_new( act, prv, (char)1 ); 
                root->nxt.push( new _regex_::str );
                act = root->nxt[ root->nxt.size()-1 ];
                continue; 
            }

            add_new( act, prv, x ); 
        }   act->nxt.push(null()); 

    return root; }

    ptr_t<ulong> match( string_t _str, ulong _off, _regex_::str* NODE ) const noexcept {
        if( icase ){ _str.to_lower_case(); }
        
        ptr_t<ulong> _res ({ 0, 0 }); if( _str.empty() )return _res;

        ulong i=_off; NODE->pipe([&]( _regex_::str* NODE ){ 
            
            char data = (i>=_str.size()) ? 1 : _str[i];
            function_t<_regex_::str*,bool> end ([&]( bool e ){ 
                i++; if(e){ return NODE->nxt[0]; }
                else { _off=i; return (_regex_::str*) nullptr; }
            });

            if( NODE->data == 0 ){
                if( NODE->alt != nullptr ){ ptr_t<ulong> idx; NODE->idx=0;
                    for( auto x:NODE->alt->nxt ){ int c=1; ulong k=0; while(1){

                    if( i > _str.size() ){ return end(1); }

                    if( x->n ){

                        if( NODE->alt->nxt.size() <= k ){ i++; return NODE->nxt[0]; }
                            
                        _regex_::str* y = NODE->alt->nxt[k]; idx = match( _str, i, y );
                        bool d = ( idx[0] != idx[1] ) ? 1 : 0;

                        if( d ) return (_regex_::str*) nullptr; else { k++; continue; }

                    } else {

                        idx = match( _str, i, x ); c = can_repeat(NODE);
                        bool d = ( idx[0] != idx[1] ) ? 1 : 0;
                             i = (!d) ? i : idx[1];

                        if( c == 1 && !d ){ return NODE->nxt[0]; }
                        if( c == 0 && !d ){ break; }

                        if( c ==-1 ){
                            if(d) { return NODE->nxt[0]; }
                            else  { break; }
                        }
                    }

                    }}  return (_regex_::str*) nullptr;
                }

                else for( auto x:NODE->nxt ){ 
                    ptr_t<ulong> idx = match( _str, i, x );
                    if( idx[0] != idx[1] ){ i = idx[1]; break; }
                }

                if( i != _off ){ _res[0] = _off; _res[1] = i; 
                    return (_regex_::str*) nullptr; 
                }   return end(0); 
            }

            if( NODE->f ){
                if( NODE->data == '$' && data == 1 ){ return NODE->nxt[0]; }
                if( NODE->data == '^' && i == 0 )   { return NODE->nxt[0]; }
                switch( NODE->data ){

                    case 'b': if( data == 1 || i == 0 ){ return NODE->nxt[0]; } return end(0); break;
                    case '.': if( data == 0 || data == 1 ) { return end(0); }   break;
                    case 'B': if( data == 1 || i == 0 )    { return end(0); }   break;
                    case 's': if( !string::is_space(data) ){ return end(0); }   break;
                    case 'S': if(  string::is_space(data) ){ return end(0); }   break;
                    case 'w': if( !string::is_alnum(data) ){ return end(0); }   break;
                    case 'W': if(  string::is_alnum(data) ){ return end(0); }   break;
                    case 'd': if( !string::is_digit(data) ){ return end(0); }   break;
                    case 'D': if(  string::is_digit(data) ){ return end(0); }   break;
                    default:                                 return end(0);     break;
                    
                }   return end(1);
            }

            if( NODE->data == 1 ){ if( _off != i ){ _res[0] = _off; _res[1] = i; } return (_regex_::str*) nullptr; }
            if( data != NODE->data ){ return end(0); } return end(1);

        }); return _res;
    }
    
public:

    regex_t( string_t _reg, string_t _flags="" ) noexcept {
        for( auto x:_flags ){
            switch(x){
                case 'i': icase = true; break;
                case 'm': multi = true; break; //not available
                case 's': dotl  = true; break; //not available
            }
        }   root = compile( &_reg );
    }
    
    /*─······································································─*/

    array_t<string_t> split( string_t _str ) const noexcept { ulong n = 0;
        auto idx = search_all( _str ); array_t<string_t> result;
        if( idx.empty() ) { return result; } for ( auto x : idx ) {
            result.push( _str.slice( n, x[0] - 1 ) ); n = x[1];
        }   result.push( _str.slice( n, _str.size() ) ); return result;
    }
    
    /*─······································································─*/

    string_t replace_all( string_t _str, string_t _rep ) const noexcept {
        auto idx = search_all( _str ).reverse();
        for( auto x : idx ){
            _str.splice( x[0], x[1] - x[0], _rep );
        }   return _str;
    }
    
    /*─······································································─*/

    string_t replace( string_t _str, string_t _rep, ulong s=0 ) const noexcept {
        auto idx = search( _str, s );
        if( idx == nullptr )  { return _str; }
        if( idx[0] == idx[1] ){ return _str; }
            _str.splice( idx[0], idx[1] - idx[0], _rep ); return _str;
    }
    
    /*─······································································─*/

    array_t<ptr_t<ulong>> search_all( string_t _str ) const noexcept {
        array_t<ptr_t<ulong>> result; ulong s=0; while(1){
            auto idx = search( _str, s );
            if( idx == nullptr )  { return result; }
            if( idx[0] == idx[1] ){ return result; } s=idx[1];
                ptr_t<ulong> mem({ idx[0], idx[1] }); result.push(mem);
        }
    }
    
    /*─······································································─*/

    ptr_t<ulong> search( string_t _str, ulong s=0 ) const noexcept {
        for( ulong i=s; i<_str.size(); i++ ){
            auto idx = this->match( _str, i, &root );
            if( idx[0] != idx[1] ) return idx;
        }   return (ulong*) nullptr;
    }
    
    /*─······································································─*/

    array_t<string_t> match_all( string_t _str ) const noexcept {
        auto idx = search_all( _str ); array_t<string_t> result;
        for( auto x : idx ){
            result.push(_str.slice( x[0], x[1] - 1 ));
        }   return result;
    }
    
    /*─······································································─*/

    string_t match( string_t _str, ulong s=0 ) const noexcept { 
        auto idx = search( _str, s );
        if( idx == nullptr )  { return ""; }
        if( idx[0] == idx[1] ){ return ""; }
            return _str.slice( idx[0], idx[1] - 1 );
    }
    
    /*─······································································─*/

    bool test( string_t _str, ulong s=0 ) const noexcept {
        auto idx = search( _str, s );
        if( idx == nullptr )  { return 0; }
        if( idx[0] == idx[1] ){ return 0; }
                                return 1;
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace regex {

    string_t replace_all( string_t _str, string_t _reg, string_t _rep, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.replace_all( _str, _rep );
    }
    
    /*─······································································─*/

    array_t<ptr_t<ulong>> search_all( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.search_all( _str );
    }
    
    /*─······································································─*/

    string_t replace( string_t _str, string_t _reg, string_t _rep, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.replace( _str, _rep );
    }
    
    /*─······································································─*/

    array_t<string_t> match_all( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.match_all( _str );
    }
    
    /*─······································································─*/

    ptr_t<ulong> search( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.search( _str );
    }
    
    /*─······································································─*/

    string_t match( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.match( _str );
    }

    /*─······································································─*/

    bool test( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.test( _str );
    }

    /*─······································································─*/

    array_t<string_t> split( string_t _str, char ch ){ return string::split( _str, ch ); }

    array_t<string_t> split( string_t _str, int ch ){ return string::split( _str, ch ); }

    array_t<string_t> split( const string_t& _str, const string_t& _reg ){ 
        if( _reg.size() == 1 ){  return string::split( _str, _reg[0] ); }
        else if( _reg.empty() ){ return string::split( _str, 1 ); }
        regex_t reg(_reg);       return reg.split( _str ); 
    }

    /*─······································································─*/

    template< class T, class... V > string_t join( string_t c, const T& argc, const V&... args ){ 
        return string::join( c, argc, args... ); 
    }
    
    /*─······································································─*/

    template< class V, class... T > string_t format( const V& val, const T&... args ){
        string_t result = string::to_string(val); ulong n=0; string::map([&]( string_t arg ){
            string_t reg = "\\$\\{" + string::to_string(n) + "\\}"; 
            result = replace_all(result,reg,arg); n++;
        },  args... ); return result;
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif