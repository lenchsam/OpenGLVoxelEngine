#pragma once

#include <vector>
class Chunk;
class Shader;
class ChunkManager
{
public:
	void AddChunk(Chunk* chunk);
	void UnloadChunk(Chunk* chunk);
	void RenderChunks(Shader& shader);
	Chunk* FindChunk(int x, int z);
private:
	
	static const unsigned int CHUNK_WIDTH = 16;
	static const unsigned int CHUNK_HEIGHT = 64;
	static const unsigned int CHUNK_DEPTH = 16;
	std::vector<Chunk*> m_chunks;
};

