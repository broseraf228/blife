#include <iostream>
#include "World.hpp"

extern int WORLD_SIZE_X;
extern int WORLD_SIZE_Y;

World::World() {
	m_size_x = 0; m_size_y = 0;
}
World::World(int size_x, int size_y)
{
	m_size_x = size_x; m_size_y = size_y;
}

void World::addBeetle(float x, float y)
{
	m_newBeetlsList.push_back(Beetle{ x, y});
	m_newBeetlsList[m_newBeetlsList.size() - 1].setWorld(this);
}
void World::addBeetle(float x, float y, float rx, float ry)
{
	m_newBeetlsList.push_back(Beetle{ x, y, rx, ry});
	m_newBeetlsList[m_newBeetlsList.size() - 1].setWorld(this);
}
void World::addBeetle(float x, float y, float rx, float ry, std::vector<int> i_genome)
{
	m_newBeetlsList.push_back(Beetle{ x, y, rx, ry, i_genome });
	m_newBeetlsList[m_newBeetlsList.size() - 1].setWorld(this);
}
void World::addBeetle(float x, float y, float rx, float ry, int energy, std::vector<int> i_genome)
{
	m_newBeetlsList.push_back(Beetle{ x, y, rx, ry, i_genome });
	m_newBeetlsList[m_newBeetlsList.size() - 1].m_energy = energy;
	m_newBeetlsList[m_newBeetlsList.size() - 1].setWorld(this);
}

void World::killBeetle(Beetle* target) {
	m_killBeetlsList.push_back(target);
}

float World::statistic(int type) {
	switch (type)
	{
	case 1:
		return m_beetlsList.size();
	default:
		break;
	}
}

Beetle* World::findNearest(float x, float y){
	float min = 999999 ;
	Beetle* nearest = nullptr;
	for (int i = 0; i < m_beetlsList.size(); i++) {
		if (sqrt(pow(x - m_beetlsList[i].m_pos_x, 2) + pow(y - m_beetlsList[i].m_pos_y, 2)) < min) {
			min = sqrt(pow(x - m_beetlsList[i].m_pos_x, 2) + pow(y - m_beetlsList[i].m_pos_y, 2));
			nearest = &(m_beetlsList[i]);
		}
	}
	return nearest;
	
}
bool World::checkPlace(Beetle* beetle, float x, float y)
{
	if ((y > m_size_y) || y < 0)
		return false;

	for (int i = 0; i < m_beetlsList.size(); i++) {
		if (&m_beetlsList[i] == beetle) continue;

		float a = sqrt(pow(x - m_beetlsList[i].m_pos_x, 2) + pow(y - m_beetlsList[i].m_pos_y, 2));
		if (a < ((m_beetlsList[i].m_size + (*beetle).m_size)/4))
			return false;
	}
	return true;
}
bool World::checkBeetleLifeStatus(Beetle* beetle){
	for (int i = 0; i < m_beetlsList.size(); i++) {
		if (&(m_beetlsList[i]) == beetle) {
			return true;
		}
	}
	return false;
}

int World::photosynthes(Beetle* target){
	int outputEnergy = ((WORLD_SIZE_Y)-(*target).m_pos_y) / 100;
	if (outputEnergy < 0) outputEnergy = 0;
	return outputEnergy;
}

void World::update()
{
	for (int i = 0; i < m_beetlsList.size(); i++) {
		m_beetlsList[i].update();

	}

	for (int i = 0; i < m_killBeetlsList.size(); i++)
		for (int j = 0; j < m_beetlsList.size(); j++) {
			if (&(m_beetlsList[j]) == m_killBeetlsList[i])
				m_beetlsList.erase(m_beetlsList.begin() + j);
		}
	m_killBeetlsList.clear();

	

	m_beetlsList.insert(m_beetlsList.end(), m_newBeetlsList.begin(), m_newBeetlsList.end());
	m_newBeetlsList.clear();

}

std::vector<Beetle>* World::getBeetlsList()
{
	return &m_beetlsList;
}