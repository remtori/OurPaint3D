#include "pch.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RendererData* Renderer::m_Data;

void Renderer::Init()
{
	m_Data = new RendererData();
	m_Data->whiteTexture = Texture::Create(1, 1);	
	m_Data->shader = Shader::Create("Geometry Shader",
		// Vertex shader
		"#version 330 core\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"		

		"layout (location = 0) in vec3 Position;\n"
		"layout (location = 1) in vec2 UV;\n"

		"out vec2 Frag_UV;\n"

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

	m_Data->shader->Bind();
	m_Data->shader->SetInt("Texture", 0);

	uint32_t whiteTextureData = 0xffffffff;
	m_Data->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	
	glGenVertexArrays(1, &m_Data->VAO);
	glBindVertexArray(m_Data->VAO);

	glGenBuffers(1, &m_Data->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_Data->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICIES * sizeof(float) * 5, nullptr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::Shutdown()
{
	delete m_Data->whiteTexture;
	delete m_Data->shader;
	delete m_Data;
}

void Renderer::Begin(Camera* cam)
{
	m_Data->shader->Bind();	
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_Data->VAO);

	m_Data->shader->SetMat4("projection", 
		glm::perspective(glm::radians(45.0f), cam->aspect, 0.1f, 100.0f)
	);

	m_Data->shader->SetMat4("view", cam->GetViewMatrix());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::End()
{
}

void Renderer::Render(Geometry* geom)
{	
	m_Data->shader->SetMat4("model", geom->transformationMatrix);
	m_Data->shader->SetFloat4("Color", geom->color);

	if (geom->texture != nullptr)
		geom->texture->Bind();
	else
		m_Data->whiteTexture->Bind();

	glBufferSubData(GL_ARRAY_BUFFER, 0, geom->verticies.size() * sizeof(float), geom->verticies.data());

	glDrawArrays(GL_TRIANGLES, 0, geom->verticies.size() / 5);
}
