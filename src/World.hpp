#ifndef b_world_h
#define b_world_h

#include <vector>
#include "Beetl.hpp"

class World
{
public:
	World();
	World(int size_x, int size_y);
	

	void update();

	void addBeetle(float x, float y);
	void addBeetle(float x, float y, float rx, float ry);
	void addBeetle(float x, float y, float rx, float ry, std::vector<int> i_genome);
	void addBeetle(float x, float y, float rx, float ry, int energy, std::vector<int> i_genome);
	
	void killBeetle(Beetle* target);

	float statistic(int type);

	Beetle* findNearest(float x, float y);
	bool checkPlace(Beetle* beetle, float x, float y);
	bool checkBeetleLifeStatus(Beetle* beetle);

	int photosynthes(Beetle* target);

	std::vector<Beetle>* getBeetlsList();

	int m_size_x; int m_size_y;
private:
	std::vector<Beetle> m_beetlsList;
	std::vector<Beetle> m_newBeetlsList;
	std::vector<Beetle*> m_killBeetlsList;
};
#endif