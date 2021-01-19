
// ArpegioView.cpp : implementation of the CDocumentView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Arpegio.h"
#endif

#include "ArpegioDoc.h"
#include "ArpegioView.h"

#include <array>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDocumentView

IMPLEMENT_DYNCREATE(CDocumentView, CScrollView)

BEGIN_MESSAGE_MAP(CDocumentView, CScrollView)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CDocumentView construction/destruction

CDocumentView::CDocumentView() noexcept
{
	// TODO: add construction code here

}

CDocumentView::~CDocumentView()
{
}

BOOL CDocumentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDocumentView drawing

int SPACING_PORTATIV = 70;
int SPACING_LINII = 10;
int OFFSET_DOCUMENT = 25;
int TITLE_HEIGHT = 80;

void CDocumentView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CArpegioDoc* pDoc = GetDocument();
	CSize sizeTotal;
	sizeTotal.cy = (pDoc->p.get_nr_elemente() * 70 + 40);
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDocumentView::OnDraw(CDC* pDC)
{
	CArpegioDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);
	
	// titlu
	CFont font;
	font.CreateFont(
		30,                       // nHeight
		0,                        // nWidth
		0,                        // nEscapement
		0,                        // nOrientation
		FW_BOLD,                // nWeight
		FALSE,                    // bItalic
		FALSE,                    // bUnderline
		0,                        // cStrikeOut
		ANSI_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,       // nOutPrecision
		CLIP_DEFAULT_PRECIS,      // nClipPrecision
		DEFAULT_QUALITY,          // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("Arial"));            // lpszFacename

	CFont* def_font = pDC->SelectObject(&font);
	pDC->DrawText(CString(pDoc->p.get_titlu().c_str()), CRect(rect.left, rect.top + OFFSET_DOCUMENT, rect.right, rect.top + OFFSET_DOCUMENT + TITLE_HEIGHT), DT_CENTER);

	// setup portative
	for (int r = 0; r < pDoc->p.get_nr_elemente(); r++)
	{
		for (int l = 0; l < 5; l++)
		{
			int inaltime = r * SPACING_PORTATIV + l * SPACING_LINII + TITLE_HEIGHT;
			CPoint pLine[] = { CPoint(rect.left + OFFSET_DOCUMENT, inaltime), CPoint(rect.right - OFFSET_DOCUMENT, inaltime) };
			pDC->Polyline(pLine, 2);
		}
	}

	CPen penDouble;
	CPen* pOldPen;
	CBrush brushFill;
	CBrush* oldBrush;

	brushFill.CreateSolidBrush(RGB(0, 0, 0));
	penDouble.CreatePen(PS_SOLID | PS_COSMETIC, 2, RGB(0, 0, 0));
	pOldPen = pDC->SelectObject(&penDouble);
	oldBrush = pDC->SelectObject(&brushFill);

	pDC->Ellipse(CRect(50, 4 * SPACING_LINII + TITLE_HEIGHT, 60, 5 * SPACING_LINII + TITLE_HEIGHT));
	CPoint pLine[] = { CPoint(59, 5 * SPACING_LINII + TITLE_HEIGHT - 5), CPoint(59, 1 * SPACING_LINII + TITLE_HEIGHT) };

	/*CPoint bzr[] = { CPoint(CPoint(59, 1 * SPACING_LINII + TITLE_HEIGHT)), CPoint(70, 2 * SPACING_LINII + TITLE_HEIGHT), CPoint(65, 3 * SPACING_LINII + TITLE_HEIGHT), CPoint(65, 1 * SPACING_LINII + TITLE_HEIGHT) };*/
	CPoint bzr[] = {
		CPoint(59, 1 * SPACING_LINII + TITLE_HEIGHT),
		CPoint(65, 1 * SPACING_LINII + TITLE_HEIGHT + 10),
		CPoint(70, 2 * SPACING_LINII + TITLE_HEIGHT),
		CPoint(66, 3 * SPACING_LINII + TITLE_HEIGHT + 3)
	};

	CPoint bzr2[] = {
		CPoint(59, 1 * SPACING_LINII + TITLE_HEIGHT + 10),
		CPoint(65, 1 * SPACING_LINII + TITLE_HEIGHT + 10 + 10),
		CPoint(70, 2 * SPACING_LINII + TITLE_HEIGHT + 10),
		CPoint(66, 3 * SPACING_LINII + TITLE_HEIGHT + 2 + 10)
	};
	pDC->Polyline(pLine, 2);
	pDC->PolyBezier(bzr, 4);
	pDC->PolyBezier(bzr2, 4);
}


// CDocumentView diagnostics

#ifdef _DEBUG
void CDocumentView::AssertValid() const
{
	CView::AssertValid();
}

void CDocumentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CArpegioDoc* CDocumentView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CArpegioDoc)));
	return (CArpegioDoc*)m_pDocument;
}
#endif //_DEBUG


// CDocumentView message handlers


void CDocumentView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CArpegioDoc* pDoc = GetDocument();
	//if (nChar == VK_ESCAPE)
	//{
	//	pDoc->mesaj.Append(L"HELLOOOO!!!!");
	//}

	//else if (nChar == VK_BACK)
	//{
	//	pDoc->mesaj = pDoc->mesaj.Right(pDoc->mesaj.GetLength() - 1);
	//}
	//else
	//{
	//	pDoc->mesaj.AppendChar((char)nChar);
	//}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDocumentView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CScrollView::OnUpdate(pSender, lHint, pHint);
	
	CArpegioDoc* pDoc = GetDocument();
	CSize sizeTotal;
	sizeTotal.cy = (pDoc->p.get_nr_elemente() * SPACING_PORTATIV + TITLE_HEIGHT);
	SetScrollSizes(MM_TEXT, sizeTotal);
}
