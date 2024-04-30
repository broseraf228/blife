#include <iostream>
#include <mutex>
#include "Beetl.hpp"
#include "World.hpp"


extern int WORLD_SIZE_X;
extern int WORLD_SIZE_Y;
extern std::mutex worldMtx;

std::vector<short> defaultGenome(96, 0);
std::vector<short> genomeCommands{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,20,21,22,23};

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
	if (m_energy <= 0)
		(*m_world).killBeetle(this);
	World& world = (*m_world);
	float Npx = m_pos_x, Npy = m_pos_y, Nrx = m_rot_x, Nry = m_rot_y, Nvx = m_vec_x, Nvy = m_vec_y, Nen = m_energy;
	//beetle work
	Npx = getCordX(Npx + Nvx);
	if (Npy + Nvy < 0 || Npy + Nvy > WORLD_SIZE_Y)
		Nvy = Nvy * -1;
	Npy += Nvy;

	Nen += world.photosynthes(this);
	Nen -= pow(m_age/100 + 1, 2);

	world.findNearest(Npx + Nrx, Npy + Nry, this);

	// edit data
	worldMtx.lock();
	m_age++;
	m_pos_x = Npx; m_pos_y = Npy;
	m_rot_x = Nrx; m_rot_y = Nry;
	m_vec_x = Nvx; m_vec_y = Nvy;
	m_energy = Nen;
	worldMtx.unlock();
}


void Beetle::clone(int energy, float range) {

	if (energy > m_energy) return;

	float nx, ny;
	for (int angle = -2; angle < 3; angle++){
		
		nx = m_rot_x * cos(angle) - m_rot_y * sin(angle);
		ny = m_rot_x * sin(angle) + m_rot_y * cos(angle);

		if ((*m_world).checkPlace(this, (*m_world).getCordX(m_pos_x - (nx * range)), m_pos_y - (ny * range)))
			goto HH2;
	}
	//-----if not cloning------
	if (rand() % 4 == 0) {
		int rn = rand() % m_genome.size();
		m_genome[rn] = (m_genome[rn] + (rand() % 4) -2) % m_genome.size();
	}
	m_energy -= energy / 4;

	return;
HH2:
	//------if cloning------

	//mutations--->>>
	auto n_genome = m_genome;

	if(rand() % 2 == 0)
		if(rand() % 4 != 0)
			n_genome[rand() % n_genome.size()] = genomeCommands[rand() % genomeCommands.size()];
		else
			n_genome[rand() % n_genome.size()] += fabs(( rand() % m_genome.size() + m_genome.size()-1));

	//cloning
	

	(*m_world).addBeetle(
	    (*m_world).getCordX( m_pos_x - nx * range),
		m_pos_y - ny * range,
		m_rot_x,
		m_rot_y,
		energy,
		n_genome);
	m_energy -= energy;

	
}
void Beetle::clone(int energy, float range, bool kill) {

	if (energy > m_energy) return;

	float nx = 0; float ny = 0;
	for (int i = 0; i < 8; i++) {
		nx = m_rot_x * cos(90) - m_rot_y * sin(90);
		ny = m_rot_x * sin(90) + m_rot_y * cos(90);
		m_rot_x = nx; m_rot_y = ny;
		if (!(*m_world).checkPlace(this, m_pos_x + nx * range, m_pos_y + ny * range))
			continue;
		else {
			//mutations--->>>
			auto n_genome = m_genome;

			if (rand() % 2 == 0)
				n_genome[rand() % n_genome.size()] = genomeCommands[rand() % genomeCommands.size()];
			else
				n_genome[rand() % n_genome.size()] = rand() % m_genome.size() * 2;
			///

			(*m_world).addBeetle(
				m_pos_x + nx * range * 0.9,
				m_pos_y + ny * range * 0.9,
				m_rot_x,
				m_rot_y,
				energy,
				n_genome);
			m_energy -= energy;
			return;
		}
	}

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