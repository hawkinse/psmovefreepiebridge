def update():
	#define globals
	global lastX
	global lastY
	global cursorScale
	
	#bind left mouse to trigger button
	#Right mouse to Move button
	#Middle mouse to Square
	mouse.leftButton = joystick[0].getDown(20)
	mouse.rightButton = joystick[0].getDown(19)
	mouse.middleButton = joystick[0].getDown(15)
	
	#Mouse movement using position data
	#mouse.deltaX = -1 * freePieIO[1].yaw#(freePieIO[2].x - lastX) * cursorScale#(freePieIO[0].x - lastX) * cursorScale
	#mouse.deltaY = -1 * freePieIO[1].pitch#-1 * (freePieIO[0].y - lastY) * cursorScale
	#lastX = freePieIO[0].x
	#lastY = freePieIO[0].y
	
	#Mouse movement using Gryoscope
	mouse.deltaX = -1 * freePieIO[1].yaw
	mouse.deltaY = -1 * freePieIO[1].pitch
	lastX = 0
	lastY = 0
	
if starting:
	lastX = freePieIO[0].x
	lastY = freePieIO[0].y
	cursorScale = 10
	freePieIO[0].update += update