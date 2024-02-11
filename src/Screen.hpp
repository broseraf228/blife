#pragma once
#include <string>
#include <vector>
#include <iostream>


class Screen {
private:
	std::vector<std::vector<std::string>> m_screen;

	int m_sx, m_sy;

	void resizeX(int x);
	void resizeY(int y);
	void resize(int x, int y);

public:
	Screen(int x, int y);

	void draw_textel(std::string symbol, int x,int y);
	void clear();

	void display();
};