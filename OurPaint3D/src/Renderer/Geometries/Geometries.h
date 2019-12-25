#pragma once

#include "Renderer/Geometries/Geometry.h"

class Cube : public Geometry
{
public:
	Cube();
	std::vector<float>* GetVerticies() override;
};


class Prismatic : public Geometry
{
public:
	Prismatic();
	std::vector<float>* GetVerticies() override;
};

class Pyramid : public Geometry
{
public:
	Pyramid();
	std::vector<float>* GetVerticies() override;
};

class Sphere : public Geometry
{
public:
	Sphere();
	std::vector<float>* GetVerticies() override;
};
