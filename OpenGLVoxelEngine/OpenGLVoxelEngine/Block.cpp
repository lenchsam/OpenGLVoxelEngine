#include "Block.h"
#include "BlockTypes.h"

Block::Block()
{
	m_isActive = true; // default to active
	m_blockType = BlockID::AIR; // default block type
}

Block::~Block()
{

}

bool Block::IsActive() const
{
	return m_isActive;
}

void Block::SetActive(bool active)
{
	m_isActive = active;
}

BlockID Block::GetBlockType() const
{
	return m_blockType;
}

void Block::SetBlockType(BlockID blockType)
{
	m_blockType = blockType;
}
