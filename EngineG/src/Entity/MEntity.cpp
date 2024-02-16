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
