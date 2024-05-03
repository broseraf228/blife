#include <iostream>
#include <mutex>
#include "World.hpp"

extern int WORLD_SIZE_X;
extern int WORLD_SIZE_Y;

std::mutex worldMtx;

World::World() {
	m_size_x = 0; m_size_y = 0;
}
World::World(int size_x, int size_y)
{
	m_size_x = size_x; m_size_y = size_y;
}

void World::addBeetle(float x, float y)
{
	worldMtx.lock();
	m_newBeetlsList.push_back(Beetle{ x, y});
	m_newBeetlsList[m_newBeetlsList.size() - 1].setWorld(this);
	m_newBeetlsList[m_newBeetlsList.size() - 1].setID(id_counter++);
	worldMtx.unlock();
}
void World::addBeetle(float x, float y, float rx, float ry)
{
	worldMtx.lock();
	m_newBeetlsList.push_back(Beetle{ x, y, rx, ry});
	m_newBeetlsList[m_newBeetlsList.size() - 1].setWorld(this);
	m_newBeetlsList[m_newBeetlsList.size() - 1].setID(id_counter++);
	worldMtx.unlock();
}
void World::addBeetle(float x, float y, float rx, float ry, std::vector<short> i_genome)
{
	worldMtx.lock();
	m_newBeetlsList.push_back(Beetle{ x, y, rx, ry, i_genome });
	m_newBeetlsList[m_newBeetlsList.size() - 1].setWorld(this);
	m_newBeetlsList[m_newBeetlsList.size() - 1].setID(id_counter++);
	worldMtx.unlock();
}
void World::addBeetle(float x, float y, float rx, float ry, int energy, std::vector<short> i_genome)
{
	worldMtx.lock();
	m_newBeetlsList.push_back(Beetle{ x, y, rx, ry, i_genome });
	m_newBeetlsList[m_newBeetlsList.size() - 1].m_energy = energy;
	m_newBeetlsList[m_newBeetlsList.size() - 1].setWorld(this);
	m_newBeetlsList[m_newBeetlsList.size() - 1].setID(id_counter++);
	worldMtx.unlock();
}
void World::addBeetle(float x, float y, float rx, float ry, float vx, float vy, int energy, std::vector<short> i_genome) 
{
	worldMtx.lock();
	m_newBeetlsList.push_back(Beetle{ x, y, rx, ry, vx, vy, i_genome, this });
	m_newBeetlsList[m_newBeetlsList.size() - 1].m_energy = energy;
	m_newBeetlsList[m_newBeetlsList.size() - 1].setID(id_counter++);
	worldMtx.unlock();
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
int World::getThreadCount() {
	return thread_count;
}

Beetle* World::findById(int id) {
	Beetle* nearest = nullptr;
	for (int i = 0; i < m_beetlsList.size(); i++) {
		if (m_beetlsList[i].m_id == id) {
			nearest = &(m_beetlsList[i]);
			return nearest;
		}
	}
	return nearest;

}
Beetle* World::findNearest(float x, float y){
	x = getCordX(x);
	float min = 999999;
	Beetle* nearest = nullptr;
	for (int i = 0; i < m_beetlsList.size(); i++) {
		if (pow(x - m_beetlsList[i].m_pos_x,2) + pow(y - m_beetlsList[i].m_pos_y,2) < min) {
			min = pow(x - m_beetlsList[i].m_pos_x, 2) + pow(y - m_beetlsList[i].m_pos_y, 2);
			nearest = &(m_beetlsList[i]);
		}
	}
	return nearest;
}

bool World::checkPlace(float x, float y)
{
	if ((y > m_size_y) || y < 0)
		return false;
	x = getCordX(x);

	Beetle* nearest = findNearest(x, y);
	if (nearest == nullptr)
		return true;
	float nearestdist = (*nearest).getDistanceTo(x, y);

	if (nearestdist < 10)
		return false;
	return true;
}
bool World::checkBeetleLifeStatus(int id){
	for (int i = 0; i < m_beetlsList.size(); i++) {
		if (m_beetlsList[i].m_id == id) {
			return true;
		}
	}
	return false;
}

float World::photosynthes(Beetle* target){
	float outputEnergy;
	outputEnergy = pow((WORLD_SIZE_Y - float((*target).m_pos_y)) / WORLD_SIZE_Y , 2) * 10;
	if (outputEnergy < 0) outputEnergy = 0;
	return outputEnergy;
}

void World::updateBeetls() {
	for (int i = 0; i < thread_count; i++) {
		int start = i * beetle_for_thread;
		int size = beetle_for_thread;
		if ((i + 1) * beetle_for_thread > m_beetlsList.size())
			size = m_beetlsList.size() - i * beetle_for_thread;

		m_threadList[i] = (std::thread(&World::updateBeetlsThread, this, start, size));
	}
	for (int i = 0; i < m_threadList.size(); i++)
		m_threadList[i].join();
}
void World::updateBeetlsThread(int start, int size){
	for (int i = start; i < start + size; i++) {
		m_beetlsList[i].update();
	}
}

void World::update()
{
	thread_count = (m_beetlsList.size() - m_beetlsList.size() % beetle_for_thread) / beetle_for_thread;
	if (m_beetlsList.size() % beetle_for_thread > 0)
		thread_count++;

	while (m_threadList.size() < thread_count)
		m_threadList.push_back(std::thread());

	updateBeetls();
	m_threadList.clear();


	for (int i = 0; i < m_killBeetlsList.size(); i++){
		for (int j = 0; j < m_beetlsList.size(); j++) {
			if (&(m_beetlsList[j]) == m_killBeetlsList[i])
				m_beetlsList.erase(m_beetlsList.begin() + j);
		}
		}
	m_killBeetlsList.clear();
	
	//checkNewBeetleList();
	m_beetlsList.insert(m_beetlsList.end(), m_newBeetlsList.begin(), m_newBeetlsList.end());
	m_newBeetlsList.clear();

}

void World::killBeetles(){
	
}
void World::checkNewBeetleList() {
	std::vector<Beetle*> m_tmpBeetlsList;
	for (int i = 0; i < m_newBeetlsList.size(); i++) {
		for (int j = 0; j < m_newBeetlsList.size(); j++) {
			if (i == j) continue;
			if ((m_newBeetlsList[i].m_pos_x == m_newBeetlsList[j].m_pos_x) && (m_newBeetlsList[i].m_pos_y == m_newBeetlsList[j].m_pos_y))
				m_killBeetlsList.push_back(&(m_newBeetlsList[i]));
		}
	}
	for (int i = 0; i < m_killBeetlsList.size(); i++)
		for (int j = 0; j < m_newBeetlsList.size(); j++) {
			if (&(m_newBeetlsList[j]) == m_killBeetlsList[i])
				m_newBeetlsList.erase(m_newBeetlsList.begin() + j);
		}
	m_killBeetlsList.clear();
}

std::vector<Beetle>* World::getBeetlsList()
{
	return &m_beetlsList;
}

float World::getCordX(float x){
	if (x < 0)
		return (m_size_x + x);
	if (x > m_size_x)
		return (x - m_size_x);
	return (x);
}