#pragma once

#include <vector>
class Chunk;
class Shader;
class ChunkManager
{
public:
	void AddChunk(Chunk* chunk);
	void RenderChunks(Shader& shader);
private:
	
	std::vector<Chunk*> m_chunks;
};

