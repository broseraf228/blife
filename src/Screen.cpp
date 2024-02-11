#include "Screen.hpp"

void Screen::resizeX(int x)
{
	m_sx = x+1;
	m_screen.resize(x+1);
	for (int ix = 0; ix < m_sx; ix++) {
		m_screen[ix].resize(m_sy);
	}
}
void Screen::resizeY(int y)
{
	m_sy = y;
	for (int ix = 0; ix < m_sx; ix++) {
		m_screen[ix].resize(y + 1);
	}
}
void Screen::resize(int x, int y)
{

}

Screen::Screen(int x, int y) {
	auto& screen = m_screen;
	m_sx = x; m_sy = y;

	screen.resize(x);
	for (int ix = 0; ix < x; ix++) {
		screen[ix].resize(y);
	}

	for (int ix = 0; ix < m_sx; ix++) {
		for (int iy = 0; iy < m_sy; iy++) {
			screen[ix][iy] = 'o';
		}
	}
}

void Screen::draw_textel(std::string symbol, int x, int y) {

	if (x > m_sx - 1)
		resizeX(x);
	if (y > m_sy-1)
		resizeY(y);

	m_screen[x][y] = symbol;

}

void Screen::clear() {

	auto& screen = m_screen;

	for (int ix = 0; ix < m_sx; ix++) {
		for (int iy = 0; iy < m_sy; iy++) {
			screen[ix][iy] = ' ';
		}
	}

}

void Screen::display() {

	auto& screen = m_screen;

	std::string tmp_str = "";


	for (int iy = 0; iy < m_sy+1; iy++) {
		for (int ix = 0; ix < m_sx; ix++) {
			tmp_str.append(screen[ix][iy]);
		}
		tmp_str.append("\n");
	}
	system("cls");
	std::cout << tmp_str << std::endl;

}