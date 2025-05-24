#include "Launcher.h"

int main()
{
	Launcher launcher;

	if (!launcher.CheckAndUpdate()) 
	{
		std::cout << "[LAUNCHER] Version check failed. Exiting" << std::endl;
		return 1;
	}

	std::cout << "[LAUNCHER] Update check passed. Starting game..." << std::endl;

	return 0;
}