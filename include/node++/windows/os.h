#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <unistd.h>
#include <cerrno>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace os {
    
    string_t hostname(){
        char buffer[UNBFF_SIZE] = {0}; DWORD bufferSize = UNBFF_SIZE;
        ::GetComputerNameA(buffer,&bufferSize); return (string_t){ buffer, bufferSize };
    }
    
    /*─······································································─*/

    string_t user(){ 
        char buffer[UNBFF_SIZE] = {0}; DWORD bufferSize = UNBFF_SIZE;
        ::GetUserNameA(buffer, &bufferSize); return (string_t){ buffer, bufferSize };
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

    string_t cwd(){ 
        char buffer[UNBFF_SIZE] = {0}; 
        ::getcwd(buffer,UNBFF_SIZE); 
        return buffer; 
    }

    /*─······································································─*/

    uint pid(){ return GetCurrentProcessId(); }
    
    /*─······································································─*/

    DWORD error(){ return GetLastError(); }

    
    /*─······································································─*/

    void exit( int err=0 ){ ::exit(err); }

    /*─······································································─*/

    void abort(){ ::abort(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/