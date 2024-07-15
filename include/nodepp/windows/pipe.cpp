#pragma once

#include <windows.h>
#include <stdio.h>

static volatile long PipeSerialNumber;

BOOL APIENTRY CreatePipe(
    OUT LPHANDLE lpReadPipe,
    OUT LPHANDLE lpWritePipe,
    IN LPSECURITY_ATTRIBUTES lpPipeAttributes,
    IN DWORD nSize,
    DWORD dwReadMode,
    DWORD dwWriteMode
){

    if ( nSize == 0 ) { nSize = CHUNK_SIZE; }
    CHAR PipeNameBuffer[ MAX_PATH ];

    sprintf( PipeNameBuffer, "\\\\.\\Pipe\\RemoteNodepp.%08x.%08x",
        GetCurrentProcessId(), InterlockedIncrement(&PipeSerialNumber)
    );

    *lpReadPipe = CreateNamedPipeA(
        PipeNameBuffer, PIPE_ACCESS_INBOUND | dwReadMode, 
        PIPE_TYPE_BYTE, 1, nSize, nSize, 0, lpPipeAttributes
    );

    *lpWritePipe = CreateFileA(
        PipeNameBuffer, GENERIC_WRITE, 0, lpPipeAttributes,
        OPEN_EXISTING, dwWriteMode, NULL
    );

    return( TRUE );
}