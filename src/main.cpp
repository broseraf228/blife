#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <time.h>
#include <mutex>
#include "Screen.hpp"
#include "World.hpp"
#include "Beetl.hpp"
#include <SFML/Graphics.hpp>

//TODO: починить систему координат; помянять команды генома полностью  

int WINDOW_SIZE_X = 1600;
int WINDOW_SIZE_Y = 800;
int WORLD_SIZE_X = 1600;
int WORLD_SIZE_Y = 1200;
std::string CURRENT_FOLDER;
int step = 0; float mouseX = 0; float mouseY = 0;
bool pause = false;
bool display_world = true;
int timing = 5;


int selectedBeetlID = 0;

clock_t start_graphic, end_graphic, start_world, end_world;

World* lWorld; Screen* lScreen;

void statisticDraw();

int main(int argc, char* argv[])
{
	std::cout << "settings" << std::endl;
	std::cout << "\t fullscreen? (y/n) > ";
	std::string ans;
	std::cin >> ans;
	if (ans == "y") {
		WINDOW_SIZE_X = sf::VideoMode::getDesktopMode().width;
		WINDOW_SIZE_Y = sf::VideoMode::getDesktopMode().height;
	}


	std::cout <<"initialization" << std::endl;
	std::cout << "\system initialization" << std::endl;
	CURRENT_FOLDER = argv[0];
	for (int i = CURRENT_FOLDER.size(); CURRENT_FOLDER[i] != '\\'; i--)
		CURRENT_FOLDER.replace(i, 1, "");
	std::cout << CURRENT_FOLDER << std::endl;
	srand(time(NULL));
	std::cout << "\tcomplete" << std::endl;
	std::cout << "\tworld initialization" << std::endl;

	World world(WORLD_SIZE_X, WORLD_SIZE_Y);
	lWorld = &world;

	std::cout << "\tcomplete" << std::endl;
	std::cout << "\tscreen initialization" << std::endl;

	Screen screen("Window", lWorld);
	sf::RenderWindow& window = screen.getWindow();
	screen.setMode(0);
	lScreen = &screen;

	std::cout << "\tcomplete" << std::endl;
	std::cout << "complete" << std::endl;

	for (int i = 0; i < 5000; i++) {
		world.addBeetle(rand() % WORLD_SIZE_X, rand() % WORLD_SIZE_Y, 1, 0, float(rand() % 10)/10.0-0.5, float(rand() % 10) / 10.0 - 0.5, 100, std::vector<short>(64,0));
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			///-----------WINDOW-----------------------------------------
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized) {
				WINDOW_SIZE_X = event.size.width;
				WINDOW_SIZE_Y = event.size.height;
			}

			///-----------MOUSE------------------------------------------
			if (event.type == sf::Event::MouseMoved) {}
			if (event.type == sf::Event::MouseButtonPressed) {
				mouseX = sf::Mouse::getPosition(window).x;
				mouseY = sf::Mouse::getPosition(window).y;
				screen.convertCords(&mouseX, &mouseY);
				selectedBeetlID = (*(world.findNearest(mouseX, mouseY))).m_id;

			}
			///-----------KEYBOARD---------------------------------------
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.scancode == sf::Keyboard::Scan::Num0) {
					timing = (timing + 10);
				}
				if (event.key.scancode == sf::Keyboard::Scan::Num9) {
					timing = (timing - 10);
				}

				if (event.key.scancode == sf::Keyboard::Scan::Space) {
					if (pause == true) pause = false;
					else pause = true;
				}
				if (event.key.scancode == sf::Keyboard::Scan::W) {
					if (display_world == true) display_world = false;
					else display_world = true;
				}
				if (event.key.scancode == sf::Keyboard::Scan::F1) {
					screen.setMode(0);
				}
				if (event.key.scancode == sf::Keyboard::Scan::F2) {
					screen.setMode(1);
				}
				if (event.key.scancode == sf::Keyboard::Scan::F3) {
					screen.setMode(2);
				}
			}

		}
		mouseX = sf::Mouse::getPosition(window).x;
		mouseY = sf::Mouse::getPosition(window).y;
		screen.convertCords(&mouseX, &mouseY);

		if (world.findById(selectedBeetlID) == nullptr) {
			selectedBeetlID = 0;
		}

		//-----------------------
		if (!pause) {
			start_world = clock();
			try { world.update(); }
			catch (const char* error_message) { std::cout << error_message << std::endl; }
			step++;
			end_world = clock();
		}
		//-----------=-----

		start_graphic = clock();

		if (display_world) {
			try { screen.drawWorld(); }
			catch (const char* error_message) { std::cout << error_message << std::endl; }
		}
		if (selectedBeetlID != 0) {
			screen.addBeetleToGraphicArray(world.findById(selectedBeetlID), sf::Color(255,255,255,255));
			screen.drawBeetleStat(world.findById(selectedBeetlID));
		}
		screen.display();
		screen.clear();

		end_graphic = clock();
		screen.drawSystem();

		std::this_thread::sleep_for(std::chrono::milliseconds(int(timing - 100*(((double)end_world - start_world) / ((double)CLOCKS_PER_SEC) + ((double)end_graphic - start_graphic) / ((double)CLOCKS_PER_SEC)))));
	}

	return 0;
}