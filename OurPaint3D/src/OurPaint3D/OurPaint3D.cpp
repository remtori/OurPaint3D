#include "pch.h"
#include "OurPaint3D/OurPaint3D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

OurPaint3D::OurPaint3D()
	: Application("OurPaint3D"), m_Color(0.0f)
{	
}

OurPaint3D::~OurPaint3D()
{
}

void OurPaint3D::OnUpdate(double dt)
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400.0f, m_Data.Height), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Menu"))
	{
		ImGui::ColorEdit3("Color", glm::value_ptr(m_Color));
	}
	ImGui::End();
}
