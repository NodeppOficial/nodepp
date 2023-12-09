#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <csignal>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {
    
    event_t<int> onSIGFPE;  //on Floating Point Exception
    event_t<int> onSIGSEGV; //on Segmentation Violation
    event_t<int> onSIGILL;  //on Illegal Instruction
    event_t<int> onSIGSINT; //on Signal Interrupt
#ifdef SIGPIPE
    event_t<int> onSIGPIPE; //on Broked Pipeline
#endif
    event_t<int> onSIGTERM; //on Terminate
    event_t<int> onSIGABRT; //on Abort

    event_t<   > onSIGERR;  //on Error
    event_t<   > onSIGEXIT; //on Exit

    void signal_handler() {
        ::signal( SIGFPE,  []( int param ){ onSIGFPE.emit(param);  onSIGERR.emit(); console::log("SIGFPE:  Floating Point Exception"); exit(); });
        ::signal( SIGSEGV, []( int param ){ onSIGSEGV.emit(param); onSIGERR.emit(); console::log("SIGSEGV: Segmentation Violation");   exit(); });
        ::signal( SIGILL,  []( int param ){  onSIGILL.emit(param); onSIGERR.emit(); console::log("SIGILL:  Illegal Instruction");      exit(); });
        ::signal( SIGTERM, []( int param ){ onSIGTERM.emit(param); onSIGERR.emit(); console::log("SIGTERM: Process Terminated");       exit(); });
        ::signal( SIGINT,  []( int param ){ onSIGSINT.emit(param); onSIGERR.emit(); console::log("SIGINT:  Signal Interrupt");         exit(); });
#ifdef SIGPIPE
        ::signal( SIGPIPE, []( int param ){ onSIGPIPE.emit(param); onSIGERR.emit(); console::log("SIGPIPE: Broked Pipeline");          exit(); });
#endif
        ::signal( SIGABRT, []( int param ){ onSIGABRT.emit(param); onSIGERR.emit(); console::log("SIGABRT: Process Abort");            exit(); });
        ::atexit( /*    */ []( /*     */ ){ onSIGEXIT.emit(     ); });
    }

    namespace signal {
        void unignore( int signal ){ ::signal( signal, SIG_DFL ); }
        void ignore( int signal ){ ::signal( signal, SIG_IGN ); }
	    void emit( int signal ){ ::raise( signal ); }
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/