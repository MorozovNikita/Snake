#include "Application.h"

#include <stdexcept>
#include <iostream>


int main()
{
	try
	{
		Game::Application app;
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}