#include "MyApp.h"
#include <math.h>

CMyApp::CMyApp(void)
{
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
		// "void"
		// egyszer beállítom és mindig ez lesz a törléso szín
		// OPENGL-ben lebegõpontos számok a színek (de ugyanúgy 256 bitesm csak inkább skála)
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	// kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	//ez SDL-en belül van, szóval elérhetõ minden SDL-es parancs itt is
	float t = SDL_GetTicks() / 1000.f;
	glClearColor( (sin(t)+1) / 2, ((sin(t + M_PI) + 1) / 2), 0, 1.0f);
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// | = bitenkénti VAGY operátor(eredmény minden bitje egyes lesz ha valamelyikben egyes volt
	// mintha összeadnánk (lényeg hogy mindkettõ apply-ol)
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );
}