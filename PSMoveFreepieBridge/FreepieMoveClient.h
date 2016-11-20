#pragma once
#include "../thirdparty/headers/PSMoveService/ClientPSMoveAPI.h"
#include "../thirdparty/headers/PSMoveService/ClientControllerView.h"
#include <chrono>
#include <windows.h>

#define FPS_REPORT_DURATION 500 // ms

class FreepieMoveClient
{
public:
	FreepieMoveClient();
	~FreepieMoveClient();

	int run(int32_t controllerCount, int32_t controllerIDs[], int32_t bulbColors[], int32_t freepieIndicies[], bool sendSensorData = true);

	void handle_client_psmove_event(ClientPSMoveAPI::eEventType event_type);

	void handle_acquire_controller(ClientPSMoveAPI::eClientPSMoveResultCode resultCode, int32_t trackedControllerIndex);

	bool startup();

	void update();

	void shutdown();

private:
	bool m_keepRunning = true;
	ClientControllerView *controller_views[4] = { nullptr, nullptr, nullptr, nullptr };
	std::chrono::milliseconds last_report_fps_timestamp;
	ClientPSMoveAPI::t_request_id start_stream_request_ids[4] = { -1, -1, -1, -1 };
	int32_t* trackedControllerIDs;
	int32_t* trackedFreepieIndicies;
	int32_t* trackedBulbColors;
	int32_t trackedControllerCount = 1;
	bool m_sendSensorData = false;
};

