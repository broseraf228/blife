#include <iostream>
#include <ctime>
//#include <thread>
#include "Windows.h"

#include "Screen.hpp"
#include "World.hpp"

const float FRAMERATE_LIMIT = 5;
const float FRAME_TIME = 1 / FRAMERATE_LIMIT * 1000;

int main()
{
	Screen screen(5, 5);

	World world(&screen, 50, 50);

	world.setMode(2);

	std::string input = "";

	while(true)
	{
		clock_t start = clock();
		//----------------------------------------------------------cod
		screen.clear();

		world.draw();

		screen.display();
		//----------------------------------------------------------end
		clock_t finish = clock();
		double time_elapsed = double(finish - start) / CLOCKS_PER_SEC;

		std::cout << "time elaps: " << std::to_string(time_elapsed) << std::endl;

		std::cin >> input;
		if (input == "m1")
			world.setMode(1);
		else if (input == "m2")
			world.setMode(2);
		else
			NULL;
		//Sleep(FRAME_TIME - time_elapsed);
		
	}

	return 0;
}