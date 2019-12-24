#include "pch.h"
#include "OurPaint3D/OurPaint3D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Renderer.h"

static Geometry* geom;

OurPaint3D::OurPaint3D()
	: Application("OurPaint3D")
{	
	m_Texture = Texture::Create("assets/Desert.jpg");

	m_Camera = new Camera({0.0f, 0.0f, 5.0f});
	m_Camera->aspect = (float) m_WinInfo.Width / (float) m_WinInfo.Height;

	CamSpeed = 1.0f;
	CamSensitivity = 1.0f;

	geom = new Geometry();
	geom->verticies = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};		
	
	//geom->verticies = {
	//	-0.5f, -0.5f, -1.0f,   0.0f, 0.0f,
	//	 0.5f, -0.5f, -1.0f,   1.0f, 0.0f,
	//	-0.5f,  0.5f, -1.0f,   0.0f, 1.0f,
	//	 0.5f,  0.5f, -1.0f,   1.0f, 1.0f,
	//};

	geom->texture = m_Texture;
}

OurPaint3D::~OurPaint3D()
{
	delete m_Camera;
	delete m_Texture;
}

void OurPaint3D::OnUpdate(double dt)
{
	// Camera wasd
	{		
		float velocity = CamSpeed * dt;

		if (IsKeyPressed(GLFW_KEY_W))
			m_Camera->Position += m_Camera->Front * velocity;

		if (IsKeyPressed(GLFW_KEY_S))
			m_Camera->Position -= m_Camera->Front * velocity;

		if (IsKeyPressed(GLFW_KEY_A))
			m_Camera->Position -= m_Camera->Right * velocity;

		if (IsKeyPressed(GLFW_KEY_D))
			m_Camera->Position += m_Camera->Right * velocity;

		if (IsKeyPressed(GLFW_KEY_SPACE))
			m_Camera->Position += m_Camera->Up * velocity;

		if (IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
			m_Camera->Position -= m_Camera->Up * velocity;
	}

	static bool firstMouse = true;
	static float lastX = 0;
	static float lastY = 0;
	if (IsMouseButtonPressed(GLFW_MOUSE_BUTTON_3))
	{
		auto pos = GetMousePos();
		float x = pos.first;
		float y = pos.second;

		if (firstMouse)
		{
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		float xoffset = (x - lastX) * CamSensitivity;
		float yoffset = (lastY - y) * CamSensitivity; // reversed since y-coordinates go from bottom to top

		lastX = x;
		lastY = y;

		m_Camera->Yaw += xoffset;
		m_Camera->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped		
		if (m_Camera->Pitch > 89.0f) m_Camera->Pitch = 89.0f;
		if (m_Camera->Pitch < -89.0f) m_Camera->Pitch = -89.0f;
		
		while (m_Camera->Yaw > 360.0f) m_Camera->Yaw -= 360.0f;
		while (m_Camera->Yaw < 0.0f) m_Camera->Yaw += 360.0f;

		m_Camera->ReCalc();
	}
	else
		firstMouse = true;

	Renderer::Begin(m_Camera);
	Renderer::Render(geom);
	Renderer::End();
}

// TODO: Use dockspace to split the geometry selection and editing 50/50
void OurPaint3D::OnImGuiRender()
{		
	static float menuWidth = 380.0f;
	static int menuWindowFlags = 
		//ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(menuWidth, m_WinInfo.Height));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 0.0f));
	ImGui::Begin("LefttMenu", 0, menuWindowFlags);
	ImGui::Image((void*) m_Texture->GetInternalID(), ImVec2(menuWidth, menuWidth));
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Camera Info");

		ImGui::Text("Postition  ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Position", glm::value_ptr(m_Camera->Position), 0.05, -1000.0f, 1000.0f);

		ImGui::Text("Yaw / Pitch = %.3f  / %.3f", m_Camera->Yaw, m_Camera->Pitch);

		ImGui::Separator();
		ImGui::Text("Camera Settings");

		ImGui::Text("Speed       ");
		ImGui::SameLine();
		ImGui::DragFloat("##Speed", &CamSpeed, 0.5f, 1.0f, 10.0f);

		ImGui::Text("Sensitivity ");
		ImGui::SameLine();
		ImGui::DragFloat("##Sensitivity", &CamSensitivity, 0.05, 0.05, 3.0f);			
	}
	ImGui::End();
	

	ImGui::SetNextWindowPos(ImVec2(m_WinInfo.Width - menuWidth, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(menuWidth, m_WinInfo.Height));	
	ImGui::Begin("RightMenu", 0, menuWindowFlags);
	ImGui::PopStyleVar();		
	
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool needReCalc = false;

		ImGui::Text("Position ");
		ImGui::SameLine();
		needReCalc |= ImGui::DragFloat3("##Position", glm::value_ptr(geom->position), 0.05, -1000.0f, 1000.0f);

		ImGui::Text("Scale    ");
		ImGui::SameLine();
		needReCalc |= ImGui::DragFloat3("##Scale", glm::value_ptr(geom->scale), 0.01f, 0.01f, 1000.0f);

		ImGui::Text("Rotation ");
		ImGui::SameLine();
		needReCalc |= ImGui::DragFloat3("##Rotation", glm::value_ptr(geom->rotation), 1.0f, -180.0f, 180.0f);

		if (needReCalc)
			geom->ReCalcTransform();
	}		

	if (ImGui::CollapsingHeader("Geometries", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("+"))
			ImGui::OpenPopup("Select Geometry");

		const char* list[] = {
			"Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"
		};

		if (ImGui::BeginPopup("Select Geometry"))
		{			
			float w = ImGui::GetContentRegionAvailWidth();
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
			for (int i = 0; i < IM_ARRAYSIZE(list); i++)
				if (ImGui::Button(list[i], ImVec2(w, 0.0f)))
				{
					// Add new Geometry
					ImGui::CloseCurrentPopup();
				}
					
			ImGui::PopStyleVar();
			ImGui::EndPopup();
		}

		
		static int current = 1;		
		ImGui::PushItemWidth(menuWidth);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));		
		float w = ImGui::GetContentRegionAvailWidth();
		for (int i = 0; i < IM_ARRAYSIZE(list); i++)
		{
			ImGui::PushID(i);			
			if (ImGui::Selectable("##geometry", i == current, ImGuiSelectableFlags_AllowItemOverlap))
				current = i;

			ImGui::SameLine();
			ImGui::Text(list[i]);
			ImGui::SameLine(w);
			
			if (ImGui::SmallButton("X"))
			{
				// Remove Geometry
				LOG_INFO("Button {} pressed!", i);
			}			

			ImGui::PopID();
		}
		ImGui::PopStyleColor();
		ImGui::PopItemWidth();
	}

	ImGui::End();
}

void OurPaint3D::OnResize(int width, int height)
{
	m_Camera->aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
}
