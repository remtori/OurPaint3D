#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Renderer/Texture.h"

class Geometry
{
public:
	enum EObject {
		Origin, Grid,
		Cube, Prismatic, Pyramid, Sphere
	};

public:
	EObject type;
	char name[128];
	Texture* texture;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec4 color;
	glm::mat4 transformationMatrix;

	Geometry(EObject type, const char* name = "Basic Geometry");
	void ReCalcTransform();

	virtual std::vector<float>* GetVerticies() = 0;
};
