#ifndef b_beetle_h
#define b_beetle_h

#include <vector>

class World;

class Beetle {
public:
	Beetle(float x, float y);
	Beetle(float x, float y, float rx, float ry);
	Beetle(float x, float y, float rx, float ry, std::vector<short> i_genome);
	Beetle(float x, float y, float rx, float ry, std::vector<short> i_genome, World* world);
	Beetle(float x, float y, float rx, float ry, float vx, float vy, std::vector<short> i_genome, World* world);
	void setWorld(World* world);
	void setID(int id);

	void update();

	void moveByVec();

	void clone(int energy, float range);
	void clone(int energy, float range, bool kill);

	float m_pos_x, m_pos_y, m_rot_x, m_rot_y, m_vec_x, m_vec_y;

	std::vector<short> m_genome;
	int m_genomeCounter{ 0 };

	float m_energy{ 100 };
	int m_age{ 0 };
	int m_size{ 10 };
	int m_id{ 0 };
private:
	float getCordX(float x);
	short getGenComm(short ctr);
	World* m_world;
};

#endif