#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0; 
	m_vaoID2 = 0;
	m_vboID2 = 0;
	m_programID = 0;
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


	//
	// geometria létrehozása
	//
	// 
	Vertex vert[] =
	{ 
		//position			 , color
		{glm::vec3(-1, -1, 0), glm::vec3(1, 0, 0)}, //bal alsó
		{glm::vec3( 1, -1, 0), glm::vec3(0, 1, 0)},
		{glm::vec3(-1,  1, 0), glm::vec3(0, 0, 1)},
		{glm::vec3(1, -1, 0), glm::vec3(0, 1, 0)},
		{glm::vec3( 1,  1, 0), glm::vec3(1, 1, 1)}, //jobb felső
		{glm::vec3(-1,  1, 0), glm::vec3(0, 0, 1)},
		//sorrend fontos, mert fordított sorrendben hátlapot fogunk kapni, és eldobódik!

	};

	// 1 db VAO foglalása
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beállítasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erőforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
		//ezzel másoljuk át az adatot GPUra
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  sizeof(vert),		// ennyi bájt nagyságban
				  vert,	// erről a rendszermemóriabeli címről olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat
									//"milyen módon érjük el és módosítjuk az adatot"
									// = egyszer fogja beleírni és sokat olvasunk mód
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az első 3 float sizeof(Vertex)-enként lesz az első attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben található adatok közül a 0. "indexű" attribútumait állítjuk be
		3,				// komponens szám
		GL_FLOAT,		// adatok típusa
		GL_FALSE,		// normalizált legyen-e
		sizeof(Vertex),	// stride (0=egymás után) = milyen "közökkel" találhatóak meg az egymás utáni adatok
		0				// a 0. indexű attribútum hol kezdődik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		(GLuint)1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) ); //void* kompabilitás miatt, logikát nem változtat
									  // = az elsp vec3 után olvassa ki a színt

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le, ne módosítjuk véletlen se
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	////////////////////////////////////////////////////////////
	// ÚJ VBO BEVEZETÉSE KÖRHÖZ ////////////////////////////////
	

	// SIMA KÖRLAP:
	//const int N = 30;
	//const float r = 0.4;
	//Vertex circle[N + 2]; //csúcsok + középpont + záró pont
	//circle[0] = { glm::vec3(0,0,0), glm::vec3(1,0,1) };

	//for (int i = 0; i < N + 2; i++) {
	//	float phi = 2 * M_PI / N * i; //az éppeni háromszöghöz a szög
	//	circle[i] = { r * glm::vec3( cosf(phi), sinf(phi) * 4/3, -0.1 ),
	//					glm::vec3(1,0,1) };
	//		//4/3: hogy default képarányban tényleg kör legyen
	//}

	// SZORGALMI: KÖR GYŰRŰ
	const int N = 60;
	const float r = 0.4;
	const float ringWidth = 0.1;

	Vertex circle[N + 2]; //belső csúcsok + külső csúcsok + kezdő + záró pont
	bool isOuter = false;

	for (int i = 0; i < N + 2 ; i++) {
		float phi = 2 * M_PI / N * i; //az éppeni háromszöghöz a szög
		float x = cosf(phi);
		float y = sinf(phi) * 4/3; //TODO: dynamic képarány
		float curR = isOuter ? r + ringWidth : r;
		circle[i] = { curR * glm::vec3( x, y, -0.1 ),
						glm::vec3(1,0,1) };

		isOuter = !isOuter;

	}


	// 1 db VAO foglalása
	glGenVertexArrays(1, &m_vaoID2);
	// a frissen generált VAO beállítasa aktívnak
	glBindVertexArray(m_vaoID2);

	// hozzunk létre egy új VBO erőforrás nevet
	glGenBuffers(1, &m_vboID2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID2); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
		//ezzel másoljuk át az adatot GPUra
	glBufferData(GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
		sizeof(circle),		// ennyi bájt nagyságban
		circle,	// erről a rendszermemóriabeli címről olvasva
		GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat
						  //"milyen módon érjük el és módosítjuk az adatot"
						  // = egyszer fogja beleírni és sokat olvasunk mód


// VAO-ban jegyezzük fel, hogy a VBO-ban az első 3 float sizeof(Vertex)-enként lesz az első attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben található adatok közül a 0. "indexű" attribútumait állítjuk be
		3,				// komponens szám
		GL_FLOAT,		// adatok típusa
		GL_FALSE,		// normalizált legyen-e
		sizeof(Vertex),	// stride (0=egymás után) = milyen "közökkel" találhatóak meg az egymás utáni adatok
		0				// a 0. indexű attribútum hol kezdődik a sizeof(Vertex)-nyi területen belül
	);

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		(GLuint)1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3))); //void* kompabilitás miatt, logikát nem változtat
									  // = az elsp vec3 után olvassa ki a színt

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le, ne módosítjuk véletlen se
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le


	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// attributomok osszerendelese a VAO es shader kozt
		//így adjuk meg a shader bemeneti változóit
	glBindAttribLocation( m_programID, 0, "vs_in_pos");
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenő-bemenő változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkelés ellenőrzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, VertexShaderErrorMessage.data());

		std::cerr << "[glLinkProgram] Shader linking error:\n" << &VertexShaderErrorMessage[0] << std::endl;
	}

	// már nincs ezekre szükség
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolása, ebben a projektben a teljes programot jelöli, hisz nem váltunk a shaderek között
	glUseProgram( m_programID );

	glBindVertexArray(m_vaoID2); //circle vao

	GLuint locTime = glGetUniformLocation(m_programID, "time");
	GLuint locCircle = glGetUniformLocation(m_programID, "circle");
	glUniform1f(locTime, SDL_GetTicks() / 1000.f);
	glUniform1i(locCircle, 1);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 62); //kör gyűrú

	// kirajzolás
	//A draw hívásokhoz a VAO és a program bindolva kell legyenek (glUseProgram() és glBindVertexArray())
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUniform1i(locCircle, 0);


	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID); //háttér vao

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// VAO kikapcsolása
	glBindVertexArray(0);

	// shader kikapcsolása
	glUseProgram( 0 );
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