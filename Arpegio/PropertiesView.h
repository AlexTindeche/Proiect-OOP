
// PropertiesView.h : interface of the CPropertiesView class
//

#pragma once

#include "ArpegioDoc.h"

// CPropertiesView form view

class CPropertiesView : public CFormView
{
	DECLARE_DYNCREATE(CPropertiesView)

protected:
	CPropertiesView();           // protected constructor used by dynamic creation
	virtual ~CPropertiesView();

// Attributes
public:
	CArpegioDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnInitialUpdate();

// Implementation
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPropertiesView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

	afx_msg void OnEnChangeEdittitlu();
	afx_msg void OnCbnSelchangeCheiecombo();
	afx_msg void OnCbnSelchangeMasuraCombo();
	afx_msg void OnLbnSelchangeNotelist();
	void PopulateNoteList();

private:
	CEdit m_titluEditCtrl;
	CComboBox m_cheieComboBox;
	CEdit m_MasIntrgCEdit;
	CComboBox m_masuraComboBox;
	CListBox m_NoteListCtrl;

	CString m_titlu;
	int m_Cheie;
	int m_Masura;
	int m_MasIntregi;
	int m_NotaSel;
};

#ifndef _DEBUG  // debug version in ArpegioView.cpp
inline CArpegioDoc* CPropertiesView::GetDocument() const
{
	return reinterpret_cast<CArpegioDoc*>(m_pDocument);
}
#endif

