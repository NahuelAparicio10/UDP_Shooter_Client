#include "Launcher.h"
#include "GameManager.h"
int main()
{
	//Launcher launcher;

	//if (!launcher.CheckAndUpdate()) 
	//{
	//	std::cout << "[LAUNCHER] Version check failed. Exiting" << std::endl;
	//	return 1;
	//}

	//std::cout << "[LAUNCHER] Update check passed. Starting game..." << std::endl;

	GameManager gameManager;

	gameManager.Run();

	return 0;
}