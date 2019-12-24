#pragma once

#include <glm/glm.hpp>

#include "Core/Application.h";
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Renderer/Geometries/Geometry.h"

class OurPaint3D : public Application
{
public:
	OurPaint3D();
	~OurPaint3D();

	void OnUpdate(double dt) override;
	void OnImGuiRender() override;
protected:
	void OnResize(int width, int height) override;

private:
	Texture* m_Texture;
	Camera* m_Camera;

	// Camera options
	float CamSpeed;
	float CamSensitivity;
};