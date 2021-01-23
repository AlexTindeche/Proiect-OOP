
// ArpegioView.cpp : implementation of the CDocumentView class
//

#include "pch.h"
#include "framework.h"
#include "resource.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Arpegio.h"
#endif

#include "ArpegioDoc.h"
#include "DocumentView.h"

#include <array>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ######################################
// # CDocumentView
// ######################################

IMPLEMENT_DYNCREATE(CDocumentView, CScrollView)

BEGIN_MESSAGE_MAP(CDocumentView, CScrollView)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// ######################################
// # CDocumentView construction/destruction

CDocumentView::CDocumentView() noexcept
{

}

CDocumentView::~CDocumentView()
{
}


// ######################################
// # Draw constants initialization

int OFFSET_DOCUMENT = 25;
int TITLE_HEIGHT = 100;

int UNITATE_MASURA = 16;
int MASURI_PER_RAND = 3;
int NR_EXTRA = 4;
int INV_THRESHOLD = 3;

int SPACING_LINII = 10;
int SPACING_PORTATIV = 40;
int H_PORTATIV = (4 + 2 * NR_EXTRA) * SPACING_LINII + SPACING_PORTATIV;
int PRE_PORTATIV = 75;
int BAR_WIDTH = 10;


// ######################################
// # DrawSize definitions

int DrawSizes::rowStart(int r)
{
	return contentTop + r * H_PORTATIV + NR_EXTRA * SPACING_LINII;
}

int DrawSizes::colStart(int c)
{
	return contentLeft + (int)(c * elemWidth) + c / elemMasura * BAR_WIDTH;
}


// ######################################
// # Overrides

void CDocumentView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, GetDocumentSize());
}

void CDocumentView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CScrollView::OnUpdate(pSender, lHint, pHint);
	SetScrollSizes(MM_TEXT, GetDocumentSize());
}

void CDocumentView::OnDraw(CDC* pDC)
{
	// incarcare document
	CArpegioDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	// dimensiuni continut
	DrawSizes sizes = GetDrawSizes();

	// titlu
	DrawTitlu(pDC, pDoc->p.get_titlu().c_str(), sizes.rect);

	// setup portative
	DrawPortative(pDC, pDoc->p.get_nr_masuri(), pDoc->p.get_cheie(), pDoc->p.get_masura(), sizes);

	// draw elemente
	DrawElemente(pDC, pDoc);
}


// ######################################
// # Draw helpers

// Draw titlu
void CDocumentView::DrawTitlu(CDC* pDC, CString titlu, CRect client)
{
	// set font
	CFont font;
	font.CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, 0, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Lucida Serif");
	CFont* oldFont = pDC->SelectObject(&font);

	pDC->DrawText(titlu, CRect(client.left, client.top + OFFSET_DOCUMENT, client.right, client.top + OFFSET_DOCUMENT + TITLE_HEIGHT), DT_CENTER);

	// revert to default font
	pDC->SelectObject(oldFont);
}

// Draw portative
void CDocumentView::DrawPortative(CDC* pDC, int nr_masuri, Cheie c, Durata m, DrawSizes draw)
{
	int stop = (nr_masuri - 1) / MASURI_PER_RAND + 1;
	if (stop == 0)
	{
		stop = 1;
	}

	for (int r = 0; r < stop; r++)
	{
		// setup values
		int startTop = draw.rowStart(r);
		int startLeft = draw.contentLeft - PRE_PORTATIV;

		// draw components
		DrawCheie(pDC, c, startTop, startLeft);
		DrawMasura(pDC, m, startTop, startLeft);
		DrawLayoutPortativ(pDC, startTop, startLeft, draw);
	}
}

// Draw elemente
void CDocumentView::DrawElemente(CDC* pDC, CArpegioDoc* pDoc)
{
	Cheie c = pDoc->p.get_cheie();
	int sel = pDoc->GetSelected();

	IterateElements([this, pDC, c, sel](Element* e, int row, int pos, int new_pos, int i_e, int i_m)
	{
		if (e->get_tip_element() == TipElement::NOTA)
			DrawNota(pDC, *((Nota*)e), c, i_e == sel, row, pos);

		if (e->get_tip_element() == TipElement::PAUZA)
			DrawPauza(pDC, *((Pauza*)e), c, i_e == sel, row, pos);
	});
}

// Draw portative - Cheie
void CDocumentView::DrawCheie(CDC* pDC, Cheie c, int startTop, int startLeft)
{
	// bitmap info
	UINT idCheie = c == Cheie::SOL ? IDB_CHEIA_SOL : IDB_CHEIA_FA;
	CSize srcSize(277, 609);
	CSize destSize(2 * SPACING_LINII + SPACING_LINII / 2, 5 * SPACING_LINII + SPACING_LINII / 2);
	
	// insert bitmap
	InsertBitmap(pDC, idCheie, srcSize, destSize, startLeft, startTop);
}

// Draw portative - Masura
void CDocumentView::DrawMasura(CDC* pDC, Durata m, int startTop, int startLeft)
{
	// setup masura font
	CFont masura;
	masura.CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Times New Roman");
	CFont* initial_font = pDC->SelectObject(&masura);

	// bottom number
	pDC->DrawText(
		to_wstring(m.get_numitor()).c_str(),
		CRect(startLeft + 40, startTop + 2 * SPACING_LINII - 6, startLeft + 60, startTop + 4 * SPACING_LINII),
		DT_TOP
	);

	// top number
	pDC->DrawText(
		to_wstring(m.get_numarator()).c_str(),
		CRect(startLeft + 40, startTop - 6, startLeft + 60, startTop + 2 * SPACING_LINII),
		DT_TOP
	);

	// revert font
	pDC->SelectObject(initial_font);
}

// Draw portative - Layout portativ
void CDocumentView::DrawLayoutPortativ(CDC* pDC, int startTop, int startLeft, DrawSizes draw)
{
	// linii portativ
	for (int l = 0; l < 5; l++)
	{
		int inaltime = startTop + l * SPACING_LINII;
		CPoint pLine[] = {CPoint(startLeft, inaltime), CPoint(draw.contentRight, inaltime)};
		pDC->Polyline(pLine, 2);
	}

	// bare masuri
	for (int s = 0; s < MASURI_PER_RAND; s++)
	{
		int pozitie = draw.contentLeft + (int)(draw.elemWidth * ((double)s + 1) * draw.elemMasura) + s * BAR_WIDTH + BAR_WIDTH / 2;

		// ultima bara din rand lipita de marginea portativului
		if (s == MASURI_PER_RAND - 1)
			pozitie = draw.contentRight;

		CPoint pLine[] = {CPoint(pozitie, startTop), CPoint(pozitie, startTop + 4 * SPACING_LINII + 1)};
		pDC->Polyline(pLine, 2);
	}
}

// Draw elemente - Nota
void CDocumentView::DrawNota(CDC* pDC, Nota n, Cheie c, bool selected, int row, int pos)
{
	// note draw info
	bool fill, line, flag, secondflag = false;
	Inaltime i;
	int octava;
	GetNotaData(n, fill, line, flag, secondflag, i, octava);

	// get view draw sizes
	DrawSizes draw = GetDrawSizes();

	// base height (note next over the first line)
	int base = c == Cheie::SOL ? 4 * 7 + (int)Inaltime::FA : 2 * 7 + (int) Inaltime::LA;

	// calculate difference from reference position
	int dif = (octava * 7 + (int)i) - base;
	int dif_min = (-NR_EXTRA - 1) * 2;
	int dif_max = (4 + NR_EXTRA) * 2;

	// limit note position
	if (dif > dif_max)
		dif = dif_max + 1;

	if (dif < dif_min)
		dif = dif_min - 1;

	// check if overflow
	bool overflow = dif > dif_max || dif < dif_min;

	// calculate positions for inverted notes
	auto inv_h = [dif](int x) {
		if (dif >= INV_THRESHOLD)
		{
			return -x - SPACING_LINII;
		}

		return x;
	};

	auto inv_w = [dif](int y) {
		if (dif >= INV_THRESHOLD)
		{
			return y - SPACING_LINII + 1;
		}

		return y;
	};

	// initialize positions
	int startLeft = draw.colStart(pos);
	int startBottom = draw.rowStart(row) + 4 * SPACING_LINII - dif * 5;

	// extra item lines
	if (overflow)
	{
		CPoint pLine[] = {
			CPoint(startLeft - 2, startBottom - SPACING_LINII / 2),
			CPoint(startLeft + SPACING_LINII + 2, startBottom - SPACING_LINII / 2)
		};
		pDC->Polyline(pLine, 2);
	}
	else
	{
		int dir = (dif > 0) ? 1 : -1;
		int start = (dif > 0) ? 4 : 0;
		for (int ln = start; 2 * ln * dir < dif * dir + dir - 1; ln += dir)
		{
			CPoint pLine[] = {
				CPoint(startLeft - 2, startBottom - (ln + dir) * SPACING_LINII + dif * 5),
				CPoint(startLeft + SPACING_LINII + 4, startBottom - (ln + dir) * SPACING_LINII + dif * 5)
			};
			pDC->Polyline(pLine, 2);
		}
	}

	// 2px line colored pen
	CPen penDouble;
	CPen* pOldPen;

	// culoare note
	COLORREF normal_col = overflow ? RGB(255, 0, 0) : RGB(0, 0, 0);
	COLORREF selected_col = overflow ? RGB(255, 150, 0) : RGB(0, 0, 255);
	COLORREF note_col = selected ? selected_col : normal_col;

	penDouble.CreatePen(PS_SOLID | PS_COSMETIC, 2, note_col);
	pOldPen = pDC->SelectObject(&penDouble);

	// fill note circle (color or transparent)
	CBrush* pOldBrush = NULL;
	CBrush brushFill;
	if (fill)
		brushFill.CreateSolidBrush(note_col);
	else
		brushFill.CreateStockObject(NULL_BRUSH);

	pOldBrush = pDC->SelectObject(&brushFill);

	// draw note circle
	pDC->Ellipse(CRect(startLeft, startBottom - SPACING_LINII, startLeft + SPACING_LINII, startBottom));

	// note line
	if (line)
	{
		int startLn = startLeft + inv_w(SPACING_LINII - 1);
		int top = startBottom + inv_h(- SPACING_LINII / 2);
		int bot = startBottom + inv_h(- 4 * SPACING_LINII);
		CPoint pLine[] = {
			CPoint(startLn, top),
			CPoint(startLn, bot)
		};
		pDC->Polyline(pLine, 2);
	}
	
	// note first flag
	if (flag)
	{
		CPoint flag[] = {
			CPoint(startLeft + inv_w(SPACING_LINII - 1), startBottom + inv_h(- 4 * SPACING_LINII)),
			CPoint(startLeft + inv_w((int)((double)3 / 2 * SPACING_LINII)), startBottom + inv_h(- 3 * SPACING_LINII)),
			CPoint(startLeft + inv_w(2 * SPACING_LINII), startBottom + inv_h(- 3 * SPACING_LINII)),
			CPoint(startLeft + inv_w((int)((double)3 / 2 * SPACING_LINII + 1)), startBottom + inv_h(- 2 * SPACING_LINII + (int)(0.3 * SPACING_LINII)))
		};
		pDC->PolyBezier(flag, 4);
	}

	// note second flag
	if (secondflag)
	{
		CPoint secondflag[] = {
			CPoint(startLeft + inv_w(SPACING_LINII - 1), startBottom + inv_h(-3 * SPACING_LINII)),
			CPoint(startLeft + inv_w((int)((double)3 / 2 * SPACING_LINII)), startBottom + inv_h(-2 * SPACING_LINII)),
			CPoint(startLeft + inv_w(2 * SPACING_LINII), startBottom + inv_h(-2 * SPACING_LINII)),
			CPoint(startLeft + inv_w((int)((double)3 / 2 * SPACING_LINII + 1)), startBottom + inv_h(-2 * SPACING_LINII + (int)((double) 0.85 * SPACING_LINII + 1)))
		};

		pDC->PolyBezier(secondflag, 4);
	}

	// reset device context
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

// Draw elemente - Pauza
void CDocumentView::DrawPauza(CDC* pDC, Pauza p, Cheie c, bool selected, int row, int pos)
{
	// draw positions
	DrawSizes draw = GetDrawSizes();
	int startLeft = draw.colStart(pos);
	int startTop = draw.rowStart(row);

	// color
	COLORREF pauza_col = selected ? RGB(0, 0, 255) : RGB(0, 0, 0);

	if (p.get_durata() == Durate::NOTA_INTREAGA || p.get_durata() == Durate::DOIME)
	{
		int left = startLeft;
		int right = left + SPACING_LINII;
		int top = startTop + SPACING_LINII + ((p.get_durata() == Durate::DOIME) ? SPACING_LINII / 2 : 1);
		int bottom = top + SPACING_LINII / 2;
		pDC->FillSolidRect(CRect(left, top, right, bottom), pauza_col);
	}
	else if (p.get_durata() == Durate::PATRIME || p.get_durata() == Durate::OPTIME || p.get_durata() == Durate::SAISPREZECIME)
	{
		UINT idPauza = 0;
		if (p.get_durata() == Durate::PATRIME)
			idPauza = IDB_PAUZA_PATRIME;
		else if (p.get_durata() == Durate::OPTIME)
			idPauza = IDB_PAUZA_OPTIME;
		else if (p.get_durata() == Durate::SAISPREZECIME)
			idPauza = IDB_PAUZA_SAISPREZECIME;

		InsertBitmap(pDC, idPauza, CSize(100, 200), CSize((int)draw.elemWidth, 4 * SPACING_LINII), startLeft, startTop, pauza_col);
	}
}

void CDocumentView::InsertBitmap(CDC* pDC, UINT idResource, CSize srcSize, CSize destSize, int pos_x, int pos_y, COLORREF color)
{
	// un memory DC are initial un bitmap 1x1 monocromatic
	// redimensionarea se face creand un nou bitmap, dar compatibil cu DC-ul initial, care e colorat
	// apoi PATPAINT preia brush-ul selectat si il combina cu destinatia
	// insa initial bitmap-ul este negru si inversat (de la PATPAINT) inseamna ca nu se intampla nimic, si astfel trebuie inversat inca o data

	// memory DC to load bitmap into
	CDC dcMemory;
	CBitmap bmp;

	// create resources and load bitmap into memory DC
	dcMemory.CreateCompatibleDC(pDC);
	bmp.LoadBitmap(idResource);
	dcMemory.SelectObject(&bmp);

	// apply color mask to bitmap
	if (color != 0)
	{
		// second memory DC for color mask
		CDC dcMask;
		CBitmap mask;
		CBrush brushFill;

		// create resources
		dcMask.CreateCompatibleDC(pDC);
		mask.CreateCompatibleBitmap(pDC, srcSize.cx, srcSize.cy);
		brushFill.CreateSolidBrush(color);
		
		// select brush and mask bitmap
		dcMask.SelectObject(&mask);

		// invert mask and apply it to bitmap
		dcMask.BitBlt(0, 0, srcSize.cx, srcSize.cy, &dcMask, 0, 0, DSTINVERT);
		dcMemory.SelectObject(&brushFill);
		dcMemory.BitBlt(0, 0, srcSize.cx, srcSize.cy, &dcMask, 0, 0, PATPAINT);
	}

	// stretch to fit and paste memory device context on document device context
	pDC->DPtoLP(&destSize);
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(pos_x, pos_y, destSize.cx, destSize.cy, &dcMemory, 0, 0, srcSize.cx, srcSize.cy, SRCAND);
}


// ######################################
// # Data helpers

void CDocumentView::IterateElements(function<void(Element*, int, int, int, int, int)> cb)
{
	// get document
	CArpegioDoc* pDoc = GetDocument();
	
	// get vector dimensions
	int nr_elem = pDoc->p.get_nr_elemente();
	int nr_mas = pDoc->p.get_nr_masuri();

	// index in portativ
	int i_e = 0;

	// every row
	for (int r = 0; r * MASURI_PER_RAND < nr_mas; r++)
	{
		// position (column) on current row
		int pos = 0;

		// every measure in current row
		for (int m = 0; m < MASURI_PER_RAND && (m + r * MASURI_PER_RAND < nr_mas); m++)
		{
			// index in masura curenta
			int i_m = 0;

			// every element in measure
			for (Element* e : pDoc->p.get_elem_masura(m + r * MASURI_PER_RAND))
			{
				// calculate position for next element
				int new_pos = pos + e->get_durata() / Durata(UNITATE_MASURA);
				
				// callback with: element, row, pos, new pos, index el, index mas
				cb(e, r, pos, new_pos, i_e, i_m);

				// set position for next element
				pos = new_pos;

				// increment element count
				i_e++; i_m++;
			}
		}
	}
}


// ######################################
// # Data getters

int CDocumentView::GetDocumentHeight()
{
	CArpegioDoc* pDoc = GetDocument();
	int height = (pDoc->p.get_nr_masuri() - 1) / MASURI_PER_RAND + 1;
	if (height == 0)
		height = 1;

	return height;
}

int CDocumentView::GetDocumentMinWidth()
{
	int elemMasura = GetDocument()->p.get_masura() / Durata(UNITATE_MASURA);
	int minWidth = 2 * OFFSET_DOCUMENT + PRE_PORTATIV + elemMasura * MASURI_PER_RAND * SPACING_LINII * 2 + MASURI_PER_RAND * BAR_WIDTH;
	return minWidth;
}

CSize CDocumentView::GetDocumentSize()
{
	DrawSizes draw = GetDrawSizes();
	return CSize(draw.minWidth, draw.documentHeight);
}

CRect CDocumentView::GetClientViewRect()
{
	int minWidth = GetDocumentMinWidth();
	CRect rect;
	GetClientRect(&rect);

	// if client rect smaller than minWidth
	if (rect.right < minWidth)
		rect.right = minWidth;

	return rect;
}

DrawSizes CDocumentView::GetDrawSizes()
{
	// dimensiune continut
	CRect rect = GetClientViewRect();

	// elemente per masura
	int elemMasura = GetDocument()->p.get_masura() / Durata(UNITATE_MASURA);

	// setup values
	int contentLeft = rect.left + OFFSET_DOCUMENT + PRE_PORTATIV;
	int contentRight = rect.right - OFFSET_DOCUMENT;
	int contentTop = rect.top + TITLE_HEIGHT;
	int contentWidth = contentRight - contentLeft - MASURI_PER_RAND * BAR_WIDTH;
	double elemWidth = contentWidth / ((double)elemMasura * MASURI_PER_RAND);
	int minWidth = GetDocumentMinWidth();
	int documentHeight = GetDocumentHeight() * H_PORTATIV + TITLE_HEIGHT;

	// return struct
	return DrawSizes{
		rect,
		contentLeft,
		contentRight,
		contentTop,
		contentWidth,
		elemMasura,
		elemWidth,
		minWidth,
		documentHeight
	};
}

void CDocumentView::GetNotaData(Nota n, bool &fill, bool &line, bool &flag, bool &secondflag, Inaltime &i, int &octava)
{
	i = n.get_inaltime();
	octava = n.get_octava();
	fill = line = flag = secondflag = false;

	if (n.get_durata() <= Durate::DOIME)
		line = true;

	if(n.get_durata() <= Durate::PATRIME)
		fill = true;

	if (n.get_durata() <= Durate::OPTIME)
		flag = true;

	if (n.get_durata() == Durate::SAISPREZECIME)
		secondflag = true;
}


// ######################################
// # CDocumentView diagnostics

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


// ######################################
// # CDocumentView message handlers

int CDocumentView::DocumentClick(CPoint point)
{
	// absolute click position (relative to document view, not to scroll view)
	CPoint p = GetScrollPosition();
	point.y += p.y;

	// get draw size
	DrawSizes draw = GetDrawSizes();

	// get positions in element grid (x - column; y - row)
	int x = 0;
	for (int pos = 0; pos < draw.elemMasura * MASURI_PER_RAND; pos++)
	{
		// if point between the start of this position and the start of the next one
		int startThis = draw.colStart(pos);
		int startNext = draw.colStart(pos + 1);

		if (point.x >= startThis && point.x < startNext)
			x = pos;
	}

	int y = point.y - TITLE_HEIGHT > 0 ? (point.y - TITLE_HEIGHT) / H_PORTATIV : -1;

	// find index of the element at the x, y position
	int sel = -1;
	IterateElements([&sel, x, y](Element* e, int row, int pos, int new_pos, int i_e, int i_m) {
		// if selection between this and next position
		if (x >= pos && x < new_pos && y == row)
			sel = i_e;
		});

	return sel;
}

BOOL CDocumentView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return DoMouseWheel(nFlags, zDelta, pt);
}

void CDocumentView::OnLButtonDown(UINT nFlags, CPoint point)
{
	int sel = DocumentClick(point);
	CArpegioDoc* pDoc = GetDocument();
	pDoc->SetSelected(sel);
}

void CDocumentView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CArpegioDoc* pDoc = GetDocument();
	pDoc->OnElementModificare();
}


// Key navigation

void CDocumentView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CArpegioDoc* pDoc = GetDocument();

	// up / down -> change inaltime si octava nota
	if (nChar == VK_UP)
		pDoc->ModNotaSel();

	if (nChar == VK_DOWN)
		pDoc->ModNotaSel(true);

	// lef / right -> move selected
	if (nChar == VK_LEFT)
	{
		int sel = pDoc->GetSelected();
		sel--;
		if (sel >= 0)
			pDoc->SetSelected(sel);
	}

	if (nChar == VK_RIGHT)
	{
		int sel = pDoc->GetSelected();
		sel++;
		if (sel < pDoc->p.get_nr_elemente())
			pDoc->SetSelected(sel);
	}

	// escape -> reset selected
	if (nChar == VK_ESCAPE)
	{
		pDoc->SetSelected(-1);
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}
