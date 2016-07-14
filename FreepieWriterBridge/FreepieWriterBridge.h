#pragma once
#include "../thirdparty/headers/FreePIE/freepie_io.h"
using namespace System;

namespace FreepieWriterBridge {

	public ref class FreepieWriterBridge
	{
		public:static void WriteToFreepie(freepie_io_6dof_data poseData, freepie_io_6dof_data button1Data, freepie_io_6dof_data button2Data, int32_t freepiePoseIndex, int32_t freepieButton1Index, int32_t freepieButton2Index)
		{
			//Can't send freepie_io_6dof_data directly, need to copy into CS compatible struct
			FreepieWriter::FreepieWriter::FreepieData csPoseData;
			csPoseData.x = poseData.x;
			csPoseData.y = poseData.y;
			csPoseData.z = poseData.z;
			csPoseData.pitch = poseData.pitch;
			csPoseData.yaw = poseData.yaw;
			csPoseData.roll = poseData.roll;

			FreepieWriter::FreepieWriter::FreepieData csButton1Data;
			csButton1Data.x = button1Data.x;
			csButton1Data.y = button1Data.y;
			csButton1Data.z = button1Data.z;
			csButton1Data.pitch = button1Data.pitch;
			csButton1Data.yaw = button1Data.yaw;
			csButton1Data.roll = button1Data.roll;

			FreepieWriter::FreepieWriter::FreepieData csButton2Data;
			csButton2Data.x = button2Data.x;
			csButton2Data.y = button2Data.y;
			csButton2Data.z = button2Data.z;
			csButton2Data.pitch = button2Data.pitch;
			csButton2Data.yaw = button2Data.yaw;
			csButton2Data.roll = button2Data.roll;

			FreepieWriter::FreepieWriter::WriteData(csPoseData, csButton1Data, csButton2Data, freepiePoseIndex, freepieButton1Index, freepieButton2Index);
		}
	};
}

__declspec(dllexport) void WriteToFreepie(freepie_io_6dof_data poseData, freepie_io_6dof_data button1Data, freepie_io_6dof_data button2Data, int32_t freepiePoseIndex = 0, int32_t freepieButton1Index = 1, int32_t freepieButton2Index = 2)
{
	FreepieWriterBridge::FreepieWriterBridge bridge;
	bridge.WriteToFreepie(poseData, button1Data, button2Data, freepiePoseIndex, freepieButton1Index, freepieButton2Index);
}