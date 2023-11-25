#ifndef NODEPP_SIGNAL
#define NODEPP_SIGNAL

/*────────────────────────────────────────────────────────────────────────────*/

#include <csignal>
#include "event.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {
    
    event_t<int> onSIGFPE;  //on Floating Point Exception
    event_t<int> onSIGSEGV; //on Segmentation Violation
    event_t<int> onSIGILL;  //on Illegal Instruction
    event_t<int> onSIGSINT; //on Signal Interrupt
    event_t<int> onSIGPIPE; //on Broked Pipeline
    event_t<int> onSIGTERM; //on Terminate
    event_t<int> onSIGABRT; //on Abort
    event_t<int> onSIGERR;  //on Error

    void signal_handler() {
        ::signal( SIGFPE,  []( int param ){ onSIGFPE.emit(param);  onSIGERR.emit(param); console::log("\033[1;31mSIGFPE:\033[0m Floating Point Exception"); exit(); });
        ::signal( SIGSEGV, []( int param ){ onSIGSEGV.emit(param); onSIGERR.emit(param); console::log("\033[1;31mSIGSEGV:\033[0m Segmentation Violation");  exit(); });
        ::signal( SIGILL,  []( int param ){  onSIGILL.emit(param); onSIGERR.emit(param); console::log("\033[1;31mSIGILL:\033[0m Illegal Instruction");      exit(); });
        ::signal( SIGTERM, []( int param ){ onSIGTERM.emit(param); onSIGERR.emit(param); console::log("\033[1;31mSIGTERM:\033[0m Process Terminated");      exit(); });
        ::signal( SIGINT,  []( int param ){ onSIGSINT.emit(param); onSIGERR.emit(param); console::log("\033[1;31mSIGINT:\033[0m Signal Interrupt");         exit(); });
        ::signal( SIGPIPE, []( int param ){ onSIGPIPE.emit(param); onSIGERR.emit(param); console::log("\033[1;31mSIGPIPE:\033[0m Broked Pipeline");         exit(); });
        ::signal( SIGABRT, []( int param ){ onSIGABRT.emit(param); onSIGERR.emit(param); console::log("\033[1;31mSIGABRT:\033[0m Process Abort");           exit(); });
    }

    namespace signal {
        void unignore( int signal ){ ::signal( signal, SIG_DFL ); }
        void ignore( int signal ){ ::signal( signal, SIG_IGN ); }
	    void emit( int signal ){ ::raise( signal ); }
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif