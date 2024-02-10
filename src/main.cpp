#include <iostream>
#include <ctime>
//#include <thread>
#include "Windows.h"

#include "Screen.hpp"

const float FRAMERATE_LIMIT = 10;
const float FRAME_TIME = 1 / FRAMERATE_LIMIT * 1000;

int main()
{
	Screen screen(5, 5);

	int px = 0;
	int py = 0;

	while(true)
	{
		clock_t start = clock();
		//----------------------------------------------------------cod
		screen.clear();
		px++; py++;
		px %= 5; py %= 5;

		screen.draw_textel("S", px, py);

		screen.display();
		//----------------------------------------------------------end
		clock_t finish = clock();
		double time_elapsed = double(finish - start) / CLOCKS_PER_SEC;

		std::cout << "fps: " << std::to_string(1000 / time_elapsed) << std::endl;
		Sleep(FRAME_TIME - time_elapsed);
		
	}

	return 0;
}