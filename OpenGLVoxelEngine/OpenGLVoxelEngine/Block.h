#pragma once

#include "BlockTypes.h"
class Block
{
public:
	Block();
	~Block();
	bool IsActive() const;
	void SetActive(bool active);

	BlockID GetBlockType() const;
	void SetBlockType(BlockID blockType);

private:
	bool m_isActive;
	BlockID m_blockType;
	// add methods to manipulate block properties, such as type, position, etc.

};

