#include "Screen.hpp"

Screen::Screen(int x, int y) {
	auto& screen = m_screen;
	m_x = x; m_y = y;

	screen.resize(x);
	for (int ix = 0; ix < x; ix++) {
		screen[ix].resize(y);
	}

	for (int ix = 0; ix < m_x; ix++) {
		for (int iy = 0; iy < m_y; iy++) {
			screen[ix][iy] = 'o';
		}
	}
}

void Screen::draw_textel(std::string symbol, int x, int y) {

	m_screen[x][y] = symbol;

}

void Screen::clear() {

	auto& screen = m_screen;

	for (int ix = 0; ix < m_x; ix++) {
		for (int iy = 0; iy < m_y; iy++) {
			screen[ix][iy] = ' ';
		}
	}

}

void Screen::display() {

	auto& screen = m_screen;

	system("cls");

	for (int ix = 0; ix < m_x; ix++) {
		for (int iy = 0; iy < m_y; iy++) {
			std::cout << screen[ix][iy];
		}
		std::cout << std::endl;
	}

}