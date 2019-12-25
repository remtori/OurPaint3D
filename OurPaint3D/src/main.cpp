#include "pch.h"

#include "OurPaint3D/OurPaint3D.h"

int CALLBACK WinMain(
	__in  HINSTANCE hInstance,
	__in  HINSTANCE hPrevInstance,
	__in  LPSTR lpCmdLine,
	__in  int nCmdShow
)
{
	Log::Init();
	Application* app = new OurPaint3D();
	app->Run();
	delete app;
}