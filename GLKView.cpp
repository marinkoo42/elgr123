
// GLKView.cpp : implementation of the CGLKView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GLK.h"
#endif

#include "GLKDoc.h"
#include "GLKView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLKView

IMPLEMENT_DYNCREATE(CGLKView, CView)

BEGIN_MESSAGE_MAP(CGLKView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGLKView construction/destruction

CGLKView::CGLKView()
{
	// TODO: add construction code here

}

CGLKView::~CGLKView()
{
}

BOOL CGLKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGLKView drawing

void CGLKView::OnDraw(CDC* pDC)
{
	CGLKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CGLKView printing

BOOL CGLKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGLKView diagnostics

#ifdef _DEBUG
void CGLKView::AssertValid() const
{
	CView::AssertValid();
}

void CGLKView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLKDoc* CGLKView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLKDoc)));
	return (CGLKDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLKView message handlers


int CGLKView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CGLKView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CGLKView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);	
}


BOOL CGLKView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CGLKView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CGLKView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_SPACE)
		m_glRenderer.angle += 0.1;
	if (nChar == '1')
		m_glRenderer.x += 0.1;
	if (nChar == '2')
		m_glRenderer.x -= 0.1;
	if (nChar == '3')
		m_glRenderer.y += 0.1;
	if (nChar == '4')
		m_glRenderer.y -= 0.1;
	if (nChar == '5')
		m_glRenderer.z+= 0.1;
	if (nChar == '6')
		m_glRenderer.z -= 0.1;
	if (nChar == '6')
		m_glRenderer.z -= 0.1;
	if (nChar == VK_UP)
		m_glRenderer.alfa += 1;
	if (nChar == VK_DOWN)
		m_glRenderer.alfa -= 1;
	if (nChar == VK_LEFT)
		m_glRenderer.beta += 1;
	if (nChar == VK_RIGHT)
		m_glRenderer.beta -= 1;
	if (nChar == VK_SUBTRACT)
		m_glRenderer.dist -= 1;
	if (nChar == VK_ADD)
		m_glRenderer.dist += 1;
	
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//dodato
void COpenGLPripremaIspitView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT)
	{
		m_glRenderer.RotateView(0, -1);
	}
	else if (nChar == VK_LEFT)
	{
		m_glRenderer.RotateView(0, 1);
	}
	else if (nChar == VK_UP)
	{
		m_glRenderer.RotateView(1, 0);
	}
	else if (nChar == VK_DOWN)
	{
		m_glRenderer.RotateView(-1, 0);
	}
	else if (nChar == VK_ADD)
	{
		m_glRenderer.Zoom(0.5);
	}
	else if (nChar == VK_SUBTRACT)
	{
		m_glRenderer.Zoom(-0.5);
	}
	else if (nChar == 'Q' || nChar == 'q')
	{
		m_glRenderer.RotateMoon(1);
	}
	else if (nChar == 'W' || nChar == 'w')
	{
		m_glRenderer.RotateMoon(-1);
	}
	else if (nChar == 'S' || nChar == 's')
	{
		m_glRenderer.ToggleLight();
	}
	Invalidate();



	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}