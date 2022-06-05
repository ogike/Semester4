#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	// shaderekhez szükséges változók
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader

	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO

	VertexArrayObject	m_SphereVao;			// VAO
	IndexBuffer			m_SphereIndices;		// index buffer
	ArrayBuffer			m_SphereVertexBuffer;	// VBO

	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		

	gCamera				m_camera;

	Texture2D			m_woodTexture;
	Texture2D			m_suzanneTexture;

	Texture2D			m_mudTexture;
	Texture2D			m_rockTexture;
	Texture2D			m_grassTexture;

	TextureCubeMap		m_skyboxTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// mesh adatok
	std::unique_ptr<Mesh> m_mesh;

	// segédfüggvények gömbhöz
	glm::vec3 GetSpherePos(float u, float v, float d);
	glm::vec3 GetNorm(float u, float v);
	glm::vec2 GetTex(float u, float v);

	// NxM darab négyszöggel közelítjük a gömböt felületünket => (N+1)x(M+1) pontban kell kiértékelni
	static const int N = 60;
	static const int M = 30;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitSphere();
	void InitSphereOld();
	GLuint m_vaoID = 0; // vertex array object erőforrás azonosító
	GLuint m_vboID = 0; // vertex buffer object erőforrás azonosító
	GLuint m_ibID = 0;  // index buffer object erőforrás azonosító
	void InitSkyBox();
	void InitCube();

	float r;
};

