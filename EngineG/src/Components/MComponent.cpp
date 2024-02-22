#include "MComponent.h"
#include "../Entity/MEntity.h"

using namespace MG;

MComponent::MComponent()
{
}

MComponent::~MComponent()
{
}

void MComponent::release()
{
	m_entity->removeComponent(m_id);
}
