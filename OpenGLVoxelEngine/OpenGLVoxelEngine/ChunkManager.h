#pragma once

#include <vector>
class Chunk;
class Shader;
class ChunkManager
{
public:
	void Update(float dt);

	void AddChunk(Chunk* chunk);
	void RemoveChunk(Chunk* chunk);
	void RenderChunks(Shader& shader, unsigned int VAO);
private:
	std::vector<Chunk*> m_chunks;
	void ClearChunks();
	void UpdateChunks(float dt);

};

