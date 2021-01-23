#pragma once


#include "portativ.h"

// AddElemDlg dialog

class AddElemDlg : public CDialog
{
	DECLARE_DYNAMIC(AddElemDlg)

public:
	AddElemDlg(bool add = true, CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddElemDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddElemDlg };
#endif

// Data exchange
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Data operations
public:
	void SetElement(Element e);
	void SetElement(Nota n);
	void SetElement(Pauza n);

	TipElement GetTipElement();
	Nota GetNota();
	Pauza GetPauza();

// Tip dialog
private:
	bool m_isAddDlg;

// Message handlers
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

// Helper functionality
private:
	void SetupTipCtrl();
	void SetupDurataCtrl();
	void SetupInaltimeCtrl();
	void SetupOctavaCtrl();

// Dialog controls
private:
	CComboBox m_TipElemComboBox;
	CComboBox m_DurElemComboBox;
	CComboBox m_InaltimeNotaComboBox;
	CSliderCtrl m_OctavaSliderCtrl;

// Dialog data
private:
	int m_TipElem;
	int m_DurElem;
	int m_InaltimeNota;
	int m_OctavaNota;

public:
	afx_msg void OnCbnSelchangeTipelemcombo();
};
