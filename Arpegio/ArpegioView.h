
// ArpegioView.h : interface of the CDocumentView class
//

#pragma once

#include <functional>
using namespace std;


// helper structure
struct DrawSizes
{
	CRect rect;
	int contentLeft;
	int contentRight;
	int contentTop;
	int contentWidth;
	double elemWidth;

	int rowStart(int r);
	int colStart(int c);
};

// CDocumentView document view

class CDocumentView : public CScrollView
{
protected: // create from serialization only
	CDocumentView() noexcept;
	DECLARE_DYNCREATE(CDocumentView)

// Attributes
public:
	CArpegioDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate(); // first time after construct
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
protected:

// Implementation
public:
	virtual ~CDocumentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Draw helpers
	void DrawPortative(CDC* pDC, int nr_masuri, Cheie c, Durata m, DrawSizes draw);
	void DrawElemente(CDC* pDC, CArpegioDoc* pDoc);
	void DrawTitlu(CDC* pDC, CString titlu, CRect client);
	void DrawCheie(CDC* pDC, Cheie c, int startTop, int startLeft);
	void DrawMasura(CDC* pDC, Durata m, int startTop, int startLeft);
	void DrawLayoutPortativ(CDC* pDC, int startTop, int startLeft, DrawSizes draw);
	void DrawNota(CDC* pDC, Nota n, Cheie c, bool selected, int row, int pos);
	void DrawPauza(CDC* pDC, Pauza p, Cheie c, bool selected, int row, int pos);

	void InsertBitmap(CDC* pDC, UINT idResource, CSize srcSize, CSize destSize, int pos_x, int pos_y, COLORREF color = 0);
	int DocumentClick(CPoint point);

	// Data helpers
	void IterateElements(function<void(Element*, int, int, int, int, int)> cb);

	// Getters
	int GetDocumentHeight();
	CSize GetDocumentSize();
	CRect GetClientViewRect();
	DrawSizes GetDrawSizes();
	void GetNotaData(Nota n, bool& fill, bool& line, bool& flag, bool& secondflag, Inaltime& i, int& octava);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in ArpegioView.cpp
inline CArpegioDoc* CDocumentView::GetDocument() const
   { return reinterpret_cast<CArpegioDoc*>(m_pDocument); }
#endif
