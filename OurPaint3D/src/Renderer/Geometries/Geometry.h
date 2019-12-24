#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Renderer/Texture.h"

class Geometry
{
public:
	Texture* texture;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec4 color;
	glm::mat4 transformationMatrix;
	std::vector<float> verticies;

	Geometry();
	void ReCalcTransform();
};

