#pragma once
#include "../thirdparty/headers/PSMoveService/PSMoveClient_CAPI.h"
#include <chrono>
#include <windows.h>

#define FPS_REPORT_DURATION 500 // ms

class FreepieMoveClient
{
public:
	FreepieMoveClient();
	~FreepieMoveClient();

	int run(int32_t controllerCount, PSMControllerID controllerIDs[], PSMTrackingColorType bulbColors[], int32_t freepieIndicies[], bool sendSensorData = true);

	void handle_client_psmove_event(PSMEventMessage::eEventType event_type);

	void handle_acquire_controller(PSMResult resultCode, PSMControllerID trackedControllerIndex);

	bool startup();

	void update();

	void shutdown();

private:
	bool m_keepRunning = true;
	PSMController *controller_views[4] = { nullptr, nullptr, nullptr, nullptr };
	std::chrono::milliseconds last_report_fps_timestamp;
	PSMRequestID start_stream_request_ids[4] = { -1, -1, -1, -1 };
	PSMControllerID* trackedControllerIDs;
	int32_t* trackedFreepieIndicies;
	PSMTrackingColorType* trackedBulbColors;
	int32_t trackedControllerCount = 1;
	bool m_sendSensorData = false;

	void init_controller_views();
	void free_controller_views();
};

