#include "pch.h"
#include "Application.h"

#include "Renderer/Renderer.h"

Application::Application(const char* title, uint32_t screenWidth, uint32_t screenHeight)
{
	m_Data = WindowData{
		this, title,
		screenWidth, screenHeight
	};

    glfwSetErrorCallback([](int error, const char* description) {
		LOG_ERROR("{0}", description);
	});

    ASSERT(glfwInit(), "Failed to initialize GLFW!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);

    if (!m_Window)
    {
        glfwTerminate();
        LOG_ERROR("Failed to create GLFW window!");
        exit(EXIT_FAILURE);
    }

	glfwSetWindowUserPointer(m_Window, &m_Data);

	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
	{
			WindowData* data = (WindowData*) glfwGetWindowUserPointer(window);
			data->Width = width;
			data->Height = height;

			data->App->OnResize(width, height);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
					data->App->OnKeyboard(key, KeyState::DOWN);
					break;

				case GLFW_RELEASE:
					data->App->OnKeyboard(key, KeyState::UP);
					break;

				case GLFW_REPEAT:
					data->App->OnKeyboard(key, KeyState::REPEAT);
					break;
			}
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
	{
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		data->App->OnMouseMove(xpos, ypos);
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
					data->App->OnMouseButton(button, KeyState::DOWN);
					break;
				case GLFW_RELEASE:
					data->App->OnMouseButton(button, KeyState::UP);
			}
	});

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }

	LOG_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
	LOG_INFO("GLSL Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	LOG_INFO("Renderer: {0}", glGetString(GL_RENDERER));

    glfwSwapInterval(1);

    Renderer::Init();
}

Application::~Application()
{
    Renderer::Shutdown();

    glfwDestroyWindow(m_Window);

    glfwTerminate();
}

void Application::Run()
{
    double currTime;
    double lastTime = 0;
    glfwSetTime(lastTime);

    while (!glfwWindowShouldClose(m_Window))
    {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        currTime = glfwGetTime();
        OnUpdate(currTime - lastTime);
        lastTime = currTime;

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Application::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
}

bool Application::IsKeyPressed(int key)
{
	int state = glfwGetKey(m_Window, key);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Application::IsMouseButtonPressed(int btn)
{
	int state = glfwGetMouseButton(m_Window, btn);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

std::pair<float, float> Application::GetMousePos()
{
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);

	return { (float) xpos, (float) ypos };
}

glm::vec2 Application::ConvertCoord(const glm::vec2& pos)
{
	return {
		(2.0 * pos.x + 1.0) / m_Data.Width - 1.0,
		(2.0 * pos.y + 1.0) / m_Data.Height - 1.0
	};
}