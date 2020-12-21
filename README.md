# DashSelector
Simple (but powerfull :wink:) dashboard selector for Original XBOX.

With this tool you can boot multiple dashboards depending on keypad button being pressed.
In addition to that - it will patch XBOX kernel to support mounting ISO files. The following 
kernels are supported: 3944, 4034, 4817, 5101, 5530, 5713, 5838 and that means most popular BIOSes
are supprted (incuding IND, M8Plus and X2). The tool detects the BIOS and only applies patch if BIOS
is supported (no harm wil be done to other BIOSes).
It has an emergency mechanism that let's you upload and boot any XBE (or dashboard) if DashSelector 
encounters a problem (eg. missing config file or missing dashboard).

A quick overview is available here: https://youtu.be/UUz7UD19Xrk

Quick start:
- copy DashSelector.xbe to XBOX C: drive and rename to whatever your BIOS boots (eg. evoxdash.xbe)
- edit the ini file to your liking (ie. whatever dashboards you have) and copy to C: drive
- enjoy

Disclaimer: this tool is inspired by Rocky5's DashLoader - which is part of Rocky5's SoftMod tool. 
For most people Rocky5's DashLoder will work (and is probably better choice). 
I prefer my DashSelector as it gives me the following:
- shorter boot time (ie. boots dashboard directly from the patcher)
- support for more BIOSes
- can define LED for each Dashboard
- has emergency http server

