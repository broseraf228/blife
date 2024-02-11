#include "World.hpp"

Tile::Tile() {
	m_picture = "?";
	m_temperatur = 0;
	m_tempConductivity = 0;
}

Tile::Tile(std::string picture) {
	m_picture = picture;
	m_temperatur = 0;
	m_tempConductivity = 0;
}

Tile::Tile(std::string picture, float temperatur, float tempConductivity) {
	m_picture = picture;
	m_temperatur = temperatur;
	m_tempConductivity = tempConductivity;
}



//===========================================================================

World::World(Screen* i_screen, int sx, int sy) {
	m_screen = i_screen; m_sx = sx; m_sy = sy;
	m_mode = 1;

	m_worldMap.resize(sx);
	for (int ix = 0; ix < sx; ix++) {
		m_worldMap[ix].resize(sy);
	}

	for (int ix = 0; ix < m_sx; ix++) {
		for (int iy = 0; iy < m_sy; iy++) {
			if(ix == 0 || ix == m_sx-1 || iy == 0 || iy == m_sy-1)
				m_worldMap[ix][iy] = Tile("##", 1, 0);
			else
				m_worldMap[ix][iy] = Tile("  ", 0, 0.1);
		}
	}
}

void World::setMode(int mode) {
	m_mode = mode;
}

void World::draw() {
	Screen& screen = *m_screen;

	switch (m_mode)
	{
	case 1:
		for (int ix = 0; ix < m_sx; ix++) {
			for (int iy = 0; iy < m_sy; iy++) {
				screen.draw_textel(m_worldMap[ix][iy].m_picture, ix, iy);
			}
		}
		break;
	case 2:
		for (int ix = 0; ix < m_sx; ix++) {
			for (int iy = 0; iy < m_sy; iy++) {
				std::string num_text = std::to_string(m_worldMap[ix][iy].m_temperatur);
				if (num_text[1] == '.')
					num_text = "0" + num_text;
				num_text = num_text.substr(0,2);
				screen.draw_textel(num_text, ix, iy);
			}
		}
		break;
	default:
		break;
	}
	
}