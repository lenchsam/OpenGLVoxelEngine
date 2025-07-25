#include "ChunkManager.h"
#include "Chunk.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

ChunkManager::~ChunkManager()
{
    for (Chunk* chunk : m_chunks) {
        delete chunk;
    }
    m_chunks.clear();
}

void ChunkManager::AddChunk(Chunk* chunk)
{
    m_chunks.push_back(chunk);
}

void ChunkManager::UnloadChunk(Chunk* chunk)
{
    m_chunks.erase(std::remove(m_chunks.begin(), m_chunks.end(), chunk), m_chunks.end());
    delete chunk;
}

void ChunkManager::RenderChunks(Shader& shader)  
{  
    glm::vec3 cameraPos = m_camera->Position;  
    int chunkX = static_cast<int>(floor(cameraPos.x / Chunk::CHUNK_WIDTH));  
    int chunkZ = static_cast<int>(floor(cameraPos.z / Chunk::CHUNK_DEPTH));  

    Chunk* currentChunk = FindChunk(chunkX, chunkZ);  

    for (int offsetZ = -RENDER_DISTANCE; offsetZ <= RENDER_DISTANCE; ++offsetZ) {  
        for (int offsetX = -RENDER_DISTANCE; offsetX <= RENDER_DISTANCE; ++offsetX) {  
            int ChunkX = chunkX + offsetX;  
            int ChunkZ = chunkZ + offsetZ;  

            if (!FindChunk(ChunkX, ChunkZ)) {  
                Chunk* newChunk = new Chunk(ChunkX, ChunkZ);  
                newChunk->GenerateMesh(m_noise);
                AddChunk(newChunk);  
            }  
        }  
    }  

	//identify and remove chunks that are too far away
    for (Chunk* chunk : m_chunks) {
        glm::ivec2 pos = chunk->getPosition();
        int distanceX = abs(pos.x - chunkX);
        int distanceZ = abs(pos.y - chunkZ);

        if (distanceX > RENDER_DISTANCE || distanceZ > RENDER_DISTANCE) {
            UnloadChunk(chunk);
        }
    }

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


