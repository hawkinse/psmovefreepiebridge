#include "stdafx.h"
#include "FreepieMoveClient.h"

int main()
{
	int32_t controllerCount = 0;
	std::cout << "How many controllers do you want to track (1-4)? Note that more than 1 disables raw sensor data access: ";
	std::cin >> controllerCount;

	int32_t controllerIDs[4];
	int32_t freepieIndicies[4] = { 0, 1, 2, 3 };
	int32_t bulbColors[4];
	for (int i = 0; i < controllerCount; i++) 
	{
		int32_t controllerID = 0;
		std::cout << "Enter the ID of the controller you wish to use: ";
		std::cin >> controllerID;
		
		int32_t bulbColor = -1;
		char customColorChoice;
		std::cout << "Do you want to use a custom bulb color for this controller (y/n)? ";
		std::cin >> customColorChoice;
		if (customColorChoice == 'Y' || customColorChoice == 'y') {
			std::cout << "Color Options:\n";
			std::cout << "  0 - Magenta\n";
			std::cout << "  1 - Cyan\n";
			std::cout << "  2 - Yellow\n";
			std::cout << "  3 - Red\n";
			std::cout << "  4 - Green\n";
			std::cout << "  5 - Blue\n";
			std::cout << "Enter the number of the desired color from the above list: ";
			std::cin >> bulbColor;
		}

		controllerIDs[i] = controllerID;
		bulbColors[i] = bulbColor;
	}

	FreepieMoveClient* client = new FreepieMoveClient();
	client->run(controllerCount, controllerIDs, bulbColors, freepieIndicies, controllerCount < 2);

	return 0;
}