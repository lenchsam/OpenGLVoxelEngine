#pragma once
class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	
	static void LoadTexture(const char* filePath);
	unsigned int GetTextureID(unsigned int blockType) const;
};

