
// ArpegioView.h : interface of the CDocumentView class
//

#pragma once


class CDocumentView : public CView
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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CDocumentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in ArpegioView.cpp
inline CArpegioDoc* CDocumentView::GetDocument() const
   { return reinterpret_cast<CArpegioDoc*>(m_pDocument); }
#endif

