#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
}
//dodato
CGLRenderer::CGLRenderer(void)
{
	xRotationAngle = 0;
	yRotationAngle = 0;
	zoom = -10;
	moonYRotationAngle = 0;
	lightOn = false;
}
CGLRenderer::~CGLRenderer(void)
{
}
bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}
void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
//dodato
void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	env = LoadTexture("env.png");
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
//za zemlju
void GLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);



	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);



	T[0] = LoadTexture(".\\Resources\\TSC0.jpg");
	T[1] = LoadTexture(".\\Resources\\TSC1.jpg");
	T[2] = LoadTexture(".\\Resources\\TSC2.jpg");
	T[3] = LoadTexture(".\\Resources\\TSC3.jpg");
	T[4] = LoadTexture(".\\Resources\\TSC4.jpg");
	T[5] = LoadTexture(".\\Resources\\TSC5.jpg");



	S[0] = LoadTexture(".\\Resources\\S0.jpg");
	S[1] = LoadTexture(".\\Resources\\S1.jpg");
	S[2] = LoadTexture(".\\Resources\\S2.jpg");
	S[3] = LoadTexture(".\\Resources\\S3.jpg");
	S[4] = LoadTexture(".\\Resources\\S4.jpg");
	S[5] = LoadTexture(".\\Resources\\S5.jpg");



	M[0] = LoadTexture(".\\Resources\\M0.jpg");
	M[1] = LoadTexture(".\\Resources\\M1.jpg");
	M[2] = LoadTexture(".\\Resources\\M2.jpg");
	M[3] = LoadTexture(".\\Resources\\M3.jpg");
	M[4] = LoadTexture(".\\Resources\\M4.jpg");
	M[5] = LoadTexture(".\\Resources\\M5.jpg");



	glEnable(GL_TEXTURE_2D);



	wglMakeCurrent(NULL, NULL);
}



// FOV se menja u reshape
void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, (double)w / (double)h, 1.0, 2000);
	glMatrixMode(GL_MODELVIEW);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);



	glDeleteTextures(6, T);
	glDeleteTextures(6, S);
	glDeleteTextures(6, M);



	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(1*dist, 1*dist, 1*dist, 0, 0, 0, 0, 1, 0);
	glRotatef(alfa, 1, 0, 0);
	glRotatef(beta, 0, 1, 0);

	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	DrawEnv(3, 3);
	glEnable(GL_DEPTH_TEST);
	//ucitaj texture;

	glEnable(GL_TEXTURE_2D);
	DrawAxis();
	DrawSphere(0.5, 60, 1, 1, 1);
	glTranslatef(0, 1, 0);
	DrawSphere(0.5, 60, 1, 1, 1);
	DrawAxis();
	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
//dodato sa svetlo
void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();



	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);



	glPushMatrix();
	glRotatef(xRotationAngle, 1.0, 0.0, 0.0);
	glRotatef(yRotationAngle, 0.0, 1.0, 0.0);
	DrawSpace(1.0, 20);
	glPopMatrix();



	if (lightOn)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}



	float light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };



	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);



	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };



	glEnable(GL_DEPTH_TEST);
	glTranslatef(0, 0, zoom);
	glRotatef(xRotationAngle, 1.0, 0.0, 0.0);
	glRotatef(yRotationAngle, 0.0, 1.0, 0.0);



	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	DrawEarth(3, 20);
	glTranslatef(-50, 0, 0);
	glRotatef(moonYRotationAngle, 0.0, 1.0, 0.0);
	DrawMoon(1.0, 20);



	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DrawEnv(double h, double a)
{
	double start = 0.0, offset = 1.0 / 4.0;
	glBindTexture(GL_TEXTURE_2D, env);
	glPushMatrix();
	{
		for (int i = 0; i < 4; i++) {
			glBegin(GL_QUADS);
			{
				glTexCoord2f(start, 2.0 / 3.0);
				glVertex3f(-a / 2, h / 2, a / 2);
				glTexCoord2f(start, 1.0 / 3.0);
				glVertex3f(-a / 2, h / 2, -a / 2);
				glTexCoord2f(start + offset, 1.0 / 3.0);
				glVertex3f(-a / 2, -h / 2, -a / 2);
				glTexCoord2f(start + offset, 2.0 / 3.0);
				glVertex3f(-a / 2, -h / 2, a / 2);
				start += offset;
			}
			glEnd();
			glRotatef(90, 0.0, 0.0, 1.0);
		}
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(90, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(2 * offset, 1);
			glVertex3f(-a / 2, h / 2, a / 2);
			glTexCoord2f(1 * offset, 1);
			glVertex3f(-a / 2, h / 2, -a / 2);
			glTexCoord2f(offset, 2.0 / 3.0);
			glVertex3f(-a / 2, -h / 2, -a / 2);
			glTexCoord2f(2 * offset, 2.0 / 3.0);
			glVertex3f(-a / 2, -h / 2, a / 2);
		}
		glEnd();

		glRotatef(-180, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(offset, 0.0);
			glVertex3f(-a / 2, h / 2, a / 2);
			glTexCoord2f(2 * offset, 0.0);
			glVertex3f(-a / 2, h / 2, -a / 2);
			glTexCoord2f(2 * offset, 1.0 / 3.0);
			glVertex3f(-a / 2, -h / 2, -a / 2);
			glTexCoord2f(offset, 1.0 / 3.0);
			glVertex3f(-a / 2, -h / 2, a / 2);
		}
		glEnd();
	}
	glPopMatrix();
}
void CGLRenderer::DrawAxis()
{
	glLineWidth(2.0);
	//glDisable(GL_TEXTURE_2D); 

	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);

	glEnd();
}
////////////////////////////////////////////////////////////////////////////////////////////

//crta valjak
void CGLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	float step = 2 * 3.14 / n;
	//gornja baza
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		glVertex3f(r1 * cos(i * step), h, r1 * sin(i * step));
	}
	glEnd();

	//donja baza
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		glVertex3f(r2 * cos(i * step), 0, r2 * sin(i * step));
	}
	glEnd();



	//omotac

	//glBindTexture(GL_TEXTURE_2D, //brick)
		//; 
	float texture_step = 1.0 / n;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= n; i++) {

		//glTexCoord2f(i * texture_step, 0);
		glVertex3f(r1 * cos(i*step), h, r1*sin(i*step));

		//glTexCoord2f(i * texture_step, 1);
		glVertex3f(r2 * cos(i * step), 0, r2 * sin(i * step));
	}
	glEnd();
}
//dodato vetrenjaca valjak
void GLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	glBegin(GL_QUAD_STRIP);
	{
		float texPart = 1.0 / (float)n; // kad imamo n segmenata nekog tela
		float angleStep = (2 * pi) / (float)n;  //kad nam treba neki ugao, okrugli objekat 
		float currAngle = 0;
		float y1 = h / 2, y2 = -h / 2;
		for (int i = 0; i <= n; i++)
		{
			float x1 = r1 * cos(currAngle);
			float z1 = r1 * sin(currAngle);
			float x2 = r2 * cos(currAngle);
			float z2 = r2 * sin(currAngle);



			float rDiff = r2 - r1;// ove dve pisem  da bih nasla ny i nr (normale)
			float L = sqrt(h * h + rDiff * rDiff);
			float ny = rDiff / L;
			float nr = h / L;
			float nx = nr * cos(currAngle);
			float nz = nr * sin(currAngle);



			glNormal3f(nx, ny, nz);



			glTexCoord2f(i * texPart, 0.0);
			glVertex3f(x1, y1, z1);



			glTexCoord2f(i * texPart, 1.0);
			glVertex3f(x2, y2, z2);



			currAngle += angleStep;
		}
	}
	glEnd();
}

//crta loptu
void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	int i, j;
	double stepLatitude=2*3.14/nSeg, stepLongitude=3.14*nSeg;
	double ang1, ang2;
	ang1 = -3.14 / 2;

	for (int i = 0; i < nSeg; i++)
	{
		ang2 = 0;
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j < nSeg + 1; j++)
		{
			double x1 = r * cos(ang1) * cos(ang2);
			double y1 = r * sin(ang1);
			double z1 = r * cos(ang1) * sin(ang2);

			double x2 = r * cos(ang1+stepLongitude) * cos(ang2);
			double y2 = r * sin(ang1+stepLongitude);
			double z2 = r * cos(ang1 + stepLongitude) * sin(ang2);

			double tx1 = texR * x1 / r + texU;
			double ty1 = texR * z1 / r + texV;
			double tx2 = texR * x2 / r + texU;
			double ty2 = texR * y2 / r + texV;

			//normale x,y,z/r
			//glTexCoord2f(tx1, ty1);
			glVertex3d(x1, y1, z1);
			//glTexCoord2f(tx2, ty2);
			glVertex3d(x2, y2, z2);

			ang2 += stepLatitude;
		}
		glEnd();
		ang1 += stepLongitude;
	}
}

//crta kupu
void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double ang1, ang2;
	double step = 3.14 * 2 / nSeg;
	ang1 = 0;

	//bind fali
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 1.0, 1.0);
	//glTexCoord2f(texU, texV);
	glVertex3d(0.0, h, 0.0);

	for (int i = 0; i < nSeg + 1; i++)
	{
		double x = r * cos(ang1);
		double z = r * sin(ang1);

		double tx = texR * x / r + texU;
		double ty = texR * z / r + texV;

		//glTexCoord2f(tx, ty);
		glVertex3d(x, 0, z);

		ang1 += step;
	}
	glEnd();
}

//crta prizmu
void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	DrawPolygon(points, texCoords, n);

	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	for (int i = 0; i < n; i++) {
		glVertex3f(points[i].x, points[i].y, 0);
		glVertex3f(points[i].x, points[i].y, zh);
		glVertex3f(points[(i + 1) % n].x, points[(i + 1) % n].y, zh);
		glVertex3f(points[(i + 1) % n].x, points[(i + 1) % n].y, 0);
	}

	glEnd();
	glPushMatrix();
	glTranslatef(0, 0, zh);
	DrawPolygon(points, texCoords, n);
	glPopMatrix();
}

//crta poligon, koristi se za prizmu
void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < n; i++) {
		glTexCoord2f(texCoords[i].x, texCoords[i].y);
		glVertex3f(points[i].x, points[i].y, 0);
	}
	glEnd();
}

//dodato sve sto je ispod
void CGLRenderer::DrawAxis()
{
	glLineWidth(2.0);



	glBegin(GL_LINES);



	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);



	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);



	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);



	glEnd();
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));



	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);



	glBindTexture(GL_TEXTURE_2D, texID);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);



	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);



	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());



	return texID;
}

void CGLRenderer::RotateView(int xAngle, int yAngle)
{
	xRotationAngle += xAngle;
	if (xRotationAngle < 0)
		xRotationAngle += 360;
	else if (xRotationAngle > 360)
		xRotationAngle -= 360;



	yRotationAngle += yAngle;
	if (yRotationAngle < 0)
		yRotationAngle += 360;
	else if (yRotationAngle > 360)
		yRotationAngle -= 360;
}

void CGLRenderer::Zoom(float zoomAmount)
{
	zoom += zoomAmount;
	if (zoom > -7.0)
		zoom = -7.0;
}


//crtanje zemlja
void GLRenderer::DrawPatch(double R, int n)
{
	double step = (double)2 / (double)n;
	double paramY = 1.0;
	for (int i = 0; i < n; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			double paramX = -1.0;
			double paramYlower = paramY - step;
			for (int j = 0; j <= n; j++)
			{
				//paramX += j * step;
				double phi = atan(paramX);
				double theta1 = atan(paramY * cos(phi));



				double x1 = R * cos(theta1) * sin(phi);
				double z1 = R * cos(theta1) * cos(phi);
				double y1 = R * sin(theta1);



				glNormal3f(x1 / R, y1 / R, z1 / R);
				float yTex = (paramY + 1.0) / 2.0;
				glTexCoord2f((paramX + 1.0) / 2.0, 1.0 - yTex);
				glVertex3f(x1, y1, z1);




				phi = atan(paramX);
				double theta2 = atan(paramYlower * cos(phi));



				double x2 = R * cos(theta2) * sin(phi);
				double z2 = R * cos(theta2) * cos(phi);
				double y2 = R * sin(theta2);



				glNormal3f(x2 / R, y2 / R, z2 / R);
				yTex = (paramYlower + 1.0) / 2.0;
				glTexCoord2f((paramX + 1.0) / 2.0, 1.0 - yTex);
				glVertex3f(x2, y2, z2);



				paramX += step;
			}
		}
		glEnd();
		paramY -= step;
	}
}
void GLRenderer::DrawPatch2(double R, int n)
{
	double TexRazmak = (double)2 / double(n);

	double y = 1.0;




	for (int i = 0; i < n; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			double x = -1.0;
			for (int j = 0; j <= n; j++)
			{



				double phi = atan(x);
				double theta = atan(y * cos(phi));



				double xd = R * cos(theta) * sin(phi);
				double yd = R * cos(theta) * cos(phi);
				double zd = R * sin(theta);



				glNormal3f(xd / R, yd / R, zd / R);
				glTexCoord2f((x + 1.0) / 2.0, 1.0 - ((y + 1.0) / 2.0));
				glVertex3f(xd, yd, yd);



				phi = atan(x);
				double theta2 = atan((y - TexRazmak) * cos(phi));



				double x2 = R * cos(theta2) * sin(phi);
				double z2 = R * cos(theta2) * cos(phi);
				double y2 = R * sin(theta2);



				y = (y - TexRazmak + 1.0) / 2.0;
				glNormal3f(x2 / R, y2 / R, z2 / R);
				glTexCoord2f((x + 1.0) / 2.0, 1.0 - y);
				glVertex3f(x2, y2, y2);



				x += TexRazmak;



			}
		}
		glEnd();
		y -= TexRazmak;
	}




}
void GLRenderer::DrawEarth(double R, int tes)
{
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glRotatef(i * 90, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, T[i]);
		DrawPatch(R, tes);
		glPopMatrix();
	}



	for (int i = 0; i < 2; i++)
	{
		glPushMatrix();
		glRotatef((1 + i * 2) * 90, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, T[5 - i]);
		DrawPatch(R, tes);
		glPopMatrix();
	}
}
void GLRenderer::DrawMoon(double R, int tes)
{
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glRotatef(i * 90, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, M[i]);
		DrawPatch(R, tes);
		glPopMatrix();
	}



	for (int i = 0; i < 2; i++)
	{
		glPushMatrix();
		glRotatef((1 + i * 2) * 90, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, M[5 - i]);
		DrawPatch(R, tes);
		glPopMatrix();
	}
}
void GLRenderer::DrawSpace(double R, int tes)
{
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glRotatef(i * 90, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, S[i]);
		DrawPatch(R, tes);
		glPopMatrix();
	}



	for (int i = 0; i < 2; i++)
	{
		glPushMatrix();
		glRotatef((1 + i * 2) * 90, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, S[5 - i]);
		DrawPatch(R, tes);
		glPopMatrix();
	}
	glEnable(GL_CULL_FACE);
}


//crtanje pauk
void CGLRenderer::DrawLegSegment(double r, double h, int nSeg) {
	glPushMatrix();
	glTranslatef(0, r, 0);
	DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.24);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.24);
	glPopMatrix();
}
void CGLRenderer::DrawLeg() {
	double ang = 85;
	glPushMatrix();
	DrawLegSegment(1, 10, 5);
	glTranslatef(0, 11, 0);
	glRotatef(ang, 1.0, 0.0, 0.0);
	DrawLegSegment(1, 15, 5);
	glPopMatrix();
}
void CGLRenderer::DrawSpiderBody() {
	double ang = 45;
	glPushMatrix();
	/// CENTER
	glPushMatrix();
	glScalef(1.0, 0.5, 1.0);
	DrawSphere(3, 10, 0.25, 0.25, 0.24);
	glPopMatrix()
		/// TAIL
	glPushMatrix();
	glTranslatef(6.0, 0.0, 0.0);
	glScalef(1.0, 0.8, 1.0);
	DrawSphere(5, 10, 0.25, 0.25, 0.24);
	glPopMatrix();
	/// HEAD
	glPushMatrix();
	glTranslatef(‐4.0, 0.0, 0.0);
	glScalef(1.0, 0.5, 2.0);
	DrawSphere(2, 10, 0.75, 0.25, 0.24);
	glPopMatrix();
	glPopMatrix();
}
void CGLRenderer::DrawSpider() {
	int i;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texSpider);
	glPushMatrix();
	glTranslatef(0.0, 5.0, 0.0);
	glPushMatrix();
	DrawSpiderBody();
	glPopMatrix();

	for (i = 0; i < 4; i++) {
		glPushMatrix();
		glRotatef(i * 30 ‐ 45, 0.0, 1.0, 0.0);
		glRotatef(45, 1.0, 0.0, 0.0);
		DrawLeg();
		glPopMatrix();
	}
	for (i = 0; i < 4; i++) {
		glPushMatrix();
		glRotatef(i * 30 ‐45 + 180, 0.0, 1.0, 0.0);
		glRotatef(45, 1.0, 0.0, 0.0);
		DrawLeg();
		glPopMatrix();
	}
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

//crtanje svemirski brod
void GLRenderer::DrawTriangle(float d1, float d2, float rep)
{



	glBegin(GL_TRIANGLES);
	{
		glColor3f(1.0, 1.0, 1.0);



		float d3 = sqrt(d2 * d2 + d1 * d1);
		float sine = d2 / d3;
		float cosine = d1 / d3;
		float x = d1 * sine;
		float y = d1 * cosine;
		float normX = x / d3;
		float normY = y / d3;




		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.5 * rep, rep);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f((0.5 + normX) * rep, rep - (normY * rep));
		glVertex3f(d1, 0.0, 0.0);
		glTexCoord2f(0.5 * rep, 0.0);
		glVertex3f(d1, d2, 0.0);
	}
	glEnd();
}
void GLRenderer::DrawShip()
{
	float d1 = 5.8, d2 = 2.15, rep = 3;
	float angle = atan(d2 / d1);
	glBindTexture(GL_TEXTURE_2D, shipTex);



	glPushMatrix();
	glRotatef(-4.75, 0.0, 1.0, 0.0);
	glRotatef(13, 1.0, 0.0, 0.0);
	glRotatef(-angle * (180 / pi), 0.0, 0.0, 1.0);
	DrawTriangle(d1, d2, rep);
	glPopMatrix();



	glPushMatrix();
	glScalef(1.0, 1.0, -1.0);
	glRotatef(-4.75, 0.0, 1.0, 0.0);
	glRotatef(13, 1.0, 0.0, 0.0);
	glRotatef(-angle * (180 / pi), 0.0, 0.0, 1.0);
	DrawTriangle(d1, d2, rep);
	glPopMatrix();



	glPushMatrix();
	glScalef(1.0, -1.0, 1.0);
	glRotatef(-4.75, 0.0, 1.0, 0.0);
	glRotatef(13, 1.0, 0.0, 0.0);
	glRotatef(-angle * (180 / pi), 0.0, 0.0, 1.0);
	DrawTriangle(d1, d2, rep);
	glPopMatrix();



	glPushMatrix();
	glScalef(1.0, -1.0, -1.0);
	glRotatef(-4.75, 0.0, 1.0, 0.0);
	glRotatef(13, 1.0, 0.0, 0.0);
	glRotatef(-angle * (180 / pi), 0.0, 0.0, 1.0);
	DrawTriangle(d1, d2, rep);
	glPopMatrix();




}
void GLRenderer::DrawSpaceCube(double a)
{
	glBindTexture(GL_TEXTURE_2D, spaceTex[4]);
	glBegin(GL_QUADS);
	{
		//top
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, a / 2, a / 2);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, a / 2, -a / 2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();



	glBindTexture(GL_TEXTURE_2D, spaceTex[5]);
	glBegin(GL_QUADS);
	{
		//bottom
		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();



	glBindTexture(GL_TEXTURE_2D, spaceTex[0]);
	glBegin(GL_QUADS);
	{
		//front
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, spaceTex[3]);
	glBegin(GL_QUADS);
	{
		//back
		glTexCoord2f(0.0, 0.0);
		glVertex3f(a / 2, a / 2, a / 2);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-a / 2, a / 2, a / 2);
	}
	glEnd();



	glBindTexture(GL_TEXTURE_2D, spaceTex[1]);
	glBegin(GL_QUADS);
	{
		//left
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, a / 2, a / 2);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-a / 2, a / 2, -a / 2);
	}
	glEnd();



	glBindTexture(GL_TEXTURE_2D, spaceTex[2]);
	glBegin(GL_QUADS);
	{
		//right
		glTexCoord2f(0.0, 0.0);
		glVertex3f(a / 2, a / 2, -a / 2);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();
}

//vetrenjaca

//kad se crta iz jednu sliku( kad je kao krst pa treba spojis)
void GLRenderer::DrawCube(double a)
{
	glBindTexture(GL_TEXTURE_2D, envTex);
	glColor3f(1.0, 1.0, 1.0);
	float quarter = 1.0 / 4.0;
	float third = 1.0 / 3.0;



	glBegin(GL_QUAD_STRIP); //10 tacke za 4 dela kocke
	{
		glTexCoord2f(0.0, third);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(0.0, 2 * third);
		glVertex3f(-a / 2, a / 2, a / 2);
		glTexCoord2f(quarter, third);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(quarter, 2 * third);
		glVertex3f(-a / 2, -a / 2, a / 2);



		glTexCoord2f(2 * quarter, third);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(2 * quarter, 2 * third);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(3 * quarter, third);
		glVertex3f(a / 2, a / 2, -a / 2);
		glTexCoord2f(3 * quarter, 2 * third);
		glVertex3f(a / 2, a / 2, a / 2);



		glTexCoord2f(1.0, third);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(1.0, 2 * third);
		glVertex3f(-a / 2, a / 2, a / 2);
	}
	glEnd();



	glBegin(GL_QUADS); //8 tacke za 2 dela kocke (gore i dole, da je ovde QUAD_STRIP, bile bi 3 strane definisanek
	{
		glTexCoord2f(quarter, 0.0);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(quarter, third);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(2 * quarter, third);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(2 * quarter, 0.0);
		glVertex3f(a / 2, a / 2, -a / 2);



		glTexCoord2f(quarter, 2 * third);
		glVertex3f(-a / 2, -a / 2, a / 2);
		glTexCoord2f(quarter, 1.0);
		glVertex3f(-a / 2, a / 2, a / 2);
		glTexCoord2f(2 * quarter, 1.0);
		glVertex3f(a / 2, a / 2, a / 2);
		glTexCoord2f(2 * quarter, 2 * third);
		glVertex3f(a / 2, -a / 2, a / 2);
	}
	glEnd();
}
void GLRenderer::DrawCone(double r, double h, int n)
{
	glBegin(GL_TRIANGLES);
	{
		float currAngle = 0;
		float angleStep = (2 * pi) / n;
		float L = sqrt(h * h + r * r);
		float ny = r / L;
		float nr = h / L;

		for (int i = 0; i < n; i++)
		{
			float nx = nr * cos(currAngle + angleStep / 2); // zasto + angleStep / 2??
			float nz = nr * sin(currAngle + angleStep / 2); // zato sto se vrh trougla nalazi na sredini stranie, tj. izmedju tacaka koje su definisane dole
			glNormal3f(nx, ny, nz);
			glVertex3f(0, h, 0);

			nx = nr * cos(currAngle); // normale racunamo sa nr; prva tacka bez rastojanja
			nz = nr * sin(currAngle);
			glNormal3f(nx, ny, nz);
			float x = r * cos(currAngle); //a koordinate sa obicno r 
			float z = r * sin(currAngle);
			glVertex3f(x, 0, z);

			nx = nr * cos(currAngle + angleStep); //druga tacka sa plus rastojanje, tj. ugao
			nz = nr * sin(currAngle + angleStep);
			glNormal3f(nx, ny, nz);
			x = r * cos(currAngle + angleStep);
			z = r * sin(currAngle + angleStep);
			glVertex3f(x, 0, z);

			currAngle += angleStep;
		}
	}
	glEnd();
}
void GLRenderer::DrawPaddle(double length, double width)
{
	glNormal3f(0.0, 0.0, 1.0);
	float xMove = width / 16;
	glBegin(GL_QUADS);
	{
		glVertex3f(0.0 - xMove, 0.0, 0.0);
		glVertex3f(width / 8 - xMove, 0.0, 0.0);
		glVertex3f(width / 8 - xMove, length / 8, 0.0);
		glVertex3f(0.0 - xMove, length / 8, 0.0);

		glVertex3f(0.0 - xMove, length / 8, 0.0);
		glVertex3f(width / 8 - xMove, length / 8, 0.0);
		glVertex3f(width - xMove, 2 * (length / 8), 0.0);
		glVertex3f(0.0 - xMove, 2 * (length / 8), 0.0);

		glVertex3f(0.0 - xMove, 2 * (length / 8), 0.0);
		glVertex3f(width - xMove, 2 * (length / 8), 0.0);
		glVertex3f(width - xMove, length, 0.0);
		glVertex3f(0.0 - xMove, length, 0.0);
	}
	glEnd();
}














