#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

class Renderer
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(float w, float h);
    static void EndScene();

    static void DrawRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);

    static void DrawTexture(const Texture* const texture, const glm::vec2& destPos);
	static void DrawTexture(const Texture* const texture, const glm::vec2& destPos, const glm::vec2& destSize);
	static void DrawTexture(
		const Texture* const texture,
		const glm::vec2& srcPos, glm::vec2& srcSize,
		const glm::vec2& destPos, const glm::vec2& destSize
	);
};
