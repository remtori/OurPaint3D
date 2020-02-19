#include "pch.h"
#include "Renderer/Geometries/Geometry.h"

#include <glm/gtc/matrix_transform.hpp>

Geometry::Geometry(EObject type, const char* inName)
	: texture(nullptr), color(1.0f, 1.0f, 1.0f, 1.0f),
	position(0.0f), rotation(0.0f), scale(1.0f),
	transformationMatrix(1.0f), type(type)
{
	strcpy(name, inName);
}

void Geometry::ReCalcTransform()
{
	transformationMatrix =
		glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1, 0, 0 }) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0, 1, 0 }) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0, 0, 1 }) *
		glm::scale(glm::mat4(1.0f), scale);
}
