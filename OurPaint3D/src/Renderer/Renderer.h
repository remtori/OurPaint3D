#pragma once

#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Renderer/Geometries/Geometry.h"

#define MAX_VERTICIES 1024

typedef unsigned int uint32_t;

struct RendererData
{
	Shader* shader;
	Texture* whiteTexture;
	// OpenGL Stuff
	uint32_t VAO;
	uint32_t vertexBuffer;
	uint32_t indexBuffer;
};

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void Begin(Camera* cam);
	static void End();
	static void Render(Geometry* geom);

public:	
	static RendererData* m_Data;
};

