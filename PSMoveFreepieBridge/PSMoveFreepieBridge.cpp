#include "stdafx.h"
#include "FreepieMoveClient.h"

int main()
{
	int32_t controllerCount = 0;
	std::cout << "How many controllers do you want to track (1-4)? Note that more than 1 disables raw sensor data access: ";
	std::cin >> controllerCount;

	int32_t controllerIDs[4];
	int32_t freepieIndicies[4] = { 0, 1, 2, 3 };
	for (int i = 0; i < controllerCount; i++) 
	{
		int32_t controllerID = 0;
		std::cout << "Enter the ID of the controller you wish to use: ";
		std::cin >> controllerID;

		controllerIDs[i] = controllerID;
	}

	FreepieMoveClient* client = new FreepieMoveClient();
	client->run(controllerCount, controllerIDs, freepieIndicies, controllerCount < 2);

	return 0;
}