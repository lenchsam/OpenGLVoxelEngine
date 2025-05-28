#include "Chunk.h"

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

void Chunk::GenerateMesh(FastNoiseLite* noise)
{
    const int baseSurfaceLevel = CHUNK_HEIGHT / 3; // where the average ground level will be
    const float terrainAmplitude = (float)CHUNK_HEIGHT / 4.0f;

    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int z = 0; z < CHUNK_DEPTH; ++z) {
			//chunk offset needed because noise is generated in world space, will be smooth across chunks.
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

    m_needsRebuild = true;
}

void Chunk::BuildRenderData() {
    if (!m_needsRebuild) return;

    m_instancePositions.clear();
    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int y = 0; y < CHUNK_HEIGHT; ++y) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                const Block& block = m_blocks[x][y][z];
                if (block.IsActive() && block.GetBlockType() != BlockID::AIR) {
                    // TODO: implement face culling here later for optimization.
                    m_instancePositions.push_back(glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)));
                }
            }
        }
    }

    if (m_instanceVBO == 0 && !m_instancePositions.empty()) {
        glGenBuffers(1, &m_instanceVBO);
    }

    if (!m_instancePositions.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, m_instancePositions.size() * sizeof(glm::vec3), m_instancePositions.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else if (m_instanceVBO != 0) { // No instances, but VBO exists, clean it up or leave empty
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    m_needsRebuild = false;
}

void Chunk::DrawInstanced(Shader& shader, GLuint VAO) {
    if (m_needsRebuild) {
        BuildRenderData();
    }

    if (m_instancePositions.empty()) {
        return; 
    }

    // Set the model matrix for the entire chunk
    glm::mat4 chunkModelMatrix = glm::mat4(1.0f);
    chunkModelMatrix = glm::translate(chunkModelMatrix, glm::vec3(
        static_cast<float>(m_x * CHUNK_WIDTH),
        static_cast<float>(m_y * CHUNK_HEIGHT),
        static_cast<float>(m_z * CHUNK_DEPTH)
    ));
    shader.setMat4("model", chunkModelMatrix);

    glBindVertexArray(VAO);

    // Enable and configure the per-instance position attribute (aInstancePos)
    // This uses the chunk's specific m_instanceVBO
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glEnableVertexAttribArray(2); // Location 2 for aInstancePos
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(2, 1);

    // Draw all instances of the block for this chunk
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, static_cast<GLsizei>(m_instancePositions.size()));

    glVertexAttribDivisor(2, 0);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
