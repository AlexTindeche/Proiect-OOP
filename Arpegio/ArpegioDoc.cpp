
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
#include "AddNotaDlg.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CArpegioDoc

IMPLEMENT_DYNCREATE(CArpegioDoc, CDocument)

BEGIN_MESSAGE_MAP(CArpegioDoc, CDocument)
	ON_COMMAND(ID_NOTA_ADAUGARE, &CArpegioDoc::OnNotaAdaugare)
	ON_COMMAND(ID_NOTA_STERGERE, &CArpegioDoc::OnNotaStergere)
END_MESSAGE_MAP()


// CArpegioDoc construction/destruction

CArpegioDoc::CArpegioDoc() noexcept :
	sel{ -1 }
{
	// TODO: add one-time construction code here

}

CArpegioDoc::~CArpegioDoc()
{
}

BOOL CArpegioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	p = Portativ(L"Partitură");
	sel = -1;

	return TRUE;
}


void CArpegioDoc::UpdateTitlu(CString titlu)
{
	p.set_titlu(titlu.GetString());
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::UpdateCheie(Cheie cheie)
{
	p.set_cheie(cheie);
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::UpdateMasura(Durata masura)
{
	p = Portativ(p.get_titlu(), masura, p.get_cheie());
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::SetSelected(int i)
{
	if (i < 0 || i >= p.get_nr_elemente())
		sel = -1;

	sel = i;
	UpdateAllViews(NULL, 0, NULL);
}

int CArpegioDoc::GetSelected()
{
	return sel;
}


// CArpegioDoc serialization

void CArpegioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		CString titlu(p.get_titlu().c_str());
		ar << titlu << p.get_masura().get_numarator() << p.get_masura().get_numitor() << (int) p.get_cheie();

		// adaugam elemente
		ar << p.get_nr_elemente();
		int size = p.get_nr_elemente();
		for (int i = 0; i < size; i++)
		{
			Element* c = p.get_element(i);
			ar << (int)c->get_tip_element() << c->get_durata().get_numarator() << c->get_durata().get_numitor();
			if (c->get_tip_element() == TipElement::NOTA) {
				ar << ((Nota*)c)->get_octava() << (int)((Nota*)c)->get_inaltime();
			}
		}
	}
	else
	{
		CString titlu;
		int masura_numarator, masura_numitor, cheie, nr_elem;
		
		ar >> titlu >> masura_numarator >> masura_numitor >> cheie;
		p = Portativ(titlu.GetString(), Durata(masura_numarator, masura_numitor, false), (Cheie)cheie);

		ar >> nr_elem;
		for (int i = 0; i < nr_elem; i++)
		{
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
		//p.add_element(Nota(Durate::OPTIME, Inaltime::DO, 3));
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


void CArpegioDoc::OnNotaAdaugare()
{
	AddNotaDlg d;
	if (d.DoModal() == IDOK)
	{
		if (d.GetTipElement() == TipElement::NOTA)
			p.add_element(d.GetNota());
		else if (d.GetTipElement() == TipElement::PAUZA)
			p.add_element(d.GetPauza());
	}
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}


void CArpegioDoc::OnNotaStergere()
{
	p.remove_element();
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}

void CArpegioDoc::OnNotaModificare(int i_e, bool remove)
{
	AddNotaDlg d(false);

	Element* e = p.get_element(i_e);
	if (e == NULL)
		return;

	if (e->get_tip_element() == TipElement::NOTA)
		d.SetElement(*(Nota*)e);
	else if(e->get_tip_element() == TipElement::PAUZA)
		d.SetElement(*(Pauza*)e);

	if (d.DoModal() == IDOK)
	{
		if (d.GetTipElement() == TipElement::NOTA)
			p.replace_element(d.GetNota(), i_e);
		else if (d.GetTipElement() == TipElement::PAUZA)
			p.replace_element(d.GetPauza(), i_e);
	}
	UpdateAllViews(NULL, 0, NULL);
	SetModifiedFlag();
}