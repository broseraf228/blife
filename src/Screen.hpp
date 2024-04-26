#ifndef b_screen_h
#define b_screen_h

#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Beetl.hpp"

struct pair { int x; int y; };

class Screen 
{
public:
	Screen();
	Screen(std::string name, World* world);
	void recalculateDisplaySettings();

	void drawWorld();
	void addBeetleToGraphicArray(Beetle* beetle, sf::Color& color);
	void drawBeetleStat(Beetle* beetle);
	void drawSystem();
	void clear(); void display();

	void setMode(int mode);

	sf::RenderWindow& getWindow();

	void convertCords(float* x, float* y);

	float getMouseScaleX();
	float getMouseScaleY();

private:
	void drawVertexes();
	int m_mode{0};
	float m_screen_size_x, m_screen_size_y; double m_scale_x, m_scale_y;

	World* m_world;

	std::vector<Beetle>* m_beetlsList;

	sf::RenderWindow m_window{};
	sf::Font m_font;

	//vertex arrays for graphic ----->
	sf::VertexArray m_beetleBodyVertexArray{sf::Triangles, 0};
};

#endif 
