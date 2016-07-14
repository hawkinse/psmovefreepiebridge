# README #

PSMoveFreepieBridge is a client for [PSMoveService](https://github.com/cboulay/PSMoveService) that takes the location, rotation and button data of a controller and passes it into [FreePIE](https://andersmalmgren.github.io/FreePIE/).

Releases can be found on the [Downloads](https://bitbucket.org/hawkinse/psmovefreepiebridge/downloads) page.

# Usage #
1. Start PSMoveService

2. Start PSMoveFreepieBridge.

3. Enter the controller ID you wish to track and press Enter. This can be found using PSMoveConfigTool.

4. Open FreePIE, File>Open the included freepie example script

5. Script>Run

# VRidge Users #
* If using with Riftcat VRidge for head tracking purposes, make sure to set Tracking source to either "Phone orientation and FreeTrack position" or "FreeTrack orientation and position" in the desktop app's settings. 
* If using with a Google Cardboard headset and you can't get rotation to work at all, it is likely because your headset contains a magnet based button. Either mount your PS Move controller on the side opposing the button, or remove the magnet.
* There will be a controller that occasionally floats in front of you or clips through your head. This is because all PSMoveService clients are capable of seeing all controllers, so Steam VR still sees the head tracking controller. There is nothing I can do about this on the PSMoveFreepieBridge side.

# Freepie IO mapping #

freePieIO[0] = position and rotational data

freePieIO[1].x = Trigger analog value

freePieIO[1].y = Trigger button

freePieIO[1].z = Move button

freePieIO[1].yaw = PS button

freePieIO[1].pitch = Start button

freePieIO[1].roll = Select button

freePieIO[2].x = Square button

freePieIO[2].y = Cross button

freePieIO[2].z = Triangle button

freePieIO[2].yaw = Circle button


# Release Changelog #

### Alpha 3 ###
* Add button support
* Add an example mouse control script

### Alpha 2 ###
* Fix 360 degree movement on yaw and pitch axis. However, only one axis can be used at a time without issues.
* Disable roll and pitch in example FreePIE script. If PSMoveFreepieBridge is being used for head tracking, yaw is the most important axis.
* Compatible with PSMoveService alpha 4.2 and up.

### Alpha 1 ###
* Initial Release
* Compatible with PSMoveService alpha 4.2 and up.