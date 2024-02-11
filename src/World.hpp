#pragma once
#include <vector>
#include <string>

#include"Screen.hpp"

class Tile {
public:
	Tile();
	Tile(std::string picture);
	Tile(std::string picture, float temperatur, float tempConductivity);

	std::string m_picture;
	float m_temperatur;
	float m_tempConductivity;
};

//===========================================================================

class World {
private:
	std::vector<std::vector<Tile>> m_worldMap;

	Screen* m_screen;

	int m_sx; int m_sy;


	int m_mode;
public:
	World(Screen* i_screen, int sx, int sy);

	void setMode(int mode);

	void draw();
};