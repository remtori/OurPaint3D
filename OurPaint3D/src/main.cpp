#include "pch.h"

#include "OurPaint3D/OurPaint3D.h"

int main()
{
	Log::Init();
	Application* app = new OurPaint3D();
	app->Run();
	delete app;
}