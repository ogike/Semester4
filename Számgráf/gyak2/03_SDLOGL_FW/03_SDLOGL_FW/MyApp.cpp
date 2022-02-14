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
	// t�rl�si sz�n legyen k�kes
		// "void"
		// egyszer be�ll�tom �s mindig ez lesz a t�rl�so sz�n
		// OPENGL-ben lebeg�pontos sz�mok a sz�nek (de ugyan�gy 256 bitesm csak ink�bb sk�la)
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
	//ez SDL-en bel�l van, sz�val el�rhet� minden SDL-es parancs itt is
	float t = SDL_GetTicks() / 1000.f;
	glClearColor( (sin(t)+1) / 2, ((sin(t + M_PI) + 1) / 2), 0, 1.0f);
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// | = bitenk�nti VAGY oper�tor(eredm�ny minden bitje egyes lesz ha valamelyikben egyes volt
	// mintha �sszeadn�nk (l�nyeg hogy mindkett� apply-ol)
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

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );
}