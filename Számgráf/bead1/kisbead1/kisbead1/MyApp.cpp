#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp()
{
}


CMyApp::~CMyApp()
{
}

//
// egy parametrikus felület (u,v) paraméterértékekhez tartozó pontjának
// kiszámítását végző függvény
//
glm::vec3 CMyApp::GetSpherePos(float u, float v)
{
	// origó középpontú, egységsugarú gömb parametrikus alakja: http://hu.wikipedia.org/wiki/G%C3%B6mb#Egyenletek 
	// figyeljünk:	matematikában sokszor a Z tengely mutat felfelé, de nálunk az Y, tehát a legtöbb képlethez képest nálunk
	//				az Y és Z koordináták felcserélve szerepelnek
	//GÖMB:
	/*u *= 3.1415f;
	v *= 3.1415f;
	float r = 2;*/

	//return glm::vec3( r*sin(v)*cos(u),
	//				  r*cos(v),
	//				  r*sin(v)*sin(u));

	u *= 2 * 3.1415f;
	v *= 2 * 3.1415f;

	float r = 0.75f;
	float R = 4;

	return glm::vec3((R + r * cos(u)) * cos(v),
		r * sin(u),
		(R + r * cos(u)) * sin(v));
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

	//
	// geometria letrehozasa
	//
	// KOCKA VERTEXEI
	Vertex vert[] =
	{
		//position			 , color				//alsó lap
		{glm::vec3(-unitL, -unitL, -unitL),	glm::vec3(1, 0, 0)},	//bal felső
		{glm::vec3(+unitL, -unitL, -unitL),	glm::vec3(0, 1, 0)},	//jobb felső
		{glm::vec3(-unitL, -unitL, +unitL),	glm::vec3(0, 0, 1)},	//bal alsó
		{glm::vec3(+unitL, -unitL, +unitL),	glm::vec3(0, 1, 0)},	//jobb alsó
													//felső lap
		{glm::vec3(-unitL,  unitL, -unitL),	glm::vec3(1, 1, 1)},	//bal felső
		{glm::vec3(+unitL,  unitL, -unitL),	glm::vec3(0, 0, 0)},	//jobb felső
		{glm::vec3(-unitL,  unitL, +unitL),	glm::vec3(1, 1, 0)},	//bal alsó
		{glm::vec3(+unitL,  unitL, +unitL),	glm::vec3(0, 1, 1)},	//jobb alsó
	};

	// kocka indexpuffer adatai, GL_TRIANGLES-el
	GLushort indices[] =
	{
		// alsó 
		2,0,1,
		3,2,1,
		//side hátra
		7,6,2,
		7,2,3,
		//side jobb
		7,3,1,
		7,1,5,
		//side előre
		4,5,1,
		4,1,0,
		//side bal
		4,0,2,
		4,2,6,
		//felső
		4,6,7,
		4,7,5
	};
	numOfIndices = (sizeof(indices) / sizeof(*indices));

#pragma region shader/vertex loading

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);

	// hozzunk létre egy új VBO erőforrás nevet
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
		sizeof(vert),		// ennyi bájt nagyságban
		vert,	// erről a rendszermemóriabeli címről olvasva
		GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat


// VAO-ban jegyezzük fel, hogy a VBO-ban az első 3 float sizeof(Vertex)-enként lesz az első attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexű" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymás után)
		0				// a 0. indexű attribútum hol kezdődik a sizeof(Vertex)-nyi területen belül
	);

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	// index puffer létrehozása
	glGenBuffers(1, &m_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés előtt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonosítója, amiből egy változóhoz szeretnénk hozzárendelést csinálni
		0,				// a VAO-beli azonosító index
		"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation(m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenő-bemenő változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, VertexShaderErrorMessage.data());
		std::cerr << "[glLinkProgram] Shader linking error:\n" << &VertexShaderErrorMessage[0] << std::endl;
	}
	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	//
	// egyéb inicializálás
	//

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_mvp = glGetUniformLocation(m_programID, "MVP");

#pragma endregion shader/vertex loading

	// kamera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram(m_programID);
}

void CMyApp::Update()
{
	//// nézeti transzformáció beállítása
	//float t = SDL_GetTicks() / 1000.0f;
	//m_matView = glm::lookAt(glm::vec3(10 * cos(t), 10, 10 * sin(t)),		// honnan nézzük a színteret
	//	glm::vec3(0, 0, 0),		// a színtér melyik pontját nézzük
	//	glm::vec3(0, 1, 0));		// felfelé mutató irány a világban

	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa, ebben a projektben a teljes programot jelöli, hisz nem váltunk a shaderek között
	glUseProgram(m_programID);

	// shader parameterek beállítása
	/*

	GLM transzformációs mátrixokra példák:
		glm::rotate<float>( szög, glm::vec3(tengely_x, tengely_y, tengely_z) ) <- tengely_{xyz} körüli elforgatás
		glm::translate<float>( glm::vec3(eltol_x, eltol_y, eltol_z) ) <- eltolás
		glm::scale<float>( glm::vec3(s_x, s_y, s_z) ) <- léptékezés

	*/

	glm::mat4 viewProj;
	glm::mat4 mvp;

	glm::vec3 pyramid_trans[] = {
		glm::vec3(unitL * -4, 0, unitL * -4), //bal felső
		glm::vec3(unitL * +4, 0, unitL * -4), //jobb felső
		glm::vec3(unitL * -4, 0, unitL * +4), //bal alsó
		glm::vec3(unitL * +4, 0, unitL * +4), //jobb alsó
		glm::vec3(0, unitL * 4, 0) //középső felül
		//TODO: calculate élhossz for felső
	};

	// KÖZÉPSŐ ALAKZAT ///////////////////////////////////////////////

	for (glm::vec3 translation : pyramid_trans) {
		m_matWorld = glm::translate<float>(translation) * glm::mat4(1.0f);
		viewProj = m_camera.GetViewProj(); // returns projective * view
		mvp = viewProj * m_matWorld;

		glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &(mvp[0][0]));
		glBindVertexArray(m_vaoID);

		glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_SHORT, 0);	
	}


	// KÖZÉPSŐ ALAKZAT ///////////////////////////////////////////////

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram(0);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
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
	glViewport(0, 0, _w, _h);

	// vetítési mátrix beállítása
	//m_matProj = glm::perspective(glm::radians(60.0f),	// 60 fokos nyílásszög radiánban
	//	_w / (float)_h,			// ablakméreteknek megfelelő nézeti arány
	//	0.01f,					// közeli vágósík
	//	1000.0f);				// távoli vágósík

	m_camera.Resize(_w, _h);
}