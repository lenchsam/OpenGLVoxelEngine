#include "Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "FastNoiseLite.h"

#include "BlockTypes.h"
#include "Block.h"
#include "Shader.h"


Chunk::Chunk(int _x, int _y, int _z) : m_x(_x), m_y(_y), m_z(_z)
{
	// Initialize the chunk with air blocks
	for (int x = 0; x < CHUNK_WIDTH; ++x) {
		for (int y = 0; y < CHUNK_HEIGHT; ++y) {
			for (int z = 0; z < CHUNK_DEPTH; ++z) {
				m_blocks[x][y][z] = Block();
			}
		}
	}
}

Chunk::~Chunk()
{
}

BlockID Chunk::getBlock(int x, int y, int z) const {
	//returns air as placeholder
	return BlockID::AIR;
}

void Chunk::setBlock(int x, int y, int z, BlockID blockType)
{
    // if coords are out of bounds
	if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH) { return; }
	m_blocks[x][y][z].SetActive(true);
	m_blocks[x][y][z].SetBlockType(blockType);
}

void Chunk::draw(Shader& shader, unsigned int VAO)
{
	//TO DO: OPTIMISE! 1 VBO TO DRAW ALL BLOCKS IN CHUNK
    glBindVertexArray(VAO);

    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int y = 0; y < CHUNK_HEIGHT; ++y) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                const Block& block = m_blocks[x][y][z];
                if (!block.IsActive() || block.GetBlockType() == BlockID::AIR) {
                    continue;
                }

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x + (m_x * CHUNK_WIDTH), y + (m_y * CHUNK_HEIGHT), z + (m_z * CHUNK_DEPTH)));
                shader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }

    glBindVertexArray(0);
}

void Chunk::GenerateMesh(FastNoiseLite* noise)
{
    const int baseSurfaceLevel = CHUNK_HEIGHT / 3; // where the average ground level will be
    const float terrainAmplitude = (float)CHUNK_HEIGHT / 4.0f;

    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int z = 0; z < CHUNK_DEPTH; ++z) {
            // add chunkWorldOffsetX and chunkWorldOffsetZ here.
            float noiseVal = noise->GetNoise((float)x + (m_x * CHUNK_WIDTH), (float)z + (m_z * CHUNK_DEPTH));

            int surfaceY = baseSurfaceLevel + (int)(noiseVal * terrainAmplitude);

            // clamp surfaceY to be within chunk boundaries
            if (surfaceY < 0) surfaceY = 0;
            if (surfaceY >= CHUNK_HEIGHT) surfaceY = CHUNK_HEIGHT - 1;

            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                if (y < surfaceY) {
                    // below surface
                    setBlock(x, y, z, BlockID::DIRT);
                }
                else if (y == surfaceY) {
                    // surface block
                    setBlock(x, y, z, BlockID::GRASS);
                }
                else {
                    //is air block
                }
            }
        }
    }
}
