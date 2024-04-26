#ifndef b_beetle_h
#define b_beetle_h

#include <vector>

class World;

class Beetle {
public:
	Beetle(float x, float y);
	Beetle(float x, float y, float rx, float ry);
	Beetle(float x, float y, float rx, float ry, std::vector<int> i_genome);
	Beetle(float x, float y, float rx, float ry, std::vector<int> i_genome, World* world);
	void setWorld(World* world);

	int update();

	void clone(int energy, float range);
	void clone(int energy, float range, bool kill);

	float m_pos_x; float m_pos_y; float m_rot_x; float m_rot_y;

	std::vector<int> m_genome;
	int m_genomeCounter{ 0 };

	float m_energy{ 100 };
	int m_age{ 0 };
	int m_size{ 10 };
private:
	int getGenComm(int ctr);
	World* m_world;
};

#endif