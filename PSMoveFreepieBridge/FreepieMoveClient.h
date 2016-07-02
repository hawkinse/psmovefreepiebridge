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

	int run(int32_t controllerID = 0);

	void handle_client_psmove_event(ClientPSMoveAPI::eEventType event_type);

	void handle_acquire_controller(ClientPSMoveAPI::eClientPSMoveResultCode resultCode);

	bool startup();

	void update();

	void shutdown();

private:
	bool m_keepRunning;
	ClientControllerView *controller_view;
	std::chrono::milliseconds last_report_fps_timestamp;
	ClientPSMoveAPI::t_request_id start_stream_request_id;
	int32_t trackedControllerID = 0;

	~FreepieMoveClient();
};

