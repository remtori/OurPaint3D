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

}

// TODO: Use dockspace to split the geometry selection and editing 50/50
void OurPaint3D::OnImGuiRender()
{		
	static float menuWidth = 380.0f;

	static glm::vec3 translation(0.0f), scale(0.0f), rotation(0.0f);

	ImGui::ShowDemoWindow();
	
	ImGui::SetNextWindowPos(ImVec2(m_WinInfo.Width - menuWidth, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(menuWidth, m_WinInfo.Height));	

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 0.0f));
	ImGui::Begin("Menu", 0, 
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus
	);
	ImGui::PopStyleVar();		
	
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Translation   ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Translation", glm::value_ptr(translation));

		ImGui::Text("Scale         ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Scale", glm::value_ptr(scale));

		ImGui::Text("Rotation      ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation));		
	}		

	if (ImGui::CollapsingHeader("Geometry List"))
	{
		const char* list[] = {
			"Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"
		};
		static int current = 1;		
		ImGui::PushItemWidth(menuWidth);
		ImGui::ListBox("##Geometry List", &current, list, IM_ARRAYSIZE(list), 20);
		ImGui::PopItemWidth();
	}

	ImGui::End();
}
