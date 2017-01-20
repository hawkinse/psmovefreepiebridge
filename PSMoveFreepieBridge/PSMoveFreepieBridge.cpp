#include "stdafx.h"
#include "FreepieMoveClient.h"

void prompt_arguments(int32_t &controllerCount, int32_t* controllerIDs, int32_t* bulbColors) {
	std::cout << "How many controllers do you want to track (1-4)? Note that more than 1 disables raw sensor data access and 4 disables button access: ";
	std::cin >> controllerCount;

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
}

bool parse_arguments(int argc, char** argv, int32_t &controllerCount, int32_t* controllerIDs, int32_t* bulbColors, bool &bExitWithPSMoveService) {
	bool bSuccess = true;

	int index = 1;
	while (index < argc) {
		if ((strcmp(argv[index], "-t") == 0) && controllerCount < 1) {
			index++;

			//All numeric values after the -t flag are controller IDs. Loop through and add them.
			while ((index < argc) && isdigit(*argv[index])) {
				//Only add up to four controllers
				if (controllerCount < 4) {
					controllerIDs[controllerCount] = atoi(argv[index]);
					controllerCount++;
				}
				else {
					std::cout << "More than four controllers have been specified on the command line!" << std::endl;
					bSuccess = false;
				}

				index++;
			}
		}
		else if (strcmp(argv[index], "-c") == 0) {
			index++;
			int32_t colorIndex = 0;
			//All numeric values after the -c flag are color indicies
			while ((index < argc) && (isdigit(*argv[index]) || (strcmp(argv[index], "-1") == 0))) {
				//Only add up to four controller indicies
				if (colorIndex < 4) {
					bulbColors[colorIndex] = atoi(argv[index]);
					colorIndex++;
				}
				else {
					std::cout << "More than four colors have been specified on the command line!" << std::endl;
					bSuccess = false;
				}

				index++;
			}
		}
		else if (strcmp(argv[index], "-x") == 0) {
			std::cout << "-x flag specified. Will not keep window open when finished" << std::endl;
			bExitWithPSMoveService = true;
			index++;
		}
		else {
			std::cout << "Unrecognized command line argument " << argv[index] << std::endl;
			bSuccess = false;
			break;
		}
	}

	return bSuccess;
}

int main(int argc, char** argv)
{
	int32_t controllerCount = 0;
	int32_t controllerIDs[4];
	int32_t freepieIndicies[4] = { 0, 1, 2, 3 };
	int32_t bulbColors[4] = { -1, -1, -1, -1 };
	bool bRun = true;
	bool bExitWithPSMoveService = false;

	if (argc == 1) {
		prompt_arguments(controllerCount, controllerIDs, bulbColors);
	}
	else {
		if (!parse_arguments(argc, argv, controllerCount, controllerIDs, bulbColors, bExitWithPSMoveService)) {
			std::cout << "Command line arguments are not valid." << std::endl;
			bRun = false;;
		}
	}

	if (bRun) {
		FreepieMoveClient* client = new FreepieMoveClient();
		client->run(controllerCount, controllerIDs, bulbColors, freepieIndicies, controllerCount < 2);
	}

	std::cout << "PSMoveFreepieBridge has ended" << std::endl;

	if (!bExitWithPSMoveService) {
		std::cin.ignore(INT_MAX);
	}

	return 0;
}