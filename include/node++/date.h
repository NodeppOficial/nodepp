#ifndef NODEPP_DATE
#define NODEPP_DATE

using DATE = struct tm;

/*────────────────────────────────────────────────────────────────────────────*/

#include <ctime>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace date {

    uint stamp(){ return (uint)( time( nullptr ) * 1000 ); }
    
    /*─······································································─*/

    uint year( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return (info->tm_year+1900); 
    }
    
    /*─······································································─*/

    uint month( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_mon; 
    }
    
    /*─······································································─*/

    uint day( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_mday; 
    }
    
    /*─······································································─*/
    
    uint hour( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_hour; 
    }
    
    /*─······································································─*/

    uint minute( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_min; 
    }
    
    /*─······································································─*/

    uint second( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_sec; 
    }
    
    /*─······································································─*/

    uint weekday( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_wday; 
    }
    
    /*─······································································─*/

    uint monthday( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_mday; 
    }
    
    /*─······································································─*/

    uint yearday( const bool& UTC=false ){
        time_t _time = time( nullptr ); DATE *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_yday; 
    }
    
    /*─······································································─*/

    string_t fulltime(){
        time_t _time = time( nullptr ); 
        string_t data(ctime(&_time));
        return data; 
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class date_t {
protected:

    DATE* info = nullptr;
    time_t _time;
    
public:

    template< class... V > date_t( V... args ) noexcept { set_time(args...); }
    
    /*─······································································─*/

    void set_time( const bool& UTC=false ) noexcept {  _time = time(nullptr); 
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
    }

    void set_time( const uint& year, const bool& UTC=false ) noexcept { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; _time = mktime(info);
    }

    void set_time( const uint& year, const uint& month, const bool& UTC=false ) noexcept { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        _time = mktime(info);
    }

    void set_time( const uint& year, const uint& month, const uint& day, const bool& UTC=false ) noexcept { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; _time = mktime(info);
    }

    void set_time( const uint& year, const uint& month, const uint& day, const uint& hour, const bool& UTC=false ) noexcept { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        _time = mktime(info);
    }

    void set_time( const uint& year, const uint& month, const uint& day, const uint& hour, const uint& min, const bool& UTC=false ) noexcept { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; _time = mktime(info);
    }

    void set_time( const uint& year, const uint& month, const uint& day, const uint& hour, const uint& min, const uint& sec, const bool& UTC=false ) noexcept { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; info->tm_sec = sec;
        _time = mktime(info);
    }
    
    /*─······································································─*/

    void set_minute( uint min ) noexcept ;  void set_second( uint sec ) noexcept ;
    void set_month( uint month ) noexcept ; void set_year( uint year ) noexcept ;
    void set_hour( uint hour ) noexcept ;   void set_day( uint day ) noexcept ;
    
    /*─······································································─*/

    string_t get_fulltime() const noexcept; uint get_weekday() const noexcept;
    uint get_monthday() const noexcept;     uint get_yearday() const noexcept;
    
    /*─······································································─*/

    uint get_minute() const noexcept; uint get_second() const noexcept; 
    uint get_stamp() const noexcept;  uint get_month() const noexcept;
    uint get_year() const noexcept;   uint get_day() const noexcept;
    uint get_hour() const noexcept;    

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

    void date_t::set_month( uint month ) noexcept { info->tm_mon = month; _time = mktime(info); }
    void date_t::set_second( uint sec )  noexcept { info->tm_sec = sec;   _time = mktime(info); }
    void date_t::set_minute( uint min )  noexcept { info->tm_min = min;   _time = mktime(info); }
    void date_t::set_year( uint year )   noexcept { info->tm_year = year; _time = mktime(info); }
    void date_t::set_hour( uint hour )   noexcept { info->tm_hour = hour; _time = mktime(info); }
    void date_t::set_day( uint day )     noexcept { info->tm_mday = day;  _time = mktime(info); }
    
    /*─······································································─*/

    string_t date_t::get_fulltime() const noexcept { return (string_t)ctime(&_time);            }
    uint     date_t::get_monthday() const noexcept { return (uint)info->tm_mday;                }
    uint     date_t::get_weekday()  const noexcept { return (uint)info->tm_wday;                }
    uint     date_t::get_yearday()  const noexcept { return (uint)info->tm_yday;                }
    uint     date_t::get_year()     const noexcept { return (uint)(info->tm_year+1900);         }
    uint     date_t::get_hour()     const noexcept { return (uint)info->tm_hour;                }
    uint     date_t::get_day()      const noexcept { return (uint)info->tm_mday;                }
    uint     date_t::get_month()    const noexcept { return (uint)info->tm_mon;                 }
    uint     date_t::get_minute()   const noexcept { return (uint)info->tm_min;                 }
    uint     date_t::get_second()   const noexcept { return (uint)info->tm_sec;                 }
    uint     date_t::get_stamp()    const noexcept { return (uint)_time * 1000;                 }

}

/*────────────────────────────────────────────────────────────────────────────*/

#endif