//-----------------------------------------------------------------------------
// File: XbNet.cpp
//
// Desc: Shortcut macros and helper functions for the Xbox networking samples
//       Requires linking with XNET[D][S].LIB or XONLINE[D][S].LIB
//
// Hist: 09.06.01 - New for November XDK release
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "XBNet.h"




//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

// Check network link status twice/sec
const FLOAT CHECK_LINK_STATUS = 0.5f;




//-----------------------------------------------------------------------------
// Name: XBNet_Init()
// Desc: Performs all required initialization for network play
//-----------------------------------------------------------------------------
HRESULT XBNet_Init( BYTE cfgFlags )
{
    // Initialize the network stack. For default initialization, call
    // XNetStartup( NULL );
    XNetStartupParams xnsp;
    ZeroMemory( &xnsp, sizeof(xnsp) );
    xnsp.cfgSizeOfStruct = sizeof(xnsp);
    xnsp.cfgFlags = cfgFlags;
	
	xnsp.cfgPrivatePoolSizeInPages = 64; // == 256kb, default = 12 (48kb)
	xnsp.cfgEnetReceiveQueueLength = 16; // == 32kb, default = 8 (16kb)
	xnsp.cfgIpFragMaxSimultaneous = 16; // default = 4
	xnsp.cfgIpFragMaxPacketDiv256 = 32; // == 8kb, default = 8 (2kb)
	xnsp.cfgSockMaxSockets = 64; // default = 64
	xnsp.cfgSockDefaultRecvBufsizeInK = 128; // default = 16
	xnsp.cfgSockDefaultSendBufsizeInK = 128; // default = 16

    INT iResult = XNetStartup( &xnsp );
	
	DWORD TIMELIMIT = 10000;
	DWORD dwStart = GetTickCount();
	while((GetTickCount() - dwStart) < TIMELIMIT) 
	{

	}
    if( iResult != NO_ERROR )
        return E_FAIL;

    return S_OK;
}





//-----------------------------------------------------------------------------
// Name: XBNet_Cleanup()
// Desc: Performs all required cleanup for network play
//-----------------------------------------------------------------------------
HRESULT XBNet_Cleanup()
{
    // Shutdown sockets
    INT iResult = WSACleanup();
    BOOL bSuccess = ( iResult == NO_ERROR );

    // Shutdown network stack
    // Always call XNetCleanup even if WSACleanup failed
    iResult = XNetCleanup();
    bSuccess &= ( iResult == NO_ERROR );

    return( bSuccess ? S_OK : E_FAIL );
}




//-----------------------------------------------------------------------------
// Name: CXBNetLink()
// Desc: Create network link checker
//-----------------------------------------------------------------------------
CXBNetLink::CXBNetLink()
{
}




//-----------------------------------------------------------------------------
// Name: IsActive()
// Desc: Returns TRUE if network link is active; FALSE if not
//-----------------------------------------------------------------------------
BOOL CXBNetLink::IsActive() const
{
    BOOL bIsActive = TRUE;

    // For speed, only check every half second
    if( !m_LinkStatusTimer.IsRunning() ||
         m_LinkStatusTimer.GetElapsedSeconds() > CHECK_LINK_STATUS )
    {
        // Reset timer
        m_LinkStatusTimer.StartZero();

        // Poll status
        DWORD dwStatus = XNetGetEthernetLinkStatus();
        bIsActive = ( dwStatus & XNET_ETHERNET_LINK_ACTIVE ) != 0;
    }

    return bIsActive;
}
