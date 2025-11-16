#include "Scene3.h"

CScene3::CScene3()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	pWoodHouse = NULL;
	
	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();	
	pTextures->CreateTextureMipMap(0, "../Scene/txTer.jpg");
	pTextures->CreateTextureAnisotropic(1, "../Scene/water.bmp");
	pTextures->CreateTextureAnisotropic(2, "../Scene/txOil1.jpg");
	pTextures->CreateTextureAnisotropic(3, "../Scene/txOil2.jpg");


	pTexturesSkybox = new CTexture();
	pTexturesSkybox->CreateTextureClamp(0, "../Scene/skybox/front.bmp");// Frente
	pTexturesSkybox->CreateTextureClamp(1, "../Scene/skybox/back.bmp"); // Trás
	pTexturesSkybox->CreateTextureClamp(2, "../Scene/skybox/down.bmp"); // Baixo
	pTexturesSkybox->CreateTextureClamp(3, "../Scene/skybox/up.bmp");  
	pTexturesSkybox->CreateTextureClamp(4, "../Scene/skybox/left.bmp"); // Esquerda
	pTexturesSkybox->CreateTextureClamp(5, "../Scene/skybox/right.bmp");// Direita

	pTerreno = NULL;
	pTerreno = new CTerreno();
	pTerreno->LoadRawFile("../Scene/terreno.raw");
	pTerreno->CreateHeightMapDisplayList(false, false);


	pWoodHouse = new CModel_3DS();
	pWoodHouse->Load("../Scene/Casa.3ds");

	// Luz da casa
	LightAmbient[0] = 0.0f;		LightAmbient[1] = 0.0f;		LightAmbient[2] = 0.0f;	LightAmbient[3] = 1.0f;
	LightDiffuse[0] = 1.0f;		LightDiffuse[1] = 1.0f;		LightDiffuse[2] = 1.0f;		LightDiffuse[3] = 1.0f;
	LightSpecular[0] = 1.0f;	LightSpecular[1] = 1.0f;	LightSpecular[2] = 1.0f;	LightSpecular[3] = 1.0f;
	LightPosition[0] = 0.0f;	LightPosition[1] = 3.7f;	LightPosition[2] = 0.0f;	LightPosition[3] = 1.0f;
	LightDirection[0] = 0.0f;	LightDirection[1] = -1.0f;	LightDirection[2] = 0.0f;
	LightPositionSky[0] = 30.0f;	LightPositionSky[1] = 30.0f;	LightPositionSky[2] = 20.0f;	LightPositionSky[3] = 1.0f;
	LightPositionSky2[0] = -30.0f;	LightPositionSky2[1] = 30.0f;	LightPositionSky2[2] = -20.0f;	LightPositionSky2[3] = 1.0f;

	habiliteFog = false;
	fFogColor[0] = 0.611f;
	fFogColor[1] = 0.760f;
	fFogColor[2] = 1.0f;
	fFogColor[3] = 1.0f;

}


CScene3::~CScene3(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}	

	if (pWoodHouse)
	{
		delete pWoodHouse;
		pWoodHouse = NULL;
	}
	if (pTerreno)
	{
		delete pTerreno;
		pTerreno = NULL;
	}
	if (pTexturesSkybox)
	{
		delete pTexturesSkybox;
		pTexturesSkybox = NULL;
	}

}




int CScene3::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter
	
	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();

	// Desenha grid 
	Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do sistema cartesiano
	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Desenha a L�mpada
	glPushMatrix();
	glTranslatef(LightPosition[0], LightPosition[1], LightPosition[2]);
	auxSolidSphere(0.3);
	glPopMatrix();


	glEnable(GL_TEXTURE_2D);

	// Sybox
	// Desenha o SkyBox
	CreateSkyBox(0.0f, 100.0f, 0.0f, // Origem
		2000.0f, 2000.0f, 2000.0f,	// Largura, Altura e Comprimento
		pTexturesSkybox);					// Gerador de texturas


	if (habiliteFog) ShowUpFog();

	// Carregando Terreno
	pTextures->ApplyTexture(0);

	glPushMatrix();
	glTranslatef(-400.0f, -61.0f, -850.0f);
	//glTranslatef(0.0f, 0.0f, 0.0f);
	pTerreno->RenderHeightMapDisplayList();
	glPopMatrix();


	// Criando o RIO
	// Habilita Blending
	glEnable(GL_BLEND);
	// Configura função de Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

	glPushAttrib(GL_TEXTURE_BIT);
	// Unidade de Textura 0
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);


	// Unidade de Textura 1
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

	glPushMatrix();
	glTranslatef(0.0f, -20.0f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f + fRenderPosY);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fRenderPosY, 0.0f);
	glVertex3f(-400.0f, 0.0f, -1100.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 5.0f, 0.0f + fRenderPosY);
	glMultiTexCoord2f(GL_TEXTURE1, 5.0f + fRenderPosY, 0.0f);
	glVertex3f(-400.0f, 0.0f, 200.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 5.0f, 5.0f + fRenderPosY);
	glMultiTexCoord2f(GL_TEXTURE1, 5.0f + fRenderPosY, 5.0f);
	glVertex3f(600.0f, 0.0f, 200.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 5.0f + fRenderPosY);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fRenderPosY, 5.0f);
	glVertex3f(600.0f, 0.0f, -1100.0f);
	glEnd();
	glPopMatrix();

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glPopAttrib();
	glDisable(GL_BLEND);


	// Habilita ilumina��o global (PHONG)
	glEnable(GL_LIGHTING);

	// Habilita a fonte de luz (l�mpada 0)
	glEnable(GL_LIGHT0);
	// Habilita a fonte de luz (l�mpada 1)
	glEnable(GL_LIGHT1);
	// Habilita a fonte de luz (l�mpada 2)
	glEnable(GL_LIGHT2);

	// Atribuindo os valores para os componentes de defini��o da l�mpada
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT2, GL_POSITION, LightPositionSky2);

	// Atribuindo os valores para os componentes de defini��o da l�mpada
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPositionSky);

	// Atribuindo os valores para os componentes de defini��o da l�mpada
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	MatAmbient[0] = 0.24725f;	MatAmbient[1] = 0.1995f;	MatAmbient[2] = 0.0745f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.75164f;	MatDiffuse[1] = 0.60648f;	MatDiffuse[2] = 0.22648f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.628281f;	MatSpecular[1] = 0.555802f;	MatSpecular[2] = 0.366065f;	MatSpecular[3] = 1.0f;
	MatShininess = 51.2f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	// Carregando casa de madeira
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0);
	glRotatef(-75.0f, 0.0f, 1.0f, 0.0f);
	pWoodHouse->Draw();
	glPopMatrix();

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHTING);



	// Criando Barris
	DrawBarrel(100.0f, -1.0f, 0.0f);
	DrawBarrel(120.0f, -4.0f, -10.0f);
	DrawBarrel(95.0f, -2.5f, -15.0f);



	glDisable(GL_TEXTURE_2D);

	// Caso o fog tiver habilitado, desativa ele no fim da geração da cena
	if (habiliteFog) glDisable(GL_FOG);

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.0005f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));


	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene3::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene3::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}



}

void CScene3::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;
	case 'F':
		habiliteFog = !habiliteFog;
		break;
	case VK_RETURN:
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene3::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene3::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}

void CScene3::CreateSkyBox(float x, float y, float z,
	float width, float height, float length,
	CTexture* pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posição especificada(x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Aplica a textura que representa a parte da frente do skybox (BACK map)
	pTextures->ApplyTexture(0);

	// Desenha face BACK do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
	pTextures->ApplyTexture(1);

	// Desenha face FRONT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
	pTextures->ApplyTexture(2);

	// Desenha face BOTTOM do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (UP map)
	pTextures->ApplyTexture(3);

	// Desenha face TOP do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
	pTextures->ApplyTexture(4);

	// Desenha face LEFT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
	pTextures->ApplyTexture(5);

	// Desenha face RIGHT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}

void CScene3::ShowUpFog() 
{
	// Habilita Neblina
	glEnable(GL_FOG);
	// Cor da Neblina
	glFogfv(GL_FOG_COLOR, fFogColor);
	// Ponto Inicial da Neblina a partir da câmera
	glFogf(GL_FOG_START, 1.0f);
	// Ponto Final da Neblina a partir da câmera
	glFogf(GL_FOG_END, 500.0f);
	// Tipo da Neblina
	glFogi(GL_FOG_MODE, GL_LINEAR);
}

void CScene3::DrawBarrel(float x, float y, float z)
{
	GLUquadric* quad = gluNewQuadric();

	float size = 5.0f;
	float radius = 1.5f;

	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	// Fechamento do barril
	pTextures->ApplyTexture(3);
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(180, 1.0f, 0.0f, 0.0f);
	gluDisk(quad, 0.0f, radius, 32, 2);
	glPopMatrix();

	// Corpo do Barril
	pTextures->ApplyTexture(2);
	glPushMatrix();
	glTranslatef(x, y, z);
	gluCylinder(quad, radius, radius, size, 32, 32);
	glPopMatrix();

	// Fechamento do barril
	pTextures->ApplyTexture(3);
	glPushMatrix();
	glTranslatef(x, y, z+size);
	gluDisk(quad, 0.0f, radius, 32, 1);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}



