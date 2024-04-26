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
	void addBeetle(float x, float y, float rx, float ry, std::vector<short> i_genome);
	void addBeetle(float x, float y, float rx, float ry, int energy, std::vector<short> i_genome);
	
	void killBeetle(Beetle* target);

	float statistic(int type);

	Beetle* findById(int id);
	Beetle* findNearest(float x, float y);
	Beetle* findNearest(float x, float y, Beetle* beetle);
	bool checkPlace(Beetle* beetle, float x, float y);
	bool checkBeetleLifeStatus(int id);

	float photosynthes(Beetle* target);

	std::vector<Beetle>* getBeetlsList();

	int m_size_x; int m_size_y;
private:
	int id_counter{ 1 };
	std::vector<Beetle> m_beetlsList;
	std::vector<Beetle> m_newBeetlsList;
	std::vector<Beetle*> m_killBeetlsList;
};
#endif