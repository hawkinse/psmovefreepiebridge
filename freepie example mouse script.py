def update():
	#define globals
	global lastX
	global lastY
	global cursorScale
	
	#bind left mouse to trigger button
	#Right mouse to Move button
	#Middle mouse to Square
	#Scroll up to Triangle
	#Scroll down to Circle
	mouse.leftButton = freePieIO[1].y
	mouse.rightButton = freePieIO[1].z
	mouse.middleButton = freePieIO[2].x
	
	mouse.deltaX = (freePieIO[0].x - lastX) * cursorScale
	mouse.deltaY = -1 * (freePieIO[0].y - lastY) * cursorScale
	lastX = freePieIO[0].x
	lastY = freePieIO[0].y
	
if starting:
	lastX = freePieIO[0].x
	lastY = freePieIO[0].y
	cursorScale = 10
	freePieIO[0].update += update