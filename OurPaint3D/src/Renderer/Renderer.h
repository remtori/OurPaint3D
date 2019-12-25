#pragma once

#include <unordered_map>

#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Renderer/Geometries/Geometry.h"

#define MAX_VERTICIES 65535

struct RendererData
{
	Shader* geomShader;
	Texture* whiteTexture;
	Texture* rgbTexture;
	uint64_t offset;
	std::unordered_map<Geometry::EObject, std::pair<uint64_t, uint64_t>> objectTable;
	// OpenGL Stuff
	uint32_t VAO;
	uint32_t vertexBuffer;
};

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void Begin(Camera* cam, bool drawCenter = true, bool drawGrid = true);
	static void End();
	static void Render(Geometry* geom, bool hightlight = false);

public:	
	static RendererData* m_Data;
};

