#pragma once
#include <vector>

class Chunk;
class Shader;
class Camera;
class FastNoiseLite;
class ChunkManager
{
public:
	ChunkManager(FastNoiseLite* noise, Camera* camera) : m_noise(noise), m_camera(camera) {}
	~ChunkManager();
	void AddChunk(Chunk* chunk);
	void UnloadChunk(Chunk* chunk);
	void RenderChunks(Shader& shader);
	Chunk* FindChunk(int x, int z);
private:
	
	static const unsigned int CHUNK_WIDTH = 16;
	static const unsigned int CHUNK_HEIGHT = 64;
	static const unsigned int CHUNK_DEPTH = 16;
	static const int RENDER_DISTANCE = 5;
	std::vector<Chunk*> m_chunks;

	FastNoiseLite* m_noise;
	Camera* m_camera;
};

