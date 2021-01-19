// CPropertiesView.cpp : implementation file
//

#include "pch.h"
#include "Arpegio.h"
#include "PropertiesView.h"

#include "durate.h"


// CPropertiesView

IMPLEMENT_DYNCREATE(CPropertiesView, CFormView)

CPropertiesView::CPropertiesView()
	: CFormView(IDD_CPropertiesView)
	, m_titlu(_T(""))
	, m_Cheie(0)
	, m_Masura(0)
	, m_MasIntregi(0)
{

}

CPropertiesView::~CPropertiesView()
{
}

void CPropertiesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDITTITLU, m_titluEditCtrl);
	DDX_Control(pDX, IDC_CHEIECOMBO, m_cheieComboBox);
	DDX_Control(pDX, IDC_MAS_INTRG, m_MasIntrgCEdit);
	DDX_Control(pDX, IDC_MASURA_COMBO, m_masuraComboBox);
	DDX_Control(pDX, IDC_NOTELIST, m_NoteListCtrl);

	DDX_Text(pDX, IDC_EDITTITLU, m_titlu);
	DDX_CBIndex(pDX, IDC_CHEIECOMBO, m_Cheie);
	DDX_Text(pDX, IDC_MAS_INTRG, m_MasIntregi);
	DDX_CBIndex(pDX, IDC_MASURA_COMBO, m_Masura);
}

BEGIN_MESSAGE_MAP(CPropertiesView, CFormView)
	ON_EN_CHANGE(IDC_EDITTITLU, &CPropertiesView::OnEnChangeEdittitlu)
	ON_EN_CHANGE(IDC_MAS_INTRG, &CPropertiesView::OnCbnSelchangeMasuraCombo)
	ON_CBN_SELCHANGE(IDC_CHEIECOMBO, &CPropertiesView::OnCbnSelchangeCheiecombo)
	ON_CBN_SELCHANGE(IDC_MASURA_COMBO, &CPropertiesView::OnCbnSelchangeMasuraCombo)
END_MESSAGE_MAP()


// CPropertiesView diagnostics

#ifdef _DEBUG
void CPropertiesView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropertiesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPropertiesView message handlers


void CPropertiesView::OnEnChangeEdittitlu()
{
	UpdateData();
	CArpegioDoc* pDoc = (CArpegioDoc*)GetDocument();
	pDoc->UpdateTitlu(m_titlu);
}


void CPropertiesView::OnCbnSelchangeCheiecombo()
{
	UpdateData();
	CArpegioDoc* pDoc = (CArpegioDoc*)GetDocument();
	pDoc->UpdateCheie((Cheie)m_Cheie);
}


void CPropertiesView::OnCbnSelchangeMasuraCombo()
{
	UpdateData();
	CArpegioDoc* pDoc = (CArpegioDoc*)GetDocument();
	Durata d(m_MasIntregi, Durate::GetDurata(m_Masura).get_numitor(), false);
	pDoc->UpdateMasura(d);
}


void CPropertiesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CArpegioDoc* pDoc = (CArpegioDoc*)GetDocument();

	m_titlu = pDoc->p.get_titlu().c_str();
	m_Cheie = (int)pDoc->p.get_cheie();
	m_MasIntregi = pDoc->p.get_masura().get_numarator();
	m_Masura = Durate::GetPos(Durata(pDoc->p.get_masura().get_numitor()));
	
	PopulateNoteList();
	UpdateData(false);
}


void CPropertiesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

void CPropertiesView::PopulateNoteList()
{
	// get document
	CArpegioDoc* pDoc = (CArpegioDoc*)GetDocument();

	// reset contents
	m_NoteListCtrl.ResetContent();

	int size = pDoc->p.get_nr_elemente();
	for (int i = 0; i < size; i++)
	{
		Element* e = pDoc->p.get_element(i);

		// text afisat in lista
		CString mesaj;
		mesaj += Durate::nume_durate[Durate::GetPos(e->get_durata())].c_str();

		// informatii aditionale nota
		if (e->get_tip_element() == TipElement::NOTA)
		{
			// Cast la nota
			Nota* n = (Nota*)e;

			// inaltime
			mesaj += L", " + CString(nume_inaltimi[(int)n->get_inaltime()].c_str());

			// octava
			CString octava;
			octava.Format(L"%d", n->get_octava());
			mesaj += L", " + octava;
		}

		// adaugare text in lista note
		m_NoteListCtrl.AddString(mesaj);
	}
}