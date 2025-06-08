#include "Chunk.h"

#include "FastNoiseLite.h"

#include "BlockTypes.h"
#include "Block.h"
#include "Shader.h"

constexpr float faceVertices[] = {
    //back
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    //front
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    0.0f, 1.0f, 1.0f,  0.0f, 1.0f,

    //left
    0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

    //right
    1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    //bottom
    0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

    //top
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  1.0f, 0.0f
};
Chunk::Chunk(int _x, int _z) : m_x(_x), m_z(_z)
{
	//initialize the chunk with air blocks
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
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }
}

BlockID Chunk::getBlock(int x, int y, int z) const {
	// out of bounds check
    if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH) {
        return BlockID::AIR;
    }

    if (!m_blocks[x][y][z].IsActive()) {
        return BlockID::AIR;
    }

    return m_blocks[x][y][z].GetBlockType();
}

bool Chunk::isSolid(int x, int y, int z) const {
    return getBlock(x, y, z) != BlockID::AIR;
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

glm::ivec2 Chunk::getPosition()
{
    return glm::ivec2(m_x, m_z);
}

void Chunk::Draw(Shader& shader) {
    if (m_needsRebuild) {
        BuildMesh();
    }

    if (m_VAO == 0 || m_meshVertices.empty()) {
        return; // Nothing to draw
    }

    glm::mat4 chunkModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
        static_cast<float>(m_x * CHUNK_WIDTH),
        0.0f, //not doing vertical chunks
        static_cast<float>(m_z * CHUNK_DEPTH)
    ));
    shader.setMat4("model", chunkModelMatrix);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_meshVertices.size() / 5));
    glBindVertexArray(0);
}

void Chunk::BuildMesh()
{
    m_meshVertices.clear();

    for (int y = 0; y < CHUNK_HEIGHT; ++y) {
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                if (!isSolid(x, y, z)) {
                    continue; //skip air blocks
                }

                // The offset for each face is its index (0-5) x 30 (6 vertices * 5 floats).

                //check each of the 6 faces
                //back
                if (!isSolid(x, y, z - 1)) {
                    for (int i = 0; i < 6; ++i) { // 6 vertices per face
                        m_meshVertices.push_back(faceVertices[0 * 30 + i * 5 + 0] + x);
                        m_meshVertices.push_back(faceVertices[0 * 30 + i * 5 + 1] + y);
                        m_meshVertices.push_back(faceVertices[0 * 30 + i * 5 + 2] + z);
                        m_meshVertices.push_back(faceVertices[0 * 30 + i * 5 + 3]);
                        m_meshVertices.push_back(faceVertices[0 * 30 + i * 5 + 4]);
                    }
                }
                //front
                if (!isSolid(x, y, z + 1)) {
                    for (int i = 0; i < 6; ++i) {
                        m_meshVertices.push_back(faceVertices[1 * 30 + i * 5 + 0] + x);
                        m_meshVertices.push_back(faceVertices[1 * 30 + i * 5 + 1] + y);
                        m_meshVertices.push_back(faceVertices[1 * 30 + i * 5 + 2] + z);
                        m_meshVertices.push_back(faceVertices[1 * 30 + i * 5 + 3]);
                        m_meshVertices.push_back(faceVertices[1 * 30 + i * 5 + 4]);
                    }
                }
                //left
                if (!isSolid(x - 1, y, z)) {
                    for (int i = 0; i < 6; ++i) {
                        m_meshVertices.push_back(faceVertices[2 * 30 + i * 5 + 0] + x);
                        m_meshVertices.push_back(faceVertices[2 * 30 + i * 5 + 1] + y);
                        m_meshVertices.push_back(faceVertices[2 * 30 + i * 5 + 2] + z);
                        m_meshVertices.push_back(faceVertices[2 * 30 + i * 5 + 3]);
                        m_meshVertices.push_back(faceVertices[2 * 30 + i * 5 + 4]);
                    }
                }
                //right
                if (!isSolid(x + 1, y, z)) {
                    for (int i = 0; i < 6; ++i) {
                        m_meshVertices.push_back(faceVertices[3 * 30 + i * 5 + 0] + x);
                        m_meshVertices.push_back(faceVertices[3 * 30 + i * 5 + 1] + y);
                        m_meshVertices.push_back(faceVertices[3 * 30 + i * 5 + 2] + z);
                        m_meshVertices.push_back(faceVertices[3 * 30 + i * 5 + 3]);
                        m_meshVertices.push_back(faceVertices[3 * 30 + i * 5 + 4]);
                    }
                }
                //bottom
                if (!isSolid(x, y - 1, z)) {
                    for (int i = 0; i < 6; ++i) {
                        m_meshVertices.push_back(faceVertices[4 * 30 + i * 5 + 0] + x);
                        m_meshVertices.push_back(faceVertices[4 * 30 + i * 5 + 1] + y);
                        m_meshVertices.push_back(faceVertices[4 * 30 + i * 5 + 2] + z);
                        m_meshVertices.push_back(faceVertices[4 * 30 + i * 5 + 3]);
                        m_meshVertices.push_back(faceVertices[4 * 30 + i * 5 + 4]);
                    }
                }
                //top
                if (!isSolid(x, y + 1, z)) {
                    for (int i = 0; i < 6; ++i) {
                        m_meshVertices.push_back(faceVertices[5 * 30 + i * 5 + 0] + x);
                        m_meshVertices.push_back(faceVertices[5 * 30 + i * 5 + 1] + y);
                        m_meshVertices.push_back(faceVertices[5 * 30 + i * 5 + 2] + z);
                        m_meshVertices.push_back(faceVertices[5 * 30 + i * 5 + 3]);
                        m_meshVertices.push_back(faceVertices[5 * 30 + i * 5 + 4]);
                    }
                }
            }
        }
    }

    if (m_meshVertices.empty()) {
        m_needsRebuild = false;
        return;
    }

    //create or update GPU buffers
    if (m_VAO == 0) {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
    }

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_meshVertices.size() * sizeof(float), m_meshVertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    m_needsRebuild = false;
}
