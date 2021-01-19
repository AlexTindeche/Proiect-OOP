#pragma once

#include "ArpegioDoc.h"

// CPropertiesView form view

class CPropertiesView : public CFormView
{
	DECLARE_DYNCREATE(CPropertiesView)

protected:
	CPropertiesView();           // protected constructor used by dynamic creation
	virtual ~CPropertiesView();

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_titluEditCtrl;
	CComboBox m_cheieComboBox;
	CEdit m_MasIntrgCEdit;
	CComboBox m_masuraComboBox;
	CListBox m_NoteListCtrl;

	int m_Cheie;
	int m_Masura;
	int m_MasIntregi;
public:
	CString m_titlu;
	afx_msg void OnEnChangeEdittitlu();
	afx_msg void OnCbnSelchangeCheiecombo();
	afx_msg void OnCbnSelchangeMasuraCombo();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnInitialUpdate();

	void PopulateNoteList();
};


