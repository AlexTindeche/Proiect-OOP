﻿// AddElemDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"

#include "Arpegio.h"
#include "AddElemDlg.h"

// AddElemDlg dialog

IMPLEMENT_DYNAMIC(AddElemDlg, CDialog)

AddElemDlg::AddElemDlg(bool add, CWnd* pParent)
	: CDialog(IDD_AddElemDlg, pParent),
	m_isAddDlg{ add },
	m_TipElem{ 0 },
	m_DurElem{ 0 },
	m_InaltimeNota{ 0 },
	m_OctavaNota{ OCTAVA_DEF }
{

}

AddElemDlg::~AddElemDlg()
{
}

void AddElemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// get controls
	DDX_Control(pDX, IDC_TIPELEMCOMBO, m_TipElemComboBox);
	DDX_Control(pDX, IDC_DURATA_ELEM_COMBO, m_DurElemComboBox);
	DDX_Control(pDX, IDC_INALTIMECOMBO, m_InaltimeNotaComboBox);
	DDX_Control(pDX, IDC_OCTAVASLIDER, m_OctavaSliderCtrl);

	// get values
	DDX_CBIndex(pDX, IDC_TIPELEMCOMBO, m_TipElem);
	DDX_CBIndex(pDX, IDC_DURATA_ELEM_COMBO, m_DurElem);
	DDX_CBIndex(pDX, IDC_INALTIMECOMBO, m_InaltimeNota);
	DDX_Slider(pDX, IDC_OCTAVASLIDER, m_OctavaNota);
}


// Data operations

// set initial data
void AddElemDlg::SetElement(Element e)
{
	m_TipElem = (int)e.get_tip_element();
	m_DurElem = Durate::GetPos(e.get_durata());
}

void AddElemDlg::SetElement(Nota n)
{
	SetElement((Element)n);
	m_InaltimeNota = (int)n.get_inaltime();
	m_OctavaNota = n.get_octava();
}

void AddElemDlg::SetElement(Pauza p)
{
	SetElement((Element)p);
}

// get dialog data
TipElement AddElemDlg::GetTipElement()
{
	return (TipElement) m_TipElem;
}

Nota AddElemDlg::GetNota()
{
	return Nota(Durate::GetDurata(m_DurElem), (Inaltime)m_InaltimeNota, m_OctavaNota);
}

Pauza AddElemDlg::GetPauza()
{
	return Pauza(Durate::GetDurata(m_DurElem));
}


// Message map


BEGIN_MESSAGE_MAP(AddElemDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_TIPELEMCOMBO, &AddElemDlg::OnCbnSelchangeTipelemcombo)
END_MESSAGE_MAP()


// AddNotaDlg message handlers


BOOL AddElemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// initializare controale dialog
	SetupTipCtrl();
	SetupDurataCtrl();
	SetupInaltimeCtrl();
	SetupOctavaCtrl();

	// titlu dialog in functie de operatiune: adaugare / modificare
	SetWindowTextW(m_isAddDlg ? L"Adaugă element" : L"Modifică element");

	// permite selectie durata doar pe adaugare
	m_DurElemComboBox.EnableWindow(m_isAddDlg);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void AddElemDlg::OnCbnSelchangeTipelemcombo()
{
	// selectie curenta
	int curSel = m_TipElemComboBox.GetCurSel();

	// resetare valori controale disabled
	if (m_TipElem != curSel && curSel == (int)TipElement::PAUZA)
	{
		m_InaltimeNotaComboBox.SetCurSel(0);
		m_OctavaSliderCtrl.SetPos(OCTAVA_DEF);
		UpdateData();
	}

	// disable la controale atunci cand tipul este pauza
	m_InaltimeNotaComboBox.EnableWindow(curSel == (int)TipElement::NOTA);
	m_OctavaSliderCtrl.EnableWindow(curSel == (int)TipElement::NOTA);
}


// Helper functions

void AddElemDlg::SetupTipCtrl()
{
	// inserare optiuni
	m_TipElemComboBox.AddString(L"Notă");
	m_TipElemComboBox.AddString(L"Pauză");

	// prima optiune selectata implicit
	m_TipElemComboBox.SetCurSel(m_TipElem);

	// enable / disable controale in functie de tipul elementului
	OnCbnSelchangeTipelemcombo();
}

void AddElemDlg::SetupDurataCtrl()
{
	// nume durate luate din vector
	for (wstring d : Durate::nume_durate)
	{
		m_DurElemComboBox.AddString(d.c_str());
	}

	// prima optiune selectata implicit
	m_DurElemComboBox.SetCurSel(m_DurElem);
}

void AddElemDlg::SetupInaltimeCtrl()
{
	// nume inaltimi din vector
	for (wstring i : nume_inaltimi)
	{
		m_InaltimeNotaComboBox.AddString(i.c_str());
	}

	// prima optiune selectata implicit
	m_InaltimeNotaComboBox.SetCurSel(m_InaltimeNota);
}

void AddElemDlg::SetupOctavaCtrl()
{
	// setarea rangului de selectie octava
	m_OctavaSliderCtrl.SetRangeMin(OCTAVA_MIN, TRUE);
	m_OctavaSliderCtrl.SetRangeMax(OCTAVA_MAX, TRUE);
	m_OctavaSliderCtrl.SetPos(m_OctavaNota);
}
