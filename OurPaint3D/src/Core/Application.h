#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum KeyState {
	UP, DOWN, REPEAT
};

class Application {
public:
    Application(const char* title, float screenWidth = 640, float screenHeight = 360);
    virtual ~Application();

    void Run();
protected:
    virtual void OnUpdate(double deltaTime) = 0;
	virtual void OnImGuiRender() = 0;

	virtual void OnResize(int width, int height);

	virtual void OnKeyboard(int keyCode, int keyState) {};

	virtual void OnMouseMove(float x, float y) {};
	virtual void OnMouseButton(int btn, int keyState) {};

	bool IsKeyPressed(int key);
	bool IsMouseButtonPressed(int button);
	std::pair<float, float> GetMousePos();

	glm::vec2 ConvertCoord(const glm::vec2& pos);
protected:
	GLFWwindow* m_Window;

	struct WindowData
	{
		Application* App;
		const char* Title;
		float Width, Height;
	};
	WindowData m_WinInfo;
};
