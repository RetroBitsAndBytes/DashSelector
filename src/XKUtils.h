/*
**********************************
**********************************
**      BROUGHT TO YOU BY:		**
**********************************
**********************************
**								**
**		  [TEAM ASSEMBLY]		**
**								**
**		www.team-assembly.com	**
**								**
******************************************************************************************************
* This is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************************************


********************************************************************************************************
**	     XKUTILS.H - XBOX Utility Class' Header
********************************************************************************************************
**
**	This is the Class Header, see the .CPP file for more comments and implementation details.
**
********************************************************************************************************

********************************************************************************************************
**	CREDITS:
********************************************************************************************************
**	XBOX-LINUX TEAM:
**  ---------------
**		Wow, you guys are awsome !!  I bow down to your greatness !!  
**		REFERENCE URL:  http://xbox-linux.sourceforge.net
**
********************************************************************************************************

UPDATE LOG:
--------------------------------------------------------------------------------------------------------
Date: 02/18/2003
By: UNDEAD [team-assembly]
Reason: Prepared 0.2 for Public Release
--------------------------------------------------------------------------------------------------------
Date: 10/29/2020
By: _AA_ 
Reason: added PLAUNCH_DATA to LaunchXBE, some LEDs
--------------------------------------------------------------------------------------------------------
*/
#pragma once
#if defined (_XBOX)
	//This complete file is only supported for XBOX..
	#pragma message ("Compiling for XBOX: " __FILE__)


#include <xtl.h>
#include "XKExports.h"

class XKUtils  
{
public:

	//Enum for Devices on SMBus
	enum SMBUS_DEVICES
	{
		SMBDEV_PIC16L = 0x20,
		SMBDEV_VIDEO_ENCODER = 0x8a,
		SMBDEV_VIDEO_ENCODER_CONNEXANT = 0x8a,
		SMBDEV_VIDEO_ENCODER_FOCUS = 0xd4,
		SMBDEV_VIDEO_ENCODER_XCALIBUR = 0xe0,
		SMBDEV_TEMP_MONITOR = 0x98,
		SMBDEV_EEPROM = 0xA8

	};

	//Commands that can be sent to the PIC
	enum PIC16L_CMD
	{
		PIC16L_CMD_POWER = 0x02,
		PIC16L_CMD_DVDTRAPSTATE = 0x03,
		PIC16L_CMD_LED_MODE = 0x07,
		PIC16L_CMD_LED_REGISTER = 0x08,
		PIC16L_CMD_EJECT = 0x0C,
		PIC16L_CMD_INTERRUPT_REASON = 0x11,
		PIC16L_CMD_RESET_ON_EJECT = 0x19,
		PIC16L_CMD_SCRATCH_REGISTER = 0x1B
	};

	//Commands that can be sent to the Video Encoder
	enum VIDEO_ENCODER_CMD
	{
		VIDEO_ENCODER_CMD_DETECT = 0x00
	};

	//Values For Video Encoder
	enum VIDEO_ENCODER_TYPE
	{
		VIDEO_ENCODER_UNKNOWN = 0,
		VIDEO_ENCODER_CONNEXANT = 1,
		VIDEO_ENCODER_FOCUS = 2,
		VIDEO_ENCODER_XCALIBUR = 3
	};

	//Sub commands for the PIC Power command
	enum POWER_SUBCMD
	{
		POWER_SUBCMD_RESET = 0x01,
		POWER_SUBCMD_CYCLE = 0x40,
		POWER_SUBCMD_POWER_OFF = 0x80
	
	};

	//Sub Command for the PIC LED command
	enum LED_MODE_SUBCMD
	{
		LED_MODE_SUBCMD_DEFAULT_MODE = 0x00,
		LED_MODE_SUBCMD_CUSTOM_MODE = 0x01
	};

	//Sub Command for the PIC LED command
	enum LED_REGISTER_CYCLE
	{
		LED_REGISTER_CYCLE3_GREEN	= 0x01,
		LED_REGISTER_CYCLE2_GREEN	= 0x02,
		LED_REGISTER_CYCLE1_GREEN	= 0x04, 
		LED_REGISTER_CYCLE0_GREEN	= 0x08,
		LED_REGISTER_CYCLE3_RED		= 0x10,
		LED_REGISTER_CYCLE2_RED		= 0x20,
		LED_REGISTER_CYCLE1_RED		= 0x40, 
		LED_REGISTER_CYCLE0_RED		= 0x80,

		LED_RED						= 0x10 | 0x20 | 0x40 | 0x80,
		LED_GREEN					= 0x01 | 0x02 | 0x04 | 0x08,
		LED_ORANGE					= 0x10 | 0x20 | 0x40 | 0x80 | 0x1 | 0x2 | 0x4 | 0x8,
		LED_FAST_RED				= 0x80 | 0x20,
		LED_FAST_GREEN				= 0x08 | 0x02,
		LED_FAST_ORANGE				= LED_FAST_RED | LED_FAST_GREEN,
		LED_SLOW_RED				= 0x80 | 0x40,
		LED_SLOW_GREEN				= 0x08 | 0x04,
		LED_SLOW_ORANGE				= LED_SLOW_RED | LED_SLOW_GREEN,

	};


	//Sub commands for the PIC EJECT command
	enum EJECT_SUBCMD
	{
		EJECT_SUBCMD_EJECT = 0x00,
		EJECT_SUBCMD_LOAD = 0x01
	};	

	//REason for interrupt
	enum INTERRUPT_REASON_BITVALUE
	{
		INTERRUPT_REASON_BITVALUE_POWER_BUTTON = 0x01,
		INTERRUPT_REASON_BITVALUE_AV_REMOVED = 0x10,
		INTERRUPT_REASON_BITVALUE_EJECT_BUTTON = 0x20
	};

	//subcommands for Reset on Eject PIC command
	enum RESET_ON_EJECT_SUBCMD
	{
		RESET_ON_EJECT_SUBCMD_ENABLE = 0x00,
		RESET_ON_EJECT_SUBCMD_DISABLE = 0x01

	};

	//Scratch register values..
	enum SCRATCH_REGISTER_BITVALUE
	{
		SCRATCH_REGISTER_BITVALUE_EJECT_AFTER_BOOT = 0x01,
		SCRATCH_REGISTER_BITVALUE_DISPLAY_ERROR = 0x02,
		SCRATCH_REGISTER_BITVALUE_NO_ANIMATION = 0x04,
		SCRATCH_REGISTER_BITVALUE_RUN_DASHBOARD = 0x08
	};

	XKUtils();
	virtual ~XKUtils();

	static void	DVDEjectTray();
	static void	DVDLoadTray();
	static void	DVDDisableEjectReset();
	static void	DVDEnableEjectReset();
	static LONG	MountDiskA();
	static LONG	MountDiskC();
	static LONG	MountDiskD();
	static LONG	MountDiskE();
	static LONG	MountDiskF();
	static LONG	MountDiskG();
	static LONG	MountDiskH();
	static LONG	MountDiskX();
	static LONG	MountDiskY();
	static LONG	MountDiskZ();
	static LONG	MountDiskS();
	static void	MountAllDisks();
	static LONG	UnMountDiskA();
	static LONG	UnMountDiskC();
	static LONG	UnMountDiskD();
	static LONG	UnMountDiskE();
	static LONG	UnMountDiskF();
	static LONG	UnMountDiskG();
	static LONG	UnMountDiskH();
	static LONG	UnMountDiskX();
	static LONG	UnMountDiskY();
	static LONG	UnMountDiskZ();
	static LONG	UnMountDiskS();
	static void	UnMountAllDisks();

	static LONG	MountDevice(LPSTR sSymbolicLinkName, LPSTR sDeviceName);
	static LONG	UnMountDevice(LPSTR sSymbolicLinkName);
	
	static void	WriteEEPROMToXBOX(LPBYTE EEPROMDATA, UCHAR STARTPOS, UCHAR ENDPOS);
	static void	ReadEEPROMFromXBOX(LPBYTE EEPROMDATA, UCHAR STARTPOS, UCHAR ENDPOS);
    static void GetMODCHIPVersion();
	static void GetXBOXVersion();
	static void	XBOXRebootToDash();
	static void	XBOXReset();
	static void	XBOXPowerOff();
	static void	XBOXPowerCycle();
	static void	XBOXCPUTemp(int *);
	static void	XBOXSYSTemp(int *);
	static void	SetXBOXLEDStatus(UCHAR LEDStatus);

	static int	GetFanSpeed(void);
	static void	SetFanSpeed(int);

	static int GetVideoEncoderType(void);

	static bool GetCPUFrequence(double *);

	static void LaunchXBE(char *, PLAUNCH_DATA);

private:
	
	static double RDTSC(void);

};
#endif
