#pragma once

#include "MeshComponent.h"

class MeshFactory
{
public:
	MeshFactory();
	~MeshFactory();

	std::unique_ptr<MeshComponent> CreateQuadGrid( unsigned int resolution, float size = 0.25f);

private:

};