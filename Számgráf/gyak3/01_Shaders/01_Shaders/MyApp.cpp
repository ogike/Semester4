#include "MyApp.h"
#include "GLUtils.hpp"

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
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	// LÉNYEG:
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");
	//visszaadja az id-ját, azzal dolgozunk

// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá az adott programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// illesszük össze a shadereket (kimenő-bemenő változók összerendelése stb.)
	glLinkProgram(m_programID);

	//mivel keletkezhetnek fordítási hibák:
	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, &VertexShaderErrorMessage[0]);

		std::cerr << "[glLinkProgram] Shader linking error:\n" << &VertexShaderErrorMessage[0] << std::endl;
	}

	// már nincs ezekre szükség, miért a programban már benne vannak
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	return true;
}

void CMyApp::Clean()
{
	glDeleteProgram(m_programID);
}

void CMyApp::Update()
{
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa, alapértelmezetten nem történik meg
	//		egyszerre csak egyet tudunk használni
	//		ha 0 paraméterrel, akkor alapértelmezettet fogja aktiválni
	glUseProgram(m_programID);

	//feladat 3: uniform változó megadása
	float time = SDL_GetTicks() / 1000.f;
	//mivel c, minden változójú uniformhoz más fgv
	//meg kell adni neki, hogy 
		//hova állítsa be ezt (program, változónév)
		//új érték

	glUniform1f(glGetUniformLocation(m_programID, "time"), time);


	//SZORGALMI:
	glUniform1f(glGetUniformLocation(m_programID, "zoom"), mouseZoom);
	glUniform1f(glGetUniformLocation(m_programID, "posXDiff"), posXDiff);
	glUniform1f(glGetUniformLocation(m_programID, "posYDiff"), posYDiff);


	// kirajzolás: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
	glDrawArrays(GL_TRIANGLES,	// primitív típusa; amikkel mi foglalkozunk: GL_POINTS, GL_LINE_STRIP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES
		0,					// index: ha van tároló amiben a kirajzolandó geometriák csúcspontjait tároljuk, akkor annak hányadik csúcspontjától rajzoljunk - most nincs ilyen, 
							// csak arra használjuk, hogy a gl_VertexID számláló a shader-ben melyik számról induljon, azaz most nulláról
		6);					// hány csúcspontot használjunk a primitívek kirajzolására - most: gl_VertexID számláló 0-tól indul és 5-ig megy, azaz összesen 6x fut le a vertex shader

// shader kikapcsolasa
	glUseProgram(0);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if (isMouseDown) {
		posXDiff += (mouse.xrel * mouseDragSpeed * (1/mouseZoom)); //should use deltaTime
		posYDiff -= (mouse.yrel * mouseDragSpeed * (1 / mouseZoom)); //should use deltaTime

	}
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	isMouseDown = true;
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
	isMouseDown = false;
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
	mouseZoom += (wheel.y * mouseZoom); //speed it up more when we are zoomed in, to make it easier to see details
	if (mouseZoom < 1)
		mouseZoom = 1;
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
}
