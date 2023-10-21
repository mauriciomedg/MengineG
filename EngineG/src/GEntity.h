#pragma once

#include "Inputs.h"
#include <iostream>

class GEntity
{
	
public:

	GEntity() {}

	void bindAxis()
	{
		Inputs::get().addBinding("MoveForward", & GEntity::moveForward, this);
	}
	
	void moveForward(float val)
	{
		std::cout << "moveeeeeeeee " << val << std::endl;
	}

	void init()
	{
		bindAxis();
	}
	
};