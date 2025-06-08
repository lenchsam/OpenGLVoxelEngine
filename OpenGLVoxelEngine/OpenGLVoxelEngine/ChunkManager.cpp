#include "ChunkManager.h"
#include "Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

void ChunkManager::AddChunk(Chunk* chunk)
{
    m_chunks.push_back(chunk);
}

void ChunkManager::UnloadChunk(Chunk* chunk)
{
    m_chunks.erase(std::remove(m_chunks.begin(), m_chunks.end(), chunk), m_chunks.end());
}

void ChunkManager::RenderChunks(Shader& shader)
{
    // get camera position
    // get chunk that camera is in
	// calculate which chunks need to be rendered based on camera position

    for (Chunk* chunk : m_chunks) {
        chunk->Draw(shader);
    }
}

Chunk* ChunkManager::FindChunk(int x, int z)
{
    //just loop through chunk vector and find matching chunk position
    for (Chunk* chunk : m_chunks) {
        glm::ivec2 pos = chunk->getPosition();
        if (pos.x == x && pos.y == z) {
            return chunk;
        }
    }
    return nullptr;
}


