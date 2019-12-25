#include "pch.h"

#include "Renderer/Geometries/Geometries.h"

Cube::Cube() : Geometry(EObject::Cube, "Cube")
{
}

std::vector<float>* Cube::GetVerticies() 
{
	return new std::vector<float>({
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
	});
}

Prismatic::Prismatic() : Geometry(EObject::Prismatic, "Prismatic")
{
}

std::vector<float>* Prismatic::GetVerticies()
{
	return new std::vector<float>({
		// Top
		0.0, -0.5, -0.5,   0.5,  0.0,
	   -0.5, -0.5,  0.5,   0.0,  1.0,
		0.5, -0.5,  0.5,   1.0,  1.0,
		// Bottom
		0.0,  0.5, -0.5,   0.5,  0.0,
	   -0.5,  0.5,  0.5,   0.0,  1.0,
		0.5,  0.5,  0.5,   1.0,  1.0,

		// 1-2
		0.0, -0.5, -0.5,   0.0,  0.0,
		0.0,  0.5, -0.5,   0.0,  1.0,
	   -0.5, -0.5,  0.5,   1.0,  0.0,
		0.0,  0.5, -0.5,   0.0,  1.0,
	   -0.5, -0.5,  0.5,   1.0,  0.0,
	   -0.5,  0.5,  0.5,   1.0,  1.0,
	   // 2-3
	  -0.5, -0.5,  0.5,   0.0,  0.0,
	  -0.5,  0.5,  0.5,   0.0,  1.0,
	   0.5, -0.5,  0.5,   1.0,  0.0,
	  -0.5,  0.5,  0.5,   0.0,  1.0,
	   0.5, -0.5,  0.5,   1.0,  0.0,
	   0.5,  0.5,  0.5,   1.0,  1.0,
	   // 1-3
	   0.0, -0.5, -0.5,   0.0,  0.0,
	   0.0,  0.5, -0.5,   0.0,  1.0,
	   0.5, -0.5,  0.5,   1.0,  0.0,
	   0.0,  0.5, -0.5,   0.0,  1.0,
	   0.5, -0.5,  0.5,   1.0,  0.0,
	   0.5,  0.5,  0.5,   1.0,  1.0,
	});
}

Pyramid::Pyramid() : Geometry(EObject::Pyramid, "Pyramid")
{
}

std::vector<float>* Pyramid::GetVerticies()
{
	return new std::vector<float>({
		// Bottom
		-0.5,  0.0, -0.5,   0.0, 0.0,
		 0.5,  0.0, -0.5,   1.0, 0.0,
		-0.5,  0.0,  0.5,   0.0, 1.0,
		 0.5,  0.0, -0.5,   1.0, 0.0,
		-0.5,  0.0,  0.5,   0.0, 1.0,
		 0.5,  0.0,  0.5,   1.0, 1.0,

		-0.5,  0.0, -0.5,   0.0, 0.0,
		 0.5,  0.0, -0.5,   1.0, 0.0,
		 0.0,  0.5,  0.0,   0.0, 1.0,

		-0.5,  0.0,  0.5,   0.0, 0.0,
		 0.5,  0.0,  0.5,   1.0, 0.0,
		 0.0,  0.5,  0.0,   0.0, 1.0,

		 0.5,  0.0,  0.5,   1.0, 0.0,
		 0.5,  0.0, -0.5,   0.0, 0.0,
		 0.0,  0.5,  0.0,   0.0, 1.0,

		-0.5,  0.0, -0.5,   0.0, 0.0,
		-0.5,  0.0,  0.5,   1.0, 0.0,
		 0.0,  0.5,  0.0,   0.0, 1.0,
	});
}

Sphere::Sphere() : Geometry(EObject::Sphere, "Sphere")
{
}

void sphere_point(std::vector<float>* verticies, float u, float v)
{
	static float PI = std::atan(1.0f) * 4.0f;

	float r = std::sin(PI * v);
	verticies->push_back(r * std::cos(2.0f * PI * u));
	verticies->push_back(r * std::sin(2.0f * PI * u));
	verticies->push_back(std::cos(PI * v));
	verticies->push_back(1.0f - u);
	verticies->push_back(v);
}

std::vector<float>* Sphere::GetVerticies()
{
	const static int hSize = 60;
	const static int vSize = 120;

	auto verticies = new std::vector<float>();
	verticies->reserve(hSize * vSize * 6 * 5);

	for (int i = 0; i < hSize; i++) 
	{
		for (int j = 0; j < vSize; j++) 
		{
			float u0 = (float)  i      / (float) hSize;
			float u1 = (float) (i + 1) / (float) hSize;
			float v0 = (float)      j  / (float) vSize;
			float v1 = (float) (j + 1) / (float) vSize;
			
			sphere_point(verticies, u0, v0);
			sphere_point(verticies, u1, v0);
			sphere_point(verticies, u0, v1);
			sphere_point(verticies, u0, v1);
			sphere_point(verticies, u1, v0);
			sphere_point(verticies, u1, v1);
		}
	}

	return verticies;
}
