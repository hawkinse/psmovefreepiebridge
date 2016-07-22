#pragma once
#include "../thirdparty/headers/FreePIE/freepie_io.h"
using namespace System;

namespace FreepieWriterBridge {

	public ref class FreepieWriterBridge
	{
		public:static void WriteToFreepie(freepie_io_6dof_data poseData, freepie_io_6dof_data extraData1, freepie_io_6dof_data extraData2, int32_t freepiePoseIndex, int32_t freepieExtra1Index, int32_t freepieExtra2Index)
		{
			//Can't send freepie_io_6dof_data directly, need to copy into CS compatible struct
			FreepieWriter::FreepieWriter::FreepieData csPoseData;
			csPoseData.x = poseData.x;
			csPoseData.y = poseData.y;
			csPoseData.z = poseData.z;
			csPoseData.pitch = poseData.pitch;
			csPoseData.yaw = poseData.yaw;
			csPoseData.roll = poseData.roll;

			FreepieWriter::FreepieWriter::FreepieData csExtra1Data;
			csExtra1Data.x = extraData1.x;
			csExtra1Data.y = extraData1.y;
			csExtra1Data.z = extraData1.z;
			csExtra1Data.pitch = extraData1.pitch;
			csExtra1Data.yaw = extraData1.yaw;
			csExtra1Data.roll = extraData1.roll;

			FreepieWriter::FreepieWriter::FreepieData csExtra2Data;
			csExtra2Data.x = extraData2.x;
			csExtra2Data.y = extraData2.y;
			csExtra2Data.z = extraData2.z;
			csExtra2Data.pitch = extraData2.pitch;
			csExtra2Data.yaw = extraData2.yaw;
			csExtra2Data.roll = extraData2.roll;

			FreepieWriter::FreepieWriter::WriteData(csPoseData, csExtra1Data, csExtra2Data, freepiePoseIndex, freepieExtra1Index, freepieExtra2Index);
		}
	};
}

__declspec(dllexport) void WriteToFreepie(freepie_io_6dof_data poseData, freepie_io_6dof_data extra1Data, freepie_io_6dof_data extra2Data, int32_t freepiePoseIndex = 0, int32_t freepieExtra1Index = 1, int32_t freepieExtra2Index = 2)
{
	FreepieWriterBridge::FreepieWriterBridge bridge;
	bridge.WriteToFreepie(poseData, extra1Data, extra2Data, freepiePoseIndex, freepieExtra1Index, freepieExtra2Index);
}