
#include "Hashira.h"
#include "App/App.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	App app;

	app.Run();

	return 0;
};