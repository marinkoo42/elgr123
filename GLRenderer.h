#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	double x = 0, y = 0, z = 0;
	double alfa = 0, beta = 0;
	double angle = 0;
	double dist = 1;
	UINT env;

	/*UINT T[6];
	UINT S[6];
	UINT M[6];
	int xRotationAngle;
	int yRotationAngle;
	float zoom;
	int moonYRotationAngle;
	bool lightOn;*/

	UINT LoadTexture(char* fileName);
	//
	void DrawEnv(double h, double a); //kocka
	void DrawAxis();
	void DrawTube(double r1, double r2, double h, int n);
	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
