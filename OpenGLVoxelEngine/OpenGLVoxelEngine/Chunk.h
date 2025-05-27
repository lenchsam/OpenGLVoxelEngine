#pragma once
#include "BlockTypes.h"
#include "Block.h"

#include <glm/glm.hpp>

class Shader;
class FastNoiseLite;
class Chunk
{
public:
	Chunk(int _x, int _y, int _z);
	~Chunk();

	//void Update(float deltaTime);
	BlockID getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, BlockID blockType);

	void draw(Shader& shader, unsigned int VAO);
	void GenerateMesh(FastNoiseLite* noise);
	
	glm::vec3 getPosition();

	static const unsigned int CHUNK_WIDTH = 16;
	static const unsigned int CHUNK_HEIGHT = 16;
	static const unsigned int CHUNK_DEPTH = 16;

private:

	int m_x, m_y, m_z; // Position of the chunk in the world
	Block m_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];

};

