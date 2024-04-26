#include <iostream>
#include "Beetl.hpp"
#include "World.hpp"


extern int WORLD_SIZE_X;
extern int WORLD_SIZE_Y;

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
void Beetle::setWorld(World* world) {
	m_world = world;
}
void Beetle::setID(int id) {
	m_id = id;
}

int Beetle::update()
{
	int command = m_genome[m_genomeCounter];
	int res = 0;

	BC:
	if (command > m_genome.size()) {
		m_genomeCounter = command % m_genome.size();

		goto HH;
	}
	if (command >= 0 && command <= 3) {
		m_energy += (*m_world).photosynthes(this);
		m_genomeCounter++;

		goto HH;
	}
	if (command >= 4 && command <= 7) {
		int max_distance = m_size + getGenComm(m_genomeCounter + 1) / 20; int distance = 0;
		int move_x = 0; int move_y = 0;


		if ((*m_world).checkPlace(this, m_pos_x + m_rot_x * max_distance, m_pos_y + m_rot_y * max_distance))
			distance = max_distance;
		else{
			command = 13;
			goto BC;
		}

		m_pos_x += m_rot_x * distance;
		move_x = m_rot_x * distance;
		m_pos_y += m_rot_y * distance;
		move_y = m_rot_y * distance;

		m_energy -= sqrt(pow(move_x, 2) + pow(move_y, 2));
		m_genomeCounter += 2;

		goto HH;
	}
	if (command >= 8 && command <= 11) {
		int angle = getGenComm(m_genomeCounter + 1) - m_genome.size();
		
		int nx = m_rot_x * cos(angle) - m_rot_y * sin(angle);
		int ny = m_rot_x * sin(angle) + m_rot_y * cos(angle);

		m_rot_x = nx;
		m_rot_y = ny;
		m_genomeCounter+=2;

		goto HH;
	}
	if (command >= 12 && command <= 12) {
		clone(getGenComm(m_genomeCounter + 1), m_size);
		m_genomeCounter+=3;

		goto HH;
	}
	if (command >= 13 && command <= 15) {
		Beetle* beetle = (*m_world).findNearest(m_pos_x, m_pos_y, this);
		if (pow(m_pos_x - (*beetle).m_pos_x, 2) + pow(m_pos_x - (*beetle).m_pos_x, 2) < pow((m_size + (*beetle).m_size), 2)){
			(*beetle).m_energy -= 50 + getGenComm(m_genomeCounter + 1) /10;
			m_energy += 50 + getGenComm(m_genomeCounter + 1) / 10;
		}
		m_genomeCounter += 1;
		goto HH;
	}
	if (command == 21) {
		if (m_pos_y / WORLD_SIZE_Y > 0.5) {
			m_energy -= 2;
			command = getGenComm(m_genomeCounter + 1);
			goto BC;
		}
		else {
			m_energy -= 2;
			command = getGenComm(m_genomeCounter + 2);
			goto BC;
		}

		m_energy -= 2;
		goto HH;
	}
	m_genomeCounter = command % m_genome.size();
	HH:;

	m_genomeCounter %= m_genome.size();

	m_pos_x = int(m_pos_x) % (*m_world).m_size_x;

	m_energy -= pow( m_age/100, 2);
	if (m_energy > 510)
		m_energy = 510;
	m_age++;

	if (m_energy < 1)
		(*m_world).killBeetle(this);
	if (m_energy > 200)
		clone(100, m_size);

	return 1;
}

void Beetle::clone(int energy, float range) {

	if (energy > m_energy) return;

	float nx, ny;
	for (int angle = -2; angle < 3; angle++){
		
		nx = m_rot_x * cos(angle) - m_rot_y * sin(angle);
		ny = m_rot_x * sin(angle) + m_rot_y * cos(angle);

		if ((*m_world).checkPlace(this, m_pos_x - (nx * range), m_pos_y - (ny * range)))
			goto HH2;
	}
	//-----if not cloning------
	if (rand() % 4 == 0) {
		m_genome[rand() % m_genome.size()] += (rand() % 3) % m_genome.size();
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
		m_pos_x - nx * range,
		m_pos_y - ny * range,
		nx,
		ny,
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
	int octr = ctr % m_genome.size();
	m_genomeCounter %= m_genome.size();
	return m_genome[octr];
}