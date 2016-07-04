/*
* Edited from test_console_client.cpp in PSMoveService
*/

#include "stdafx.h"
#include "../thirdparty/headers/FreePIE/freepie_io.h"
#include "../thirdparty/headers/glm/glm.hpp"
#include "../thirdparty/headers/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/headers/glm/gtc/quaternion.hpp"
#include "../thirdparty/headers/glm/gtc/type_ptr.hpp"
#include "../thirdparty/headers/glm/gtx/euler_angles.hpp"
#include "FreepieMoveClient.h"

__declspec(dllexport) void WriteToFreepie(freepie_io_6dof_data data, int32_t freepieIndex = 0);

FreepieMoveClient::FreepieMoveClient()
	: m_keepRunning(true)
	, controller_view(nullptr)
	, start_stream_request_id(-1)
{
}

int FreepieMoveClient::run(int32_t controllerID)
{
	// Attempt to start and run the client
	try
	{
		trackedControllerID = controllerID;

		if (startup())
		{
			while (m_keepRunning)
			{
				update();

				Sleep(1);
			}
		}
		else
		{
			std::cerr << "Failed to startup the Freepie Move Client" << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	// Attempt to shutdown the client
	try
	{
		shutdown();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

void FreepieMoveClient::handle_client_psmove_event(ClientPSMoveAPI::eEventType event_type)
{
	switch (event_type)
	{
	case ClientPSMoveAPI::connectedToService:
		std::cout << "FreepieMoveClient - Connected to service" << std::endl;

		// Once created, updates will automatically get pushed into this view
		controller_view = ClientPSMoveAPI::allocate_controller_view(trackedControllerID);

		// Kick off request to start streaming data from the first controller
		start_stream_request_id =
			ClientPSMoveAPI::start_controller_data_stream(
				controller_view, ClientPSMoveAPI::includePositionData);
		break;
	case ClientPSMoveAPI::failedToConnectToService:
		std::cout << "FreepieMoveClient - Failed to connect to service" << std::endl;
		m_keepRunning = false;
		break;
	case ClientPSMoveAPI::disconnectedFromService:
		std::cout << "FreepieMoveClient - Disconnected from service" << std::endl;
		m_keepRunning = false;
		break;
	case ClientPSMoveAPI::opaqueServiceEvent:
		std::cout << "FreepieMoveClient - Opaque service event(%d)" << static_cast<int>(event_type) << std::endl;
		m_keepRunning = false;
		break;
	default:
		assert(0 && "Unhandled event type");
		break;
	}
}

void FreepieMoveClient::handle_acquire_controller(ClientPSMoveAPI::eClientPSMoveResultCode resultCode)
{
	if (resultCode == ClientPSMoveAPI::_clientPSMoveResultCode_ok)
	{
		std::cout << "FreepieMoveClient - Acquired controller "
			<< controller_view->GetControllerID() << std::endl;

		// Updates will now automatically get pushed into the controller view

		if (controller_view->GetControllerViewType() == ClientControllerView::PSMove)
		{
			const ClientPSMoveView &PSMoveView = controller_view->GetPSMoveView();

			if (PSMoveView.GetIsCurrentlyTracking())
			{
				PSMovePosition controller_position = PSMoveView.GetPosition();
			}
		}
	}
	else
	{
		std::cout << "FreepieMoveClient - failed to acquire controller " << std::endl;
		m_keepRunning = false;
	}
}

bool FreepieMoveClient::startup()
{
	bool success = true;

	// Attempt to connect to the server
	if (success)
	{
		if (!ClientPSMoveAPI::startup("localhost", "9512"))
		{
			std::cout << "FreepieMoveClient - Failed to initialize the client network manager" << std::endl;
			success = false;
		}
	}

	if (success)
	{
		last_report_fps_timestamp =
			std::chrono::duration_cast< std::chrono::milliseconds >(
				std::chrono::system_clock::now().time_since_epoch());
	}

	return success;
}

void FreepieMoveClient::update()
{
	// Process incoming/outgoing networking requests
	ClientPSMoveAPI::update();

	// Poll events queued up by the call to ClientPSMoveAPI::update()
	ClientPSMoveAPI::Message message;
	while (ClientPSMoveAPI::poll_next_message(&message, sizeof(message)))
	{
		switch (message.payload_type)
		{
		case ClientPSMoveAPI::_messagePayloadType_Response:
			if (start_stream_request_id != -1 &&
				message.response_data.request_id == start_stream_request_id)
			{
				handle_acquire_controller(message.response_data.result_code);
				start_stream_request_id = -1;
			}
			break;
		case ClientPSMoveAPI::_messagePayloadType_Event:
			handle_client_psmove_event(message.event_data.event_type);
			break;
		}
	}

	if (controller_view)
	{
		std::chrono::milliseconds now =
			std::chrono::duration_cast< std::chrono::milliseconds >(
				std::chrono::system_clock::now().time_since_epoch());
		std::chrono::milliseconds diff = now - last_report_fps_timestamp;

		ClientPSMoveView moveView = controller_view->GetPSMoveView();
		PSMovePose controllerPose = moveView.GetPose();

		freepie_io_6dof_data data;
		PSMoveQuaternion normalizedQuat = controllerPose.Orientation.normalize_with_default(*k_psmove_quaternion_identity);
		//glm::quat glmOrientation = glm::quat(normalizedQuat.w, normalizedQuat.x, normalizedQuat.y, normalizedQuat.z);

		data.x = controllerPose.Position.x;
		data.y = controllerPose.Position.y;
		data.z = controllerPose.Position.z;
		//data.pitch = glm::pitch(glmOrientation);
		//data.yaw = glm::yaw(glmOrientation);
		//data.roll = glm::roll(glmOrientation);

		//Calcuate rotation here, glm library doesn't work for yaw
		//Both glm and this seem to work fine when each axis is independent, but issues when combined. 
		data.yaw = std::atan2(2 * normalizedQuat.y * normalizedQuat.w - 2 * normalizedQuat.x * normalizedQuat.z, 1 - 2 * normalizedQuat.y * normalizedQuat.y - 2 * normalizedQuat.z * normalizedQuat.z);
		data.roll = std::asin(2 * normalizedQuat.x * normalizedQuat.y + 2 * normalizedQuat.z * normalizedQuat.w);
		data.pitch = std::atan2(2 * normalizedQuat.x * normalizedQuat.w - 2 * normalizedQuat.y * normalizedQuat.z, 1 - 2 * normalizedQuat.x * normalizedQuat.x - 2 * normalizedQuat.z * normalizedQuat.z);

		WriteToFreepie(data);

		if (diff.count() > FPS_REPORT_DURATION && controller_view->GetDataFrameFPS() > 0)
		{
			last_report_fps_timestamp = now;
		}
	}
}

void FreepieMoveClient::shutdown()
{
	// Free any allocated controller views
	if (controller_view)
	{
		ClientPSMoveAPI::free_controller_view(controller_view);
		controller_view = nullptr;
	}

	// Close all active network connections
	ClientPSMoveAPI::shutdown();
}

FreepieMoveClient::~FreepieMoveClient()
{
	shutdown();
}
