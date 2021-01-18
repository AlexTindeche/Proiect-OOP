
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDocumentView

IMPLEMENT_DYNCREATE(CDocumentView, CView)

BEGIN_MESSAGE_MAP(CDocumentView, CView)
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

void CDocumentView::OnDraw(CDC* pDC)
{
	CArpegioDoc* pDoc = GetDocument();
	pDC->DrawText(pDoc->mesaj, CRect(0, 0, 1000, 1000), 0);
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
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
	if (nChar == VK_ESCAPE)
	{
		pDoc->mesaj.Append(L"HELLOOOO!!!!");
	}

	else if (nChar == VK_BACK)
	{
		pDoc->mesaj = pDoc->mesaj.Right(pDoc->mesaj.GetLength() - 1);
	}
	else
	{
		pDoc->mesaj.AppendChar((char)nChar);
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
