#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace os {
    
    string_t hostname(){
        char buffer[UNBFF_SIZE]; DWORD bufferSize = UNBFF_SIZE;
        GetComputerNameA(buffer,&bufferSize); return (string_t){ buffer, bufferSize };
    }
    
    /*─······································································─*/

    string_t user(){ 
        char buffer[UNBFF_SIZE]; DWORD bufferSize = UNBFF_SIZE;
        GetUserNameA(buffer, &bufferSize); return (string_t){ buffer, bufferSize };
    }
    
    /*─······································································─*/

    string_t cwd(){ char buffer[ UNBFF_SIZE ];
        DWORD length = GetCurrentDirectory( UNBFF_SIZE, buffer );
        return (string_t){ buffer, length };
    }
    
    /*─······································································─*/

    uint cpus(){ 
        SYSTEM_INFO sysInfo; GetSystemInfo(&sysInfo);
        return sysInfo.dwNumberOfProcessors;
    }
    
    /*─······································································─*/

    string_t tmp(){ string_t tmp (MAX_PATH);
        GetTempPath( MAX_PATH, tmp.data() );
        return tmp;
    }

    /*─······································································─*/

    uint pid(){ return GetCurrentProcessId(); }
    
    /*─······································································─*/

    DWORD error(){ return GetLastError(); }

    
    /*─······································································─*/

    void exit( int err=0 ){ ::exit(err); }

}}

/*────────────────────────────────────────────────────────────────────────────*/