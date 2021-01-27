
// ArpegioDoc.cpp : implementation of the CArpegioDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Arpegio.h"
#endif

#include "ArpegioDoc.h"
#include "AddElemDlg.h"
#include "MIDIPlayer.h"

#include "DocumentView.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CArpegioDoc

IMPLEMENT_DYNCREATE(CArpegioDoc, CDocument)

BEGIN_MESSAGE_MAP(CArpegioDoc, CDocument)
	ON_COMMAND(ID_ELEMENT_ADAUGARE, &CArpegioDoc::OnElementAdaugare)
	ON_COMMAND(ID_ELEMENT_STERGERE, &CArpegioDoc::OnElementStergere)
	ON_COMMAND(ID_ELEMENT_MODIFICARE, &CArpegioDoc::OnElementModificare)
	ON_COMMAND(ID_ELEMENT_REM_SEL, &CArpegioDoc::OnElementRemSel)
END_MESSAGE_MAP()


// CArpegioDoc construction/destruction

CArpegioDoc::CArpegioDoc() noexcept :
	sel{ -1 },
	disabled { false }
{

}

CArpegioDoc::~CArpegioDoc()
{
}

BOOL CArpegioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// New document initialization
	p = Portativ(L"Titlu");
	sel = -1;

	return TRUE;
}


void CArpegioDoc::UpdateTitlu(CString titlu)
{
	// prevent document modify
	if (disabled)
		return;

	p.set_titlu(titlu.GetString());
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::UpdateCheie(Cheie cheie)
{
	// prevent document modify
	if (disabled)
		return;

	p.set_cheie(cheie);
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::UpdateMasura(Durata masura)
{
	// prevent document modify
	if (disabled)
		return;

	p = Portativ(p.get_titlu(), masura, p.get_cheie());
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::SetSelected(int i)
{
	// prevent document modify
	if (disabled)
		return;

	if (i < -1 || i >= p.get_nr_elemente())
		return;

	sel = i;
	UpdateAllViews(NULL, 0, NULL);
}

void CArpegioDoc::SetPlay(int p)
{
	sel = p;
}

int CArpegioDoc::GetSelected()
{
	return sel;
}

bool CArpegioDoc::GetDisabled()
{
	return disabled;
}

void CArpegioDoc::SetDisabled(bool dis)
{
	disabled = dis;
}


// CArpegioDoc serialization

void CArpegioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// informatii portativ
		CString titlu(p.get_titlu().c_str());
		ar << titlu << p.get_masura().get_numarator() << p.get_masura().get_numitor() << (int) p.get_cheie();

		// adaugam elemente
		ar << p.get_nr_elemente();
		int size = p.get_nr_elemente();
		for (int i = 0; i < size; i++)
		{
			Element* c = p.get_element(i);

			// informatii standard element
			ar << (int)c->get_tip_element() << c->get_durata().get_numarator() << c->get_durata().get_numitor();
			
			// informatii specifice nota
			if (c->get_tip_element() == TipElement::NOTA) {
				ar << ((Nota*)c)->get_octava() << (int)((Nota*)c)->get_inaltime();
			}
		}
	}
	else
	{
		// informatii portativ
		CString titlu;
		int masura_numarator, masura_numitor, cheie, nr_elem;
		ar >> titlu >> masura_numarator >> masura_numitor >> cheie;

		// initializare portativ
		p = Portativ(titlu.GetString(), Durata(masura_numarator, masura_numitor, false), (Cheie)cheie);

		// citire elemente
		ar >> nr_elem;
		for (int i = 0; i < nr_elem; i++)
		{
			// informatii element
			int tip_elem, dur_numarator, dur_numitor, octava, inaltime;
			ar >> tip_elem >> dur_numarator >> dur_numitor;

			if ((TipElement)tip_elem == TipElement::NOTA)
			{
				ar >> octava >> inaltime;
				p.add_element(Nota(Durata(dur_numarator, dur_numitor), (Inaltime)inaltime, octava));
			}
			else
			{
				p.add_element(Pauza(Durata(dur_numarator, dur_numitor)));
			}
		}
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CArpegioDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CArpegioDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CArpegioDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CArpegioDoc diagnostics

#ifdef _DEBUG
void CArpegioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CArpegioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CArpegioDoc commands


void CArpegioDoc::OnElementAdaugare()
{
	// prevent document modify
	if (disabled)
		return;

	// run dialog
	AddElemDlg d;
	if (d.DoModal() == IDCANCEL)
		return;

	// add selected element
	bool success = false;
	if (d.GetTipElement() == TipElement::NOTA)
		success = p.add_element(d.GetNota());
	else if (d.GetTipElement() == TipElement::PAUZA)
		success = p.add_element(d.GetPauza());

	// error message if not enough space in current measure
	if(!success)
		AfxMessageBox(L"Durata prea mare pentru măsura curentă");

	// update views
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::OnElementStergere()
{
	// prevent document modify
	if (disabled)
		return;

	p.remove_element();
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::OnElementModificare()
{
	// prevent document modify
	if (disabled)
		return;

	// no element selected
	if (sel == -1)
		return;

	// get element
	Element* e = p.get_element(sel);
	if (e == NULL)
		return;

	// initialize dialog
	AddElemDlg d(false);

	if (e->get_tip_element() == TipElement::NOTA)
		d.SetElement(*(Nota*)e);
	else if(e->get_tip_element() == TipElement::PAUZA)
		d.SetElement(*(Pauza*)e);

	// do not continue if dialog canceled
	if (d.DoModal() == IDCANCEL)
		return;

	// get updated element data from dialog
	if (d.GetTipElement() == TipElement::NOTA)
		p.replace_element(d.GetNota(), sel);
	else if (d.GetTipElement() == TipElement::PAUZA)
		p.replace_element(d.GetPauza(), sel);

	// update views
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::OnElementRemSel()
{
	// prevent document modify
	if (disabled)
		return;

	// no element selected
	if (sel == -1)
		return;

	// get element
	Element* e = p.get_element(sel);
	if (e == NULL)
		return;

	// doar pentru note
	if (e->get_tip_element() != TipElement::NOTA)
		return;

	// replace nota
	Pauza pauza = Pauza(e->get_durata());
	p.replace_element(pauza, sel);

	// update views
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::ModNotaSel(bool lower)
{
	// prevent document modify
	if (disabled)
		return;

	// no element selected
	if (sel == -1)
		return;

	// get element
	Element* e = p.get_element(sel);
	if (e == NULL)
		return;

	// doar pentru note
	if (e->get_tip_element() != TipElement::NOTA)
		return;

	Nota* n = (Nota*)e;

	// decrease by one
	int dif = lower ? -1 : 1;
	int i = (int)n->get_inaltime() + dif;
	int octava = n->get_octava();
	if (i < 0 || i >= 7)
	{
		octava += dif;
		i = lower ? 6 : 0;
	}

	// replace nota
	Nota mod = Nota(n->get_durata(), (Inaltime)i, octava);
	p.replace_element(mod, sel);

	// update views
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}
