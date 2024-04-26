#include "Screen.hpp"
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <time.h>

extern int WINDOW_SIZE_X;
extern int WINDOW_SIZE_Y;
extern int WORLD_SIZE_X;
extern int WORLD_SIZE_Y;
extern std::string CURRENT_FOLDER;
extern int timing;
extern float mouseX; extern float mouseY;

extern bool pause;
extern int step;
extern clock_t start_graphic, end_graphic, start_world, end_world;


Screen::Screen(std::string name, World* world)
{
	m_world = world;
	m_beetlsList = (*m_world).getBeetlsList();

	m_window.create(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), name);
	m_screen_size_x = WINDOW_SIZE_X; m_screen_size_y = WINDOW_SIZE_Y;
	if (!m_font.loadFromFile(CURRENT_FOLDER + "1font.ttf")) std::cerr << "can't load the font <1font.ttf>";
	m_mode = 0;

	recalculateDisplaySettings();
}
void Screen::recalculateDisplaySettings() {
	float r1 = float(WORLD_SIZE_X) / (WINDOW_SIZE_X);
	float r2 = float(WORLD_SIZE_Y) / WINDOW_SIZE_Y;

	m_screen_size_x = WORLD_SIZE_X;
	m_screen_size_y = float(WORLD_SIZE_Y / r1);
	if (m_screen_size_y > WINDOW_SIZE_Y) {
		m_screen_size_y = WINDOW_SIZE_Y;
		m_screen_size_x = float(WORLD_SIZE_X / r2);
	}

	m_scale_x = m_screen_size_x / WORLD_SIZE_X; m_scale_y = m_screen_size_y / WORLD_SIZE_Y;
}

void Screen::drawVertexes() {
	sf::RenderTexture world_texture;
	if (!world_texture.create(WORLD_SIZE_X, WORLD_SIZE_Y)) std::cerr << "can't create tmp texture";
	world_texture.draw(m_beetleBodyVertexArray);
	m_beetleBodyVertexArray.clear();
	world_texture.display();
	sf::Sprite world_sprite(world_texture.getTexture());
	world_sprite.setScale(m_scale_x, m_scale_y);
	m_window.draw(world_sprite);
	world_texture.clear(sf::Color(25, 25, 25, 255));
}

void Screen::addBeetleToGraphicArray(Beetle* beetle, sf::Color& color) {
	if (beetle == nullptr)
		return;
	m_beetleBodyVertexArray.append(sf::Vertex{
				sf::Vector2f(
					((*beetle).m_pos_x + (*beetle).m_rot_x * (*beetle).m_size),
					((*beetle).m_pos_y + (*beetle).m_rot_y * (*beetle).m_size)),
				color });
	m_beetleBodyVertexArray.append(sf::Vertex{
		sf::Vector2f(
			((*beetle).m_pos_x - (*beetle).m_rot_y * (*beetle).m_size * 0.5),
			((*beetle).m_pos_y + (*beetle).m_rot_x * (*beetle).m_size * 0.5)),
		color });
	m_beetleBodyVertexArray.append(sf::Vertex{
		sf::Vector2f(
			((*beetle).m_pos_x + (*beetle).m_rot_y * (*beetle).m_size * 0.5),
			((*beetle).m_pos_y - (*beetle).m_rot_x * (*beetle).m_size * 0.5)),
		color });
}

void Screen::drawWorld()
{
	std::vector<Beetle>& beetls = *(m_beetlsList);
	int bitleSize = beetls.size();

	float max = 0; float color = 0;

	switch (m_mode)
	{
	case 0:
		for (int i = 0; i < bitleSize; i++)
		{
			addBeetleToGraphicArray(&((*m_beetlsList)[i]), sf::Color(100,200,100,200));
		}
		break;
	case 1:

		for (int i = 0; i < bitleSize; i++)
		{
			color = beetls[i].m_energy / 2;
			addBeetleToGraphicArray(&((*m_beetlsList)[i]), sf::Color(255, color, color, 200));
		}
		break;
	case 2:
		for (int i = 0; i < bitleSize; i++) {
			if (beetls[i].m_age > max) max = beetls[i].m_age;
		}

		for (int i = 0; i < bitleSize; i++)
		{
			color = (beetls[i].m_age) / max * 100;
			addBeetleToGraphicArray(&((*m_beetlsList)[i]), sf::Color(color, color, color, 200));
		}
		break;
	}

	//----draw world----
	drawVertexes();

}

void Screen::drawBeetleStat(Beetle* beetle) {
	if (beetle == nullptr) return;
	Beetle& lbeetle = (*beetle);
	///????????????????????//////?????//?????//???
	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(12);
	text.setPosition(sf::Vector2f(m_screen_size_x + 10, 0));
	std::string string = "";

	for (int i = 0; i < lbeetle.m_genome.size(); i++) {
		string.append(std::to_string(lbeetle.m_genome[i]) + "   ");
		if (i == lbeetle.m_genomeCounter) {
			string.append("<");
		}
		else string.append(" ");
		if (i % 6 == 5) {
			string.append(std::to_string(lbeetle.m_genome[i]) + "\n");
		}
		
		
	}

	
	string.append("\n");
	string.append("pX= " + std::to_string(lbeetle.m_pos_x) + "\n");
	string.append("pY= " + std::to_string(lbeetle.m_pos_y) + "\n");
	string.append("energy= " + std::to_string(lbeetle.m_energy) + "\n");
	string.append("age= " + std::to_string(lbeetle.m_age) + "\n");

	text.setString(string);
	m_window.draw(text);
}

void Screen::drawSystem() {
	//----draw info---
	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(12);
	text.setPosition(sf::Vector2f(m_screen_size_x + 10, m_screen_size_y - 100));
	std::string string = "";

	string.append("timing: " + std::to_string(timing) + "\n");
	string.append("step: " + std::to_string(step) + "\n");
	string.append("beetls: " + std::to_string((*m_world).getBeetlsList()->size()) + "\n");
	string.append("world time: " + std::to_string(((double)end_world - start_world) / ((double)CLOCKS_PER_SEC)) + "\n");
	string.append("graphics prepare: " + std::to_string(((double)end_graphic - start_graphic) / ((double)CLOCKS_PER_SEC)) + "\n");

	text.setString(string);
	m_window.draw(text);


	string = std::string("LMB - select bot\nF1 - default mode\nF2 - energy mode\nF3 - age mode\nW - don't draw workd\n\n0-3 : fotosinthes\n4-7 : move\n8-11 : rotate\n12-12 : clone\n13-15 : hunt\n21 - if on top  ");
	text.setPosition(sf::Vector2f(m_screen_size_x + 200, 0));
	text.setString(string);
	m_window.draw(text);
	//----draw menu-----
	sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(m_screen_size_x, 0));
	rect.setSize(sf::Vector2f(2, m_screen_size_y));

	rect.setFillColor(sf::Color::Green);
	if (pause)
		rect.setFillColor(sf::Color::Red);
	m_window.draw(rect);
}

void Screen::clear() { m_window.clear(); }
void Screen::display() { m_window.display(); }

void Screen::setMode(int mode) {
	if(mode == 0 || mode == 1 || mode == 2)
		m_mode = mode;
}

sf::RenderWindow& Screen::getWindow()
{
	return m_window;
}

void Screen::convertCords(float* x, float* y) {
	double r1 = float(WORLD_SIZE_X) / (WINDOW_SIZE_X);
	double r2 = float(WORLD_SIZE_Y) / WINDOW_SIZE_Y;

	double screen_size_x = WORLD_SIZE_X;
	double screen_size_y = double(WORLD_SIZE_Y) / r1;
	if (screen_size_y > WINDOW_SIZE_Y) {
		screen_size_y = WINDOW_SIZE_Y;
		screen_size_x = double(WORLD_SIZE_X) / r2;
	}

	*x = (*x) / (screen_size_x / WORLD_SIZE_X);
	*y = (*y) / (screen_size_y / WORLD_SIZE_Y);
}

float Screen::getMouseScaleX() {
	float r1 = float(WORLD_SIZE_X) / (WINDOW_SIZE_X);
	float r2 = float(WORLD_SIZE_Y) / WINDOW_SIZE_Y;

	float screen_size_x = WORLD_SIZE_X;
	float screen_size_y = float(WORLD_SIZE_Y / r1);
	if (m_screen_size_y > WINDOW_SIZE_Y) {
		m_screen_size_y = WINDOW_SIZE_Y;
		m_screen_size_x = float(WORLD_SIZE_X / r2);
	}

	return (screen_size_x / WORLD_SIZE_X);
}
float Screen::getMouseScaleY() {
	float r1 = float(WORLD_SIZE_X) / (WINDOW_SIZE_X);
	float r2 = float(WORLD_SIZE_Y) / WINDOW_SIZE_Y;

	float screen_size_x = WORLD_SIZE_X;
	float screen_size_y = float(WORLD_SIZE_Y / r1);
	if (m_screen_size_y > WINDOW_SIZE_Y) {
		m_screen_size_y = WINDOW_SIZE_Y;
		m_screen_size_x = float(WORLD_SIZE_X / r2);
	}

	return (screen_size_y / WORLD_SIZE_Y);
}