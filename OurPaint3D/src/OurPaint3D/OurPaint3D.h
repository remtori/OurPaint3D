#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Core/Application.h"
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
	void HandleCamera(double dt);
	void OnResize(int width, int height) override;
private:
	std::vector<Texture*> m_Textures;
	std::vector<Geometry*> m_Geometries;
	Geometry* currentGeom;
	Camera* m_Camera;
private:
	// Camera options
	float CamSpeed;
	float CamSensitivity;

	// World options
	bool IsCenterVisible;
	bool IsGridVisible;
	bool IsHightlightEnable;

	// ImGui
	float menuWidth;
};