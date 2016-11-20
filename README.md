# README #

PSMoveFreepieBridge is a client for [PSMoveService](https://github.com/cboulay/PSMoveService) that takes the location and rotation data of a controller and passes it into [FreePIE](https://andersmalmgren.github.io/FreePIE/).

Releases can be found on the [Downloads](https://bitbucket.org/hawkinse/psmovefreepiebridge/downloads) page.

# Usage #
1. Start PSMoveService

2. Start PSMoveFreepieBridge.

3. Enter the number of controllers you wish to track and press enter.

4. For each controller, enter the controller ID you wish to track and press Enter. This can be found using PSMoveConfigTool.

5. Open FreePIE, File>Open the included freepie example script

6. Script>Run

# VRidge Users #
* If using with Riftcat VRidge for head tracking purposes, make sure to set Tracking source to either "Phone orientation and FreeTrack position" or "FreeTrack orientation and position" in the desktop app's settings. 
* If using with a Google Cardboard headset and you can't get rotation to work at all, it is likely because your headset contains a magnet based button. Either mount your PS Move controller on the side opposing the button, or remove the magnet.

# Freepie IO mapping #

freePieIO[0] = position and rotational data

If tracking single controller:

freePieIO[1].x/y/z = Accelerometer data

freePieIO[1].pitch/roll/yaw = Gyroscope data

freePieIO[2].x/y/z = Magnetometer data

If tracking multiple controllers, each index is for a different controller.

# Release Changelog #

### Alpha 7 ###
* Added ability to set the bulb color of the tracked controllers

### Alpha 6 ###
* Fix compatibility with PSMoveService alpha 5

### Alpha 5 ###
* Add support for up to 4 controllers. Note that tracking more than one controller disables raw sensor data access.
* Compatible with PSMoveService alpha 4.2 through alpha 4.6.

### Alpha 4 ###
* Remove button support. FreePIE can already read PS Move button input using joystick[0].
* Add support for raw sensor data.
* Update example mouse control script to use joystick and gyroscope movement.
* Compatible with PSMoveService alpha 4.2 through alpha 4.6.

### Alpha 3 ###
* Add button support
* Add an example mouse control script
* Compatible with PSMoveService alpha 4.2 through alpha 4.6.

### Alpha 2 ###
* Fix 360 degree movement on yaw and pitch axis. However, only one axis can be used at a time without issues.
* Disable roll and pitch in example FreePIE script. If PSMoveFreepieBridge is being used for head tracking, yaw is the most important axis.
* Compatible with PSMoveService alpha 4.2 through alpha 4.6.

### Alpha 1 ###
* Initial Release
* Compatible with PSMoveService alpha 4.2 through alpha 4.6.