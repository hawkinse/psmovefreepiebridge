def update():
	freeTrack.pitch = -freePieIO[0].pitch * math.cos(freePieIO[0].yaw/2) - freePieIO[0].roll * math.sin(freePieIO[0].yaw/2)
	freeTrack.roll = - freePieIO[0].pitch * math.sin(freePieIO[0].yaw/2) + freePieIO[0].roll * math.cos(freePieIO[0].yaw/2)
	freeTrack.yaw = freePieIO[0].yaw
	
	#Multiply by 10 for SteamVR use
	freeTrack.x = (freePieIO[0].x * 10)
	freeTrack.y = (freePieIO[0].y * 10)
	freeTrack.z = (freePieIO[0].z * 10)

	
if starting:
	freePieIO[0].update += update