
// GDI-2018View.cpp : implementation of the CGDI2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-2018.h"
#endif

#include "GDI-2018Doc.h"
#include "GDI-2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTRL+M+O collapse all funcs


IMPLEMENT_DYNCREATE(CGDI2018View, CView)

BEGIN_MESSAGE_MAP(CGDI2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGDI2018View construction/destruction

CGDI2018View::CGDI2018View() noexcept
{
	// TODO: add construction code here
	arm1 = new DImage();
	arm2 = new DImage();
	bager = new DImage();
	pozadina = new DImage();

	// mat = new DImage * [3];
	// for (int i = 0; i < 3; i++)
	// 	mat[i] = new DImage[3];

	viljuska = GetEnhMetaFile(CString("viljuska.emf"));
	GetEnhMetaFileHeader(viljuska, sizeof(ENHMETAHEADER), &header_viljuska);

	arm1->Load(CString("res/arm1.png"));
	arm2->Load(CString("res/arm2.png"));
	bager->Load(CString("res/bager.png"));
	pozadina->Load(CString("res/pozadina.png"));
}

CGDI2018View::~CGDI2018View()
{
	DeleteEnhMetaFile(viljuska);
	delete arm1;
	delete arm2;
	delete bager;
	delete pozadina;

	// for (int i = 0; i < 3; i++)
	// 	delete[] mat[i];

	// delete[] mat;
}


// CGDI2018View drawing

void CGDI2018View::Translate(CDC* pDC, float dx, float dy, bool rightmul)
{
	XFORM forma = {
		1,
		0,
		0,
		1,
		dx,
		dy
	};

	pDC->ModifyWorldTransform(&forma, rightmul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2018View::Rotate(CDC* pDC, float angle, bool rightmul)
{
	angle *= 3.14 / 180.;
	XFORM forma = {
	cos(angle),
	sin(angle),
	-sin(angle),
	cos(angle),
	0,
	0
	};

	pDC->ModifyWorldTransform(&forma, rightmul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2018View::Scale(CDC* pDC, float sx, float sy, bool rightmul)
{
	XFORM forma = {
	sx,
	0,
	0,
	sy,
	0,
	0
	};

	pDC->ModifyWorldTransform(&forma, rightmul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2018View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM form =
	{
		mx ? -1 : 1,
		0,
		0,
		my ? -1 : 1,
		0,
		0
	};
	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CGDI2018View::DrawGrid(CDC* pDC)
{		
	CPen* pen = new CPen(PS_SOLID, 1, RGB(1, 1, 1));
	CPen* oldPen = pDC->SelectObject(pen);

	for (int i = 25; i <= 500; i += 25)
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);
		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);
	}

	pDC->SelectObject(oldPen);
	pen->DeleteObject();
}

void CGDI2018View::ChangeDC(CDC* memDC, CDC* pDC)
{
	CRect client;
	GetClientRect(&client);

	CBitmap mask;
	mask.CreateBitmap(client.Width(), client.Height(), 1, 1, NULL);

	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(memDC);
	dstDC->SelectObject(mask);

	memDC->SetBkColor(RGB(0, 0, 0));
	memDC->SetTextColor(RGB(255, 255, 255));

	dstDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCPAINT);

	pDC->BitBlt(0, 0, client.Width(), client.Height(), dstDC, 0, 0, SRCAND);

	delete dstDC;

	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCPAINT);
}


//Ako ne postoji DrawTransparent u DImage ili za bitmape
void CGDI2018View::DrawImgTransparent(CDC* pDC, DImage* img)
{
	
		CBitmap mask;
		mask.CreateBitmap(img->Width(), img->Height(), 1, 1, NULL);

		CBitmap image;
		image.CreateBitmap(img->Width(), img->Height(), 4, 8, NULL);
		image.SetBitmapBits(img->Width() * img->Height() * img->BPP(), img->GetDIBBits());

		CDC* srcDC = new CDC();
		CDC* dstDC = new CDC();
		srcDC->CreateCompatibleDC(pDC);
		dstDC->CreateCompatibleDC(pDC);

		CBitmap* srcOldBitmap = srcDC->SelectObject(&image);
		CBitmap* dstOldBitmap = dstDC->SelectObject(&mask);


		COLORREF removeColor = srcDC->GetPixel(0, 0);
		COLORREF srcOldBckColor = srcDC->SetBkColor(removeColor);
		dstDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCCOPY);

		COLORREF srcOldTextColor = srcDC->SetTextColor(RGB(255, 255, 255));
		srcDC->SetBkColor(RGB(0, 0, 0));
		srcDC->BitBlt(0, 0, img->Width(), img->Height(), dstDC, 0, 0, SRCAND);

		dstDC->SelectObject(dstOldBitmap);
		delete dstDC;

		srcDC->SelectObject(&mask);
		pDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCAND);

		srcDC->SelectObject(&image);
		pDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCPAINT);
		srcDC->SelectObject(srcOldBitmap);
		delete srcDC;
	
}

void CGDI2018View::DrawBackground(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);
	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), client);
}

//bilijar
void CGDI2016View::DrawStick(CDC* pDC, int w)
{
	CBrush* oldbrush;
	CBrush* braonbrush = new CBrush(RGB(255, 190, 128));
	CBrush* tamnibrush = new CBrush(RGB(64, 32, 0));

	CPen* pen, * oldpen;
	pen = new CPen(PS_SOLID, 1, RGB(64, 32, 0));
	oldpen = pDC->SelectObject(pen);

	float sirina = w / 100.0;
	oldbrush = pDC->SelectObject(tamnibrush);
	CPoint* niz = new CPoint[4];



	niz[0] = CPoint(0, -sirina);
	niz[1] = CPoint(0, sirina);
	niz[2] = CPoint(0.33 * w, sirina * 0.66);
	niz[3] = CPoint(0.33 * w, -sirina * 0.66);

	pDC->Polygon(niz, 4);

	pDC->Ellipse(-sirina, -sirina, sirina, sirina);

	niz[0] = CPoint(0.33 * w, -sirina * 0.66);
	niz[1] = CPoint(0.33 * w, sirina * 0.66);
	niz[2] = CPoint(w, sirina * 0.5);
	niz[3] = CPoint(w, -sirina * 0.5);

	pDC->SelectObject(braonbrush);
	pDC->Polygon(niz, 4);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);

	delete braonbrush;
	delete tamnibrush;
	delete pen;
	delete[] niz;

	pen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	pDC->SelectObject(pen);

	pDC->MoveTo(2, -sirina + 4);
	pDC->LineTo(w - 2, -sirina * 0.5 + 3);

	pDC->SelectObject(oldpen);

}

void CGDI2016View::DrawBall(CDC* pDC, int w)
{
	CBrush* oldbrush, * redbrush;
	redbrush = new CBrush(RGB(255, 64, 64));
	CPen* oldpen, * pen;
	pen = new CPen(PS_SOLID, 1, RGB(128, 0, 0));

	oldpen = pDC->SelectObject(pen);
	oldbrush = pDC->SelectObject(redbrush);

	pDC->Ellipse(-w / 2, -w / 2, w / 2, w / 2);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	delete pen;
	delete redbrush;
	pen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	redbrush = new CBrush(RGB(255, 255, 255));

	pDC->SelectObject(pen);
	pDC->SelectObject(redbrush);

	pDC->Ellipse(-w / 6. + w / 6., -w / 6. - w / 6., w / 6. + w / 6., w / 6. - w / 6.);


	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	delete pen;
	delete redbrush;
}

void CGDI2016View::DrawBounds(CDC* pDC, CRect rect, int w)
{
	CRgn r1, r2;
	r1.CreateRectRgn(rect.left + w, rect.top + w, rect.right - w, rect.bottom - w);
	r2.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);

	r1.CombineRgn(&r1, &r2, RGN_XOR);
	pDC->SelectClipRgn(&r1);

	wood->Draw(pDC, CRect(0, 0, wood->Width(), wood->Height()), rect);

	pDC->SelectClipRgn(&r2);


}

void CGDI2016View::DrawHoles(CDC* pDC, CRect rect, int size)
{
	CBrush* oldbrush, * brush;
	brush = new CBrush(RGB(0, 0, 0));
	oldbrush = pDC->SelectObject(brush);

	pDC->Ellipse(rect.left + size, rect.top + size, rect.left + 2 * size, rect.top + 2 * size);
	pDC->Ellipse(rect.Width() / 2 - size / 2, rect.top + size, rect.Width() / 2 + size / 2, rect.top + 2 * size);
	pDC->Ellipse(rect.right - 2 * size, rect.top + size, rect.right - size, rect.top + 2 * size);

	pDC->Ellipse(rect.left + size, rect.bottom - size, rect.left + 2 * size, rect.bottom - 2 * size);
	pDC->Ellipse(rect.Width() / 2 - size / 2, rect.bottom - size, rect.Width() / 2 + size / 2, rect.bottom - 2 * size);
	pDC->Ellipse(rect.right - 2 * size, rect.bottom - size, rect.right - size, rect.bottom - 2 * size);

	pDC->SelectObject(oldbrush);
	delete brush;
}
//end_bilijar

//font
void CIND17739View::DrawName(CDC* pDC)
{

	XFORM newForm, oldForm;



	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 475, 475);
	Rotate(pDC, -180);
	pDC->SetWorldTransform(&newForm);

	CFont* newFont = new CFont();
	int oldMode = pDC->GetBkMode();
	pDC->SetBkMode(TRANSPARENT);

	newFont->CreateFontW(1.2 * 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	CFont* OldFont = pDC->SelectObject(newFont);
	pDC->TextOutW(0, 0, CString("17746 Ivan Markovic"));
	COLORREF oldColor = pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOutW(2, 2, CString("17746 Ivan Markovic"));

	pDC->SetWorldTransform(&oldForm);
	pDC->SetBkMode(oldMode);
	pDC->SelectObject(OldFont);
	newFont->DeleteObject();



}

//end_font

//koristi DrawTransparent iz DImage
void CGDI2018View::DrawBody(CDC* pDC)
{
	CRect bagerRect = CRect(0, 0, bager->Width(), bager->Height());
	bager->DrawTransparent(pDC, bagerRect, bagerRect,NULL);
	Translate(pDC, 0, 220, false);
	Rotate(pDC, -90, false);
}

void CGDI2018View::DrawArm1(CDC* pDC)
{


	Translate(pDC, 58, 61, false);
	Rotate(pDC, ugaoArm1, false);
	Translate(pDC, -58, -61, false);

	CRect arm1Rect = CRect(0, 0, arm1->Width(), arm1->Height());
	arm1->DrawTransparent(pDC, arm1Rect, arm1Rect, NULL);

	Translate(pDC, 309-39, 61+37, false);
	Rotate(pDC, -90, false);

}

void CGDI2018View::DrawArm2(CDC* pDC)
{

	Translate(pDC, 36, 40, false);
	Rotate(pDC, ugaoArm2, false);
	Translate(pDC, -36, -40, false);

	CRect arm2Rect = CRect(0, 0, arm2->Width(), arm2->Height());
	arm2->DrawTransparent(pDC, arm2Rect, arm2Rect, NULL);
	
	Translate(pDC, 272, 40, false);
	Rotate(pDC, -90, false);
}

void CGDI2018View::DrawFork(CDC* pDC)
{
	
	Translate(pDC, 14, -20, false);

	Translate(pDC, 14, 20, false);
	Rotate(pDC, ugaoFork, false);
	Translate(pDC, -14, -20, false);


	CRect viljuskaRect = CRect( header_viljuska.rclBounds.left,
								header_viljuska.rclBounds.top,
								header_viljuska.rclBounds.right,
								header_viljuska.rclBounds.bottom
								);
	Scale(pDC, 2.5, 2.5, false);
	pDC->PlayMetaFile(viljuska, viljuskaRect);
	Scale(pDC, -2.5, -2.5, false);

}

void CGDI2018View::DrawExcavator(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);

	Translate(pDC, client.Width() - 400 + put, client.Height() - 300, false);
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}

void CGDI2018View::OnDraw(CDC* pDC)
{
	CGDI2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CRect client;
	GetClientRect(&client);
	CDC* mem = new CDC();

	mem->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	mem->SelectObject(&bmp);

	mem->SetGraphicsMode(GM_ADVANCED);
	
	XFORM stara;
	mem->GetWorldTransform(&stara);

	DrawBackground(mem);

	DrawExcavator(mem);

	mem->SetWorldTransform(&stara);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), mem, 0, 0, SRCCOPY);

	delete mem;

}

void CIND17739View::PaintGrey(DImage* img)
{
	unsigned char* pixelArray = img->GetDIBBits();
	DWORD sizeOfPicture = img->Width() * img->Height() * img->BPP();

	int firstPixel[3];
	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img->BPP())
	{
		int pixel = 0;
		bool bg = true;

		for (int i = 0; i < 3; i++)
		{
			if (ptr == 0)
				firstPixel[i] = pixelArray[i];
			else
				bg = (firstPixel[i] == pixelArray[ptr + i]) && bg;
			pixel += pixelArray[ptr + i];
		}
		pixel /= 3;
		pixel += 64;

		if (pixel > 255)
			pixel = 255;

		for (int i = 0; i < 3 && !bg; i++)
			pixelArray[ptr + i] = pixel;
	}
	img->Update();
}

void CIND17739View::Paint(DImage* img, int color)
{
	if (color < 0 && color > 3)
		return;

	unsigned char* pixelArray = img->GetDIBBits();
	DWORD sizeOfPicture = img->Width() * img->Height() * img->BPP();

	int firstPixel[3];
	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img->BPP())
	{
		bool bg = true;
		for (int i = 0; i < 3; i++)
		{
			if (ptr == 0)
				firstPixel[i] = pixelArray[i];
			else
				bg = (firstPixel[i] == pixelArray[ptr + i]) && bg;

		}

		for (int i = 0; i < 3 && !bg; i++)
			if (i != color)
				pixelArray[ptr + i] = 0;
	}
	img->Update();
}



BOOL CGDI2018View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CGDI2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar) {
	case '1':
		ugaoArm1 -= 10;
		break;
	case '2':
		ugaoArm1 += 10;
		break;
	case '3':
		ugaoArm2 -= 10;
		break;
	case '4':
		ugaoArm2 += 10;
		break;
	case '5':
		ugaoFork -= 10;
		break;
	case '6':
		ugaoFork += 10;
		break;
	case VK_LEFT:
		put -= 10;
		break;
	case VK_RIGHT:
		put += 10;
		break;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
