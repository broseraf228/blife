#include <iostream>
#include <mutex>
#include "Beetl.hpp"
#include "World.hpp"


extern int WORLD_SIZE_X;
extern int WORLD_SIZE_Y;
extern std::mutex worldMtx;

std::vector<short> defaultGenome(96, 0);
std::vector<short> genomeCommands{0,1,2,3,4,5};

Beetle::Beetle(float x, float y) {
	m_pos_x = x; m_pos_y = y; m_rot_x = 1; m_rot_y = 0;
	m_genome = defaultGenome;
}
Beetle::Beetle(float x, float y, float rx, float ry){
	m_pos_x = x; m_pos_y = y; m_rot_x = rx; m_rot_y = ry;
	m_genome = defaultGenome;
}
Beetle::Beetle(float x, float y, float rx, float ry, std::vector<short> i_genome) {
	m_pos_x = x; m_pos_y = y; m_rot_x = rx; m_rot_y = ry;
	m_genome = i_genome;
}
Beetle::Beetle(float x, float y, float rx, float ry, std::vector<short> i_genome, World* world) {
	m_pos_x = x; m_pos_y = y; m_rot_x = rx; m_rot_y = ry;
	m_genome = i_genome;
	m_world = world;
}
Beetle::Beetle(float x, float y, float rx, float ry, float vx, float vy, std::vector<short> i_genome, World* world) {
	m_pos_x = x; m_pos_y = y; m_rot_x = rx; m_rot_y = ry; m_vec_x = vx; m_vec_y = vy;
	m_genome = i_genome;
	m_world = world;
}
void Beetle::setWorld(World* world) {
	m_world = world;
}
void Beetle::setID(int id) {
	m_id = id;
}

void Beetle::moveByVec() {
	
}

void Beetle::update()
{
	if (m_energy <= 0) {
		(*m_world).killBeetle(this);
		return;
	}
	World& world = (*m_world);
	n_pos_x = m_pos_x, n_pos_y = m_pos_y, n_rot_x = m_rot_x, n_rot_y = m_rot_y, n_vec_x = m_vec_x, n_vec_y = m_vec_y, n_energy = m_energy, n_genomeCounter = m_genomeCounter;
	//beetle mov ****************************************************
	if (n_pos_y + n_vec_y < 0 || n_pos_y + n_vec_y > WORLD_SIZE_Y)
		n_vec_y = n_vec_y * -1;
	if((*m_world).checkPlace(getCordX(n_pos_x + n_vec_x), n_pos_y + n_vec_y)){
		n_pos_x = getCordX(n_pos_x + n_vec_x);
		n_pos_y += n_vec_y;
	}
	else {
		n_vec_x = 0; n_vec_y = 0;
	}
	//genome *********************************
	short cnt = 3; short command;

BUGB:
	command = getGenComm(n_genomeCounter);
	if (--cnt = 0) goto BUGE;

	if (command == 0) {
		n_energy += (*m_world).photosynthes(this);
		n_genomeCounter = (n_genomeCounter + 1)%m_genome.size();
		goto BUGE;
	}
	if (command == 1) {
		move((getGenComm(n_genomeCounter+1) + m_size) / 10);
		n_genomeCounter = (n_genomeCounter + 1) % m_genome.size();
		goto BUGE;
	}
	if (command == 2) {
		rotate((getGenComm(n_genomeCounter + 1)) / 100);
		n_genomeCounter = (n_genomeCounter + 1) % m_genome.size();
		goto BUGE;
	}
	n_genomeCounter = (n_genomeCounter + 1) % m_genome.size();
	goto BUGE;
BUGE:

	//energy **********************************
	n_energy -= pow(m_age / 100 + 1, 2);

	if (n_energy > 300){
		clone(100, m_size);
	}

	// edit data *****************************************************
	worldMtx.lock();
	m_age++;
	m_pos_x = n_pos_x; m_pos_y = n_pos_y;
	m_rot_x = n_rot_x; m_rot_y = n_rot_y;
	m_vec_x = n_vec_x; m_vec_y = n_vec_y;
	m_energy = n_energy;
	m_genomeCounter = n_genomeCounter;
	worldMtx.unlock();
}


void Beetle::clone(int energy, float range) {
	n_energy -= energy;
	if (n_energy < energy)
		return;
	std::vector<short> tmp_genome;

	float nx = n_rot_x, ny = n_rot_y;
	float dx, dy; int r = rand() % 8;
	for (int i = 0; i < 4; i++) {
		r = rand() % 8;
		//std::cout << r << "\n";
		dx = nx * cos(r * 45) + ny * sin(r * 45);
		dy = nx * sin(r * 45) + ny * cos(r * 45);
		if (!(*m_world).checkPlace(n_pos_x + dx * range, n_pos_y + dy * range))
			continue;
		goto BCC;
	}
	goto BCE;

BCC:
	//-----cloning-----
	tmp_genome = m_genome;
	
	if (rand() % 3 != 0) {
		tmp_genome[rand() % tmp_genome.size()] = rand() % genomeCommands.size();
	}

	(*m_world).addBeetle(
		n_pos_x + dx * range,
		n_pos_y + dy * range,
		n_rot_x,
		n_rot_y,
		energy,
		tmp_genome);

BCE:
	return;
}
void Beetle::move(float force){
	n_energy -= force;
	n_vec_x += n_rot_x * force;
	n_vec_y += n_rot_y * force;
}
void Beetle::rotate(float angle){
	float nx = n_rot_x, ny = n_rot_y;
	n_rot_x = nx * cos(angle) + ny * sin(angle);
	n_rot_y = nx * sin(angle) + ny * cos(angle);
}

float Beetle::getDistanceTo(float x, float y) {
	return(sqrt(pow(m_pos_x - x, 2) + pow(m_pos_y - y, 2)));
}

short Beetle::getGenComm(short ctr) {
	return m_genome[ctr % m_genome.size()];
}
float Beetle::getCordX(float x){
	if (x < 0)
		return ((*m_world).m_size_x + x);
	if (x > WORLD_SIZE_X)
		return (x - WORLD_SIZE_X);
	return (x);
}