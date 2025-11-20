#pragma once
#include "SceneBaseClass.h"
#include "CTexture.h"
#include "CTimer.h"
#include "CTexto.h"
#include "CCamera.h"
#include "CModel_3DS.h"
#include "CTerreno.h"



class CScene3 : public CSceneBaseClass
{
public:
	CScene3();
	~CScene3(void);


	virtual void MouseMove(void);					// Tratamento de movimento do mouse
	virtual void KeyPressed(void);					// Tratamento de teclas pressionadas
	virtual void KeyDownPressed(WPARAM	wParam);	// Tratamento de teclas pressionadas
	virtual int DrawGLScene(void);					// Função que desenha a cena

	void Draw3DSGrid(float width, float length);
	void DrawAxis();
	void DrawBarrel(float x, float y, float z);
	void DrawSpool(float x, float y, float z, float rotation);
	void DrawCylinderNormalMapped(float radius, float height, int slices);
	void CreateSkyBox(float x, float y, float z,
		float width, float height, float length,
		CTexture* pTextures);
	void ShowUpFog();

	
private:

	bool	keys[256];		// Array usado para rotinas do teclado
	bool	active;			// Window Active Flag Set To TRUE By Default
	bool	fullscreen;		// Exibir janela em modo fullscreem (TRUE) ou em janela (FALSE)
	bool    habiliteFog;	// Habilita a neblina (FOG)
	bool	houseLightOn;


	CCamera* pCamera;	// Gerencia câmera OpenGL
	float fDeltaY;				// Rotação da câmera OpenGL no eixo Y
	float fDeltaX;				// Rotação da câmera OpenGL no eixo X

	CTexto* pTexto;	// Objeto que gerencia texto
	CTexture* pTextures;	// Objeto que gerencia texturas
	CTexture* pTexturesSkybox;
	CTimer* pTimer;	// Objeto que gerencia o timer
	CTerreno* pTerreno; // Objeto que gerencia o terreno


	int		iFPS;			// FPS and FPS Counter
	int		iFrames;		// FPS and FPS Counter
	DWORD	ulLastFPS;		// FPS and FPS Counter
	char	szTitle[256];	// FPS and FPS Counter


	bool bIsWireframe;	// Modos Wireframe/Solid
	bool bIsCameraFPS;	// Ativa modo de camera First Person Shooter (true) ou Third Person Shooter (false)

	float fRenderPosY;
	float fTimerPosY;
	float fFogColor[4];

	GLfloat LightAmbient[4];
	GLfloat LightDiffuse[4];
	GLfloat LightSpecular[4];
	GLfloat LightPosition[4];
	GLfloat LightPositionSky[4];
	GLfloat LightPositionSky2[4];
	GLfloat LightDirection[3];
	GLfloat LightDirectionCam[3];
	// Definindo as propriedades do material
	GLfloat MatAmbient[4];
	GLfloat MatDiffuse[4];
	GLfloat MatSpecular[4];
	GLfloat MatShininess;


	CModel_3DS* pWoodHouse;

};

