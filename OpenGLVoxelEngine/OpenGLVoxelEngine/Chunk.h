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
	Chunk(int _x, int _z);
	~Chunk();

	//void Update(float deltaTime);
	BlockID getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, BlockID blockType);

	void GenerateMesh(FastNoiseLite* noise);
	
	glm::ivec2 getPosition();

	void Draw(Shader& shader);

	static const unsigned int CHUNK_WIDTH = 16;
	static const unsigned int CHUNK_HEIGHT = 128;
	static const unsigned int CHUNK_DEPTH = 16;

private:
	void BuildMesh();
	bool isSolid(int x, int y, int z) const;

	int m_x, m_z; // Position of the chunk in the world
	Block m_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];

    std::vector<float> m_meshVertices;
    GLuint m_VAO = 0, m_VBO = 0;

    bool m_needsRebuild = true;

};

