#include "pch.h"
#include "Renderer.h"

#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

struct RendererData
{
    Shader* TextureShader;
    Texture* WhiteTexture;
    uint32_t VBO, VAO, EBO;
};

static RendererData* data;

void Renderer::Init()
{
    data = new RendererData();

    float vertices[5 * 4] = {
		// Pos        // Tex
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &data->VAO);
    glGenBuffers(1, &data->VBO);
    glGenBuffers(1, &data->EBO);

	// Must bind VAO before binding VBO or EBO
    glBindVertexArray(data->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, data->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position Attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

	// Texture Coord Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind follow the reverse order
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    data->WhiteTexture = Texture::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
    data->TextureShader->Bind();
    data->TextureShader->SetInt("u_Texture", 0);
}

void Renderer::Shutdown()
{
    glDeleteVertexArrays(1, &data->VAO);
    glDeleteBuffers(1, &data->VBO);
    glDeleteBuffers(1, &data->EBO);

    delete data->TextureShader;
    delete data->WhiteTexture;
    delete data;
}

void Renderer::BeginScene(float width, float height)
{
	data->TextureShader->Bind();
    data->TextureShader->SetMat4(
        "u_Projection",
        glm::ortho(
            0.0f, width,
            height, 0.0f,
            0.0f, 1.0f
        )
    );
    data->TextureShader->SetMat4(
        "u_TexTransform",
        glm::scale(
            glm::mat4(1.0f),
            { 0.1f, 0.8f, 1.0f }
        )
    );

	glBindVertexArray(data->VAO);
}

void Renderer::EndScene()
{
}

void Renderer::DrawRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
    data->TextureShader->SetFloat4("u_Color", color);
    data->WhiteTexture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    data->TextureShader->SetMat4("u_Transform", transform);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawTexture(const Texture* const texture, const glm::vec2& destPos)
{
	DrawTexture(texture, destPos, { 1.0f, 1.0f });
}

void Renderer::DrawTexture(const Texture* const texture, const glm::vec2& destPos, const glm::vec2& destSize)
{
    data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    texture->Bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { destPos.x, destPos.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { destSize.x, destSize.y, 1.0f });
    data->TextureShader->SetMat4("u_Transform", transform);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
