#include "MEntity.h"
#include "MEntitySystem.h"

using namespace MG;

MEntity::MEntity()
{
}

MEntity::~MEntity()
{
}

void MEntity::release()
{
	m_entitySystem->removeEntity(this);
}

MEntitySystem* MG::MEntity::getEntitySystem()
{
	return m_entitySystem;
}
