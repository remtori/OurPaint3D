#pragma once

#include "Core/Application.h";

#include <glm/glm.hpp>

class OurPaint3D : public Application
{
public:
	OurPaint3D();
	~OurPaint3D();

	void OnUpdate(double dt) override;
	void OnImGuiRender() override;
private:
	glm::vec3 m_Color;
};