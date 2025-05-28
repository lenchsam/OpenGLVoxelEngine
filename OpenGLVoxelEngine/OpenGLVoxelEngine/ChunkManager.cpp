#include "ChunkManager.h"
#include "Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

void ChunkManager::AddChunk(Chunk* chunk)
{
	m_chunks.push_back(chunk);
}

void ChunkManager::RenderChunks(Shader& shader, unsigned int VAO)
{
	for(Chunk* chunk : m_chunks)
	{
		chunk->DrawInstanced(shader, VAO);
	}
}


