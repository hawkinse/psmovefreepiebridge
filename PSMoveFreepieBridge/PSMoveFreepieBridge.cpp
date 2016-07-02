#include "stdafx.h"
#include "FreepieMoveClient.h"

int main()
{
	int32_t controllerID = 0;
	std::cout << "Enter the ID of the controller you wish to use: ";
	std::cin >> controllerID;

	FreepieMoveClient* client = new FreepieMoveClient();
	client->run(controllerID);
	return 0;
}