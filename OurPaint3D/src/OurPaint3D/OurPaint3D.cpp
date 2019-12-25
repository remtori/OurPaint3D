#include "pch.h"
#include "OurPaint3D/OurPaint3D.h"

#include <imgui.h>
#include <windows.h>
#include <Commdlg.h>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/Geometries/Geometries.h"

OurPaint3D::OurPaint3D()
	: Application("OurPaint3D"),
	m_Textures(), m_Geometries(),
	IsCenterVisible(false), IsGridVisible(true), IsHightlightEnable(true),
	CamSensitivity(1.0f), CamSpeed(2.0f),
	menuWidth(254.0f)
{	
	m_Camera = new Camera({ 6.0f, 6.0f, 6.0f });
	m_Camera->aspectRatio = (float) m_WinInfo.Width / (float) m_WinInfo.Height;
	m_Camera->Yaw = 225.0f;
	m_Camera->Pitch = -30.0f;
	m_Camera->ReCalc();

	Texture* tex = Renderer::m_Data->rgbTexture;
	m_Textures.push_back(tex);

	currentGeom = new Cube();
	currentGeom->texture = tex;
	currentGeom->position.y = 3.0f;
	currentGeom->ReCalcTransform();
	m_Geometries.push_back(currentGeom);

	currentGeom = new Pyramid();
	currentGeom->texture = tex;
	currentGeom->position.x = 3.0f;
	currentGeom->ReCalcTransform();
	m_Geometries.push_back(currentGeom);

	currentGeom = new Prismatic();
	currentGeom->texture = tex;
	currentGeom->position.z = 3.0f;
	currentGeom->ReCalcTransform();
	m_Geometries.push_back(currentGeom);

	currentGeom = new Sphere();
	currentGeom->texture = tex;
	currentGeom->scale = { 2.0f, 2.0f, 2.0f };
	currentGeom->ReCalcTransform();
	m_Geometries.push_back(currentGeom);
}

OurPaint3D::~OurPaint3D()
{
	for (Texture* t : m_Textures)
		delete t;

	for (Geometry* g : m_Geometries)
		delete g;

	delete m_Camera;	
}

void OurPaint3D::OnUpdate(double dt)
{	
	HandleCamera(dt);
	Renderer::Begin(m_Camera, IsCenterVisible, IsGridVisible);
	for (Geometry* geom : m_Geometries)
		Renderer::Render(geom, IsHightlightEnable && geom == currentGeom);
	Renderer::End();
}

// TODO: Use dockspace to split the geometry selection and editing 50/50
void OurPaint3D::OnImGuiRender()
{			
	static int menuWindowFlags = 
		//ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;	

	static bool showUsage = true;
	if (showUsage)
		ImGui::OpenPopup("Basic Usage");

	if (ImGui::BeginPopupModal("Basic Usage", &showUsage, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(
			"Use WASD to move around\n"
			"Hold middle mouse button to look around\n"
			"All numeric input can be change quicky by dragging\n"
			"Have fun test thing out~ OurPaint3D\n"
		);

		if (ImGui::Button("OK", ImVec2(-1.0, 0)))
		{
			showUsage = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(menuWidth, m_WinInfo.Height));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 0.0f));
	ImGui::Begin("LefttMenu", 0, menuWindowFlags);
	
	if (ImGui::CollapsingHeader("General Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Show World Center", &IsCenterVisible);		
		ImGui::Checkbox("Show World Grid", &IsGridVisible);				
		ImGui::Checkbox("Hightlight Selected Object", &IsHightlightEnable);

		ImGui::Text("Menu Width");
		ImGui::SameLine();
		ImGui::DragFloat("##MenuWidth", &menuWidth, 0.5, 150.0f, 400.0f);
	}

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Postition ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Position", glm::value_ptr(m_Camera->Position), 0.05, -1000.0f, 1000.0f);

		ImGui::Text("FOV       ");
		ImGui::SameLine();
		ImGui::DragFloat("##FOV", &m_Camera->FOV, 0.1, 0.0f, 180.0f);

		ImGui::Text("(RX | RY) = (%.3f | %.3f)", m_Camera->Yaw, m_Camera->Pitch);
		
		ImGui::Text("");
		ImGui::Text("Camera Settings");
		ImGui::Separator();

		ImGui::Text("Speed       ");
		ImGui::SameLine();
		ImGui::DragFloat("##Speed", &CamSpeed, 0.5f, 1.0f, 10.0f);

		ImGui::Text("Sensitivity ");
		ImGui::SameLine();
		ImGui::DragFloat("##Sensitivity", &CamSensitivity, 0.05, 0.05, 3.0f);			
	}	

	if (ImGui::CollapsingHeader("Object List", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Geometry* tobeRemove = nullptr;
		float w = ImGui::GetContentRegionAvailWidth();

		if (ImGui::Button("Add Object", ImVec2(w / 2, 0.0f)))
			ImGui::OpenPopup("AddGeometry_ContextMenu");

		ImGui::SameLine();

		if (ImGui::Button("Clear Objects", ImVec2(w / 2, 0.0f)))
		{
			for (Geometry* g : m_Geometries)
				delete g;

			m_Geometries.clear();
		}

		ImGui::Separator();

		ImGui::BeginChild("TextureContainer");
		
		int id = 0;
		for (Geometry* geom : m_Geometries)
		{
			ImGui::PushID(++id);

			if (ImGui::Selectable("##geometry", geom == currentGeom, ImGuiSelectableFlags_AllowItemOverlap))
				currentGeom = geom;

			if (ImGui::BeginPopupContextItem("##edit name ctx menu"))
			{
				ImGui::InputText("##edit name", geom->name, 128);
				ImGui::EndPopup();
			}

			ImGui::SameLine();
			ImGui::Text(geom->name);
			ImGui::SameLine(w);

			if (ImGui::SmallButton("X"))
				tobeRemove = geom;

			ImGui::PopID();
		}

		if (tobeRemove != nullptr)
		{
			auto it = std::find(m_Geometries.cbegin(), m_Geometries.cend(), tobeRemove);
			m_Geometries.erase(it);
			if (currentGeom == tobeRemove && m_Geometries.size() > 0)
				currentGeom = m_Geometries[0];
		}

		ImGui::EndChild();
	}


	if (IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
	{
		auto pos = GetMousePos();
		if (menuWidth < pos.first && pos.first < (m_WinInfo.Width - menuWidth))
			ImGui::OpenPopup("AddGeometry_ContextMenu");
	}

	if (ImGui::BeginPopup("AddGeometry_ContextMenu"))
	{
		float w = ImGui::GetContentRegionAvailWidth();
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

		ImGui::Text("Choose Geometry");

		void* prevGeom = currentGeom;
		if (ImGui::Button("Cube", ImVec2(w, 0.0f))) currentGeom = new Cube(); 
		if (ImGui::Button("Prismatic", ImVec2(w, 0.0f))) currentGeom = new Prismatic();
		if (ImGui::Button("Pyramid", ImVec2(w, 0.0f))) currentGeom = new Pyramid();
		if (ImGui::Button("Sphere", ImVec2(w, 0.0f))) currentGeom = new Sphere();

		if (prevGeom != currentGeom)
		{
			m_Geometries.push_back(currentGeom);
			ImGui::CloseCurrentPopup();
		}

		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}
	ImGui::End();
	

	ImGui::SetNextWindowPos(ImVec2(m_WinInfo.Width - menuWidth, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(menuWidth, m_WinInfo.Height));	
	ImGui::Begin("RightMenu", 0, menuWindowFlags);
	ImGui::PopStyleVar();		
	
	if (ImGui::CollapsingHeader("Object Properties", ImGuiTreeNodeFlags_DefaultOpen) && currentGeom != nullptr)
	{		
		ImGui::Text("Name     ");
		ImGui::SameLine();
		ImGui::InputText("##edit name", currentGeom->name, 128);

		ImGui::Text("Color    ");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Color", glm::value_ptr(currentGeom->color));

		bool needReCalc = false;
		ImGui::Text("Position ");
		ImGui::SameLine();
		needReCalc |= ImGui::DragFloat3("##Position", glm::value_ptr(currentGeom->position), 0.05, -1000.0f, 1000.0f);

		ImGui::Text("Scale    ");
		ImGui::SameLine();
		needReCalc |= ImGui::DragFloat3("##Scale", glm::value_ptr(currentGeom->scale), 0.01f, 0.01f, 1000.0f);

		ImGui::Text("Rotation ");
		ImGui::SameLine();
		needReCalc |= ImGui::DragFloat3("##Rotation", glm::value_ptr(currentGeom->rotation), 1.0f, -180.0f, 180.0f);		

		if (needReCalc)
			currentGeom->ReCalcTransform();
	}		

	if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float w = ImGui::GetContentRegionAvailWidth();

		if (ImGui::Button("Add Texture", ImVec2(w / 2, 0.0f)))
		{
			char filename[MAX_PATH];

			tagOFNA ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = "Image File\0*.png;*.jpg;*.jpeg;*.bmp;*.gif;\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select an image";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			GetOpenFileNameA(&ofn);

			Texture* tex = Texture::Create(ofn.lpstrFile);
			currentGeom->texture = tex;
			m_Textures.push_back(tex);
		}

		ImGui::SameLine();

		if (ImGui::Button("White Texture", ImVec2(w / 2, 0.0f)) && currentGeom)
			currentGeom->texture = Renderer::m_Data->whiteTexture;

		ImGui::Separator();

		ImGui::BeginChild("TextureContainer");

		int i = 0;		
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5, 0.5));

		for (Texture* texture : m_Textures)
		{
			ImVec2 size;
			size.x = w;
			size.y = w / texture->GetWidth() * texture->GetHeight();

			ImGui::PushID(++i);

			if (ImGui::Selectable("##Texture", currentGeom && currentGeom->texture == texture, 0, size))
			{
				currentGeom->texture = texture;
			}

			ImGui::SameLine(4);
			ImGui::Image((void*)texture->GetInternalID(), ImVec2(size.x - 8, size.y - 8));

			ImGui::PopID();
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();
	}

	ImGui::End();
}

void OurPaint3D::HandleCamera(double dt)
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

	static bool firstMouse = true;
	static float lastX = 0;
	static float lastY = 0;
	if (IsMouseButtonPressed(GLFW_MOUSE_BUTTON_3))
	{
		auto pos = GetMousePos();
		float x = pos.first;
		float y = pos.second;

		if (x < menuWidth || x > (m_WinInfo.Width - menuWidth)) return;

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
}

void OurPaint3D::OnResize(int width, int height)
{
	if (height == 0) return;
	m_Camera->aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}
