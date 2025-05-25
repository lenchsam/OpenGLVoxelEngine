#pragma once
#include "BlockTypes.h"
#include "Block.h"

class Shader;
class Chunk
{
public:
	Chunk();
	~Chunk();

	//void Update(float deltaTime);
	BlockID getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, BlockID blockType);

	void draw(Shader& shader, unsigned int VAO);
private:
	static const unsigned int CHUNK_WIDTH = 16;
	static const unsigned int CHUNK_HEIGHT = 16;
	static const unsigned int CHUNK_DEPTH = 16;

	Block m_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];

};

