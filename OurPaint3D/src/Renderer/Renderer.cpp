#include "pch.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RendererData* Renderer::m_Data;

void Renderer::Init()
{
	m_Data = new RendererData();
	m_Data->objectTable = std::unordered_map<Geometry::EObject, std::pair<uint64_t, uint64_t>>();

	m_Data->whiteTexture = Texture::Create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	m_Data->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

	m_Data->rgbTexture = Texture::Create(2, 2);
	uint32_t rgbTextureData[] = { 0xff0000ff, 0xff00ff00, 0xffff0000, 0xffffffff };
	m_Data->rgbTexture->SetData(&rgbTextureData, sizeof(rgbTextureData));

	glGenVertexArrays(1, &m_Data->VAO);
	glBindVertexArray(m_Data->VAO);

	glGenBuffers(1, &m_Data->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_Data->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICIES * sizeof(float) * 5, nullptr, GL_DYNAMIC_DRAW);

	m_Data->geomShader = Shader::Create("Geometry Shader",
		// Vertex shader
		"#version 330 core\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"layout (location = 0) in vec3 Position;\n"
		"layout (location = 1) in vec2 UV;\n"

		"out vec2 Frag_UV;\n"
		"out vec3 Frag_Normal;\n"

		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(Position, 1);\n"
		"	Frag_UV = vec2(UV.x, 1.0 - UV.y);\n"
		"}\n",
		// Fragment shader
		"#version 330 core\n"

		"uniform vec4 Color;\n"
		"uniform sampler2D Texture;\n"

		"in vec2 Frag_UV;\n"
		"layout(location = 0) out vec4 Frag_Color;\n"

		"void main()\n"
		"{\n"
		"    Frag_Color = Color * texture(Texture, Frag_UV);\n"
		"}\n"
	);

	m_Data->geomShader->Bind();
	m_Data->geomShader->SetInt("Texture", 0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float x[] = {
		-5.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 	 5.0f,  0.0f,  0.0f,  0.0f, 0.0f
	};
	float y[] = {
		 0.0f, -5.0f,  0.0f,  0.5f, 0.0f,
	 	 0.0f,  5.0f,  0.0f,  0.5f, 0.0f
	};
	float z[] = {
		 0.0f,  0.0f, -5.0f,  1.0f, 0.5f,
		 0.0f,  0.0f,  5.0f,  1.0f, 0.5f
	};
	uint64_t s = sizeof(x);

	m_Data->offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, m_Data->offset, s, x);
	m_Data->offset += s;
	glBufferSubData(GL_ARRAY_BUFFER, m_Data->offset, s, y);
	m_Data->offset += s;
	glBufferSubData(GL_ARRAY_BUFFER, m_Data->offset, s, z);
	m_Data->offset += s;

	m_Data->objectTable[Geometry::Origin] = { 0, 6 };

	static int GRID_COUNT = 63;
	static uint64_t GRID_SIZE = GRID_COUNT * 10 * 2;
	float* grid = new float[GRID_SIZE];

	for (int i = 0; i < GRID_COUNT * 10; i += 10)
	{
		grid[i + 0] = -GRID_COUNT / 2;
		grid[i + 1] = 0.0f;
		grid[i + 2] = i / 10 - GRID_COUNT / 2;
		grid[i + 3] = 0.5f;
		grid[i + 4] = 0.5f;

		grid[i + 5] = GRID_COUNT / 2;
		grid[i + 6] = 0.0f;
		grid[i + 7] = i / 10 - GRID_COUNT / 2;
		grid[i + 8] = 0.5f;
		grid[i + 9] = 0.5f;

		grid[GRID_COUNT * 10 + i + 0] = i / 10 - GRID_COUNT / 2;
		grid[GRID_COUNT * 10 + i + 1] = 0.0f;
		grid[GRID_COUNT * 10 + i + 2] = -GRID_COUNT / 2;
		grid[GRID_COUNT * 10 + i + 3] = 0.5f;
		grid[GRID_COUNT * 10 + i + 4] = 0.5f;

		grid[GRID_COUNT * 10 + i + 5] = i /10 - GRID_COUNT / 2;
		grid[GRID_COUNT * 10 + i + 6] = 0.0f;
		grid[GRID_COUNT * 10 + i + 7] = GRID_COUNT / 2;
		grid[GRID_COUNT * 10 + i + 8] = 0.5f;
		grid[GRID_COUNT * 10 + i + 9] = 0.5f;
	}

	glBufferSubData(GL_ARRAY_BUFFER, m_Data->offset, GRID_SIZE * sizeof(float), grid);
	m_Data->objectTable[Geometry::Grid] = { m_Data->offset / (5 * sizeof(float)), GRID_SIZE / 5 };
	m_Data->offset += GRID_SIZE * sizeof(float);

	delete[] grid;
}

void Renderer::Shutdown()
{
	delete m_Data;
}

void Renderer::Begin(Camera* cam, bool drawCenter, bool drawGrid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_Data->VAO);

	m_Data->geomShader->Bind();
	m_Data->geomShader->SetMat4("model", glm::mat4(1.0f));
	m_Data->geomShader->SetMat4("view", cam->GetViewMatrix());
	m_Data->geomShader->SetMat4("projection", cam->GetProjectionMatrix());

	m_Data->rgbTexture->Bind();
	m_Data->geomShader->SetFloat4("Color", { 1.0f, 1.0f, 1.0f, 1.0f });

	if (drawCenter)
		glDrawArrays(GL_LINES, m_Data->objectTable[Geometry::Origin].first, m_Data->objectTable[Geometry::Origin].second);

	if (drawGrid)
		glDrawArrays(GL_LINES, m_Data->objectTable[Geometry::Grid].first, m_Data->objectTable[Geometry::Grid].second);
}

void Renderer::End()
{
	//GLenum err;
	//while ((err = glGetError()) != GL_NO_ERROR)
	//{
	//	LOG_ERROR("OpenGL Error: {}", err);
	//}
}

void Renderer::Render(Geometry* geom, bool hightlight)
{
	m_Data->geomShader->SetMat4("model", geom->transformationMatrix);
	m_Data->geomShader->SetFloat4("Color", geom->color);

	if (geom->texture != nullptr)
		geom->texture->Bind();
	else
		m_Data->rgbTexture->Bind();

	if (m_Data->objectTable.find(geom->type) == m_Data->objectTable.cend())
	{
		auto v = geom->GetVerticies();
		glBufferSubData(GL_ARRAY_BUFFER, m_Data->offset, v->size() * sizeof(float), v->data());
		m_Data->objectTable[geom->type] = { m_Data->offset / (5 * sizeof(float)), v->size() / 5 };
		m_Data->offset += v->size() * sizeof(float);
		delete v;
	}

	glDrawArrays(GL_TRIANGLES, m_Data->objectTable[geom->type].first, m_Data->objectTable[geom->type].second);

	if (hightlight)
	{
		m_Data->whiteTexture->Bind();
		m_Data->geomShader->SetMat4("model", glm::scale(geom->transformationMatrix, { 1.01f, 1.01f, 1.01f }));
		m_Data->geomShader->SetFloat4("Color", { 0.0f, 0.5f, 1.0f, 0.5f });
		glDrawArrays(GL_TRIANGLES, m_Data->objectTable[geom->type].first, m_Data->objectTable[geom->type].second);
	}
}
