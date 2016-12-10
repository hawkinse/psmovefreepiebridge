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
{
}

int FreepieMoveClient::run(int32_t controllerCount, int32_t controllerIDs[], int32_t bulbColors[], int32_t freepieIndicies[], bool sendSensorData)
{
	// Attempt to start and run the client
	try
	{
		trackedControllerIDs = controllerIDs;
		trackedFreepieIndicies = freepieIndicies;
		trackedBulbColors = bulbColors;
		m_sendSensorData = sendSensorData;
		trackedControllerCount = controllerCount;

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
		for (int i = 0; i < trackedControllerCount; i++)
		{
			controller_views[i] = ClientPSMoveAPI::allocate_controller_view(trackedControllerIDs[i]);

			// Kick off request to start streaming data from the first controller
			start_stream_request_ids[i] =
				ClientPSMoveAPI::start_controller_data_stream(
					controller_views[i], (m_sendSensorData ? ClientPSMoveAPI::includePositionData | ClientPSMoveAPI::includeRawSensorData : ClientPSMoveAPI::includePositionData));

			//Set bulb color if specified
			if ((trackedBulbColors[i] >= 0) && (trackedBulbColors[i] < PSMoveTrackingColorType::MAX_PSMOVE_COLOR_TYPES)) {
				ClientPSMoveAPI::set_led_tracking_color(controller_views[i], (PSMoveTrackingColorType)trackedBulbColors[i]);
			}
		}
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

void FreepieMoveClient::handle_acquire_controller(ClientPSMoveAPI::eClientPSMoveResultCode resultCode, int32_t trackedControllerIndex)
{
	if (resultCode == ClientPSMoveAPI::_clientPSMoveResultCode_ok)
	{
		std::cout << "FreepieMoveClient - Acquired controller "
			<< controller_views[trackedControllerIndex]->GetControllerID() << std::endl;
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
			for (int i = 0; i < trackedControllerCount; i++)
			{
				if (start_stream_request_ids[i] != -1 &&
					message.response_data.request_id == start_stream_request_ids[i])
				{
					handle_acquire_controller(message.response_data.result_code, i);
					start_stream_request_ids[i] = -1;
				}
			}
			break;
		case ClientPSMoveAPI::_messagePayloadType_Event:
			handle_client_psmove_event(message.event_data.event_type);
			break;
		}
	}

	//Button data must be outside of loop because it contains data for all tracked controllers!
	freepie_io_6dof_data buttonData;

	for (int i = 0; i < trackedControllerCount; i++)
	{
		if (controller_views[i] && controller_views[i]->IsValid() && controller_views[i]->GetControllerViewType() == ClientControllerView::PSMove)
		{
			std::chrono::milliseconds now =
				std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now().time_since_epoch());
			std::chrono::milliseconds diff = now - last_report_fps_timestamp;

			ClientPSMoveView moveView = controller_views[i]->GetPSMoveView();
			PSMovePose controllerPose = moveView.GetPose();

			freepie_io_6dof_data poseData;
			PSMoveQuaternion normalizedQuat = controllerPose.Orientation.normalize_with_default(*k_psmove_quaternion_identity);
			//glm::quat glmOrientation = glm::quat(normalizedQuat.w, normalizedQuat.x, normalizedQuat.y, normalizedQuat.z);

			poseData.x = controllerPose.Position.x;
			poseData.y = controllerPose.Position.y;
			poseData.z = controllerPose.Position.z;

			//data.pitch = glm::pitch(glmOrientation);
			//data.yaw = glm::yaw(glmOrientation);
			//data.roll = glm::roll(glmOrientation);

			//Calcuate rotation here, glm library doesn't work for yaw
			//Both glm and this seem to work fine when each axis is independent, but issues when combined. 
			poseData.yaw = std::atan2(2 * normalizedQuat.y * normalizedQuat.w - 2 * normalizedQuat.x * normalizedQuat.z, 1 - 2 * normalizedQuat.y * normalizedQuat.y - 2 * normalizedQuat.z * normalizedQuat.z);
			poseData.roll = std::asin(2 * normalizedQuat.x * normalizedQuat.y + 2 * normalizedQuat.z * normalizedQuat.w);
			poseData.pitch = std::atan2(2 * normalizedQuat.x * normalizedQuat.w - 2 * normalizedQuat.y * normalizedQuat.z, 1 - 2 * normalizedQuat.x * normalizedQuat.x - 2 * normalizedQuat.z * normalizedQuat.z);

			WriteToFreepie(poseData, trackedFreepieIndicies[i]);

			if (m_sendSensorData)
			{
				PSMoveRawSensorData sensors = moveView.GetRawSensorData();

				//Send sensor data through pos/rot struct
				freepie_io_6dof_data sensorData1;
				sensorData1.x = sensors.Accelerometer.i;
				sensorData1.y = sensors.Accelerometer.j;
				sensorData1.z = sensors.Accelerometer.k;

				sensorData1.pitch = sensors.Gyroscope.i;
				sensorData1.roll = sensors.Gyroscope.j;
				sensorData1.yaw = sensors.Gyroscope.k;
				WriteToFreepie(sensorData1, 1);

				freepie_io_6dof_data sensorData2;
				sensorData2.x = (float)sensors.Magnetometer.i;
				sensorData2.y = (float)sensors.Magnetometer.j;
				sensorData2.z = (float)sensors.Magnetometer.k;

				WriteToFreepie(sensorData2, 2);
			}

			// If we have less than four controllers, also include button data
			if (trackedControllerCount < 4)
			{
				float triggerState = moveView.GetTriggerValue();
				uint8_t buttonsPressed = 0;

				buttonsPressed |= (moveView.GetButtonSquare() == PSMoveButtonState::PSMoveButton_DOWN);
				buttonsPressed |= ((moveView.GetButtonTriangle() == PSMoveButtonState::PSMoveButton_DOWN) << 1);
				buttonsPressed |= ((moveView.GetButtonCross() == PSMoveButtonState::PSMoveButton_DOWN) << 2);
				buttonsPressed |= ((moveView.GetButtonCircle() == PSMoveButtonState::PSMoveButton_DOWN) << 3);
				buttonsPressed |= ((moveView.GetButtonMove() == PSMoveButtonState::PSMoveButton_DOWN) << 4);
				buttonsPressed |= ((moveView.GetButtonPS() == PSMoveButtonState::PSMoveButton_DOWN) << 5);
				buttonsPressed |= ((moveView.GetButtonStart() == PSMoveButtonState::PSMoveButton_DOWN) << 6);
				buttonsPressed |= ((moveView.GetButtonSelect() == PSMoveButtonState::PSMoveButton_DOWN) << 7);

				switch (i)
				{
					case 0:
						buttonData.x = buttonsPressed;
						buttonData.yaw = triggerState;
						break;
					case 1:
						buttonData.y = buttonsPressed;
						buttonData.pitch = triggerState;
						break;
					case 2:
						buttonData.z = buttonsPressed;
						buttonData.roll = triggerState;
						break;
					case 3:
						break;
					defaut:
						std::cout << "Unable to set button data for controller " << i << std::endl;
						break;
				}
			}
		}

		WriteToFreepie(buttonData, 3);
	}
}

void FreepieMoveClient::shutdown()
{
	// Free any allocated controller views
	for (int i = 0; i < trackedControllerCount; i++)
	{
		if (controller_views[i])
		{
			ClientPSMoveAPI::free_controller_view(controller_views[i]);
			controller_views[i] = nullptr;
		}
	}
	// Close all active network connections
	ClientPSMoveAPI::shutdown();
}

FreepieMoveClient::~FreepieMoveClient()
{
	shutdown();
}
