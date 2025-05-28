#pragma once
#include "BlockTypes.h"
#include "Block.h"
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	void GenerateMesh(FastNoiseLite* noise);
	
	glm::vec3 getPosition();

	void BuildRenderData();
	void DrawInstanced(Shader& shader, GLuint sharedBlockMeshVAO);

	static const unsigned int CHUNK_WIDTH = 16;
	static const unsigned int CHUNK_HEIGHT = 16;
	static const unsigned int CHUNK_DEPTH = 16;

private:

	int m_x, m_y, m_z; // Position of the chunk in the world
	Block m_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];

	GLuint m_instanceVBO = 0; // VBO for this chunks instance positions
	std::vector<glm::vec3> m_instancePositions; // positions of blocks to render local to chunk
	bool m_needsRebuild = true;

};

