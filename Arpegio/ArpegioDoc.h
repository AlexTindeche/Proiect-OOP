
// ArpegioDoc.h : interface of the CArpegioDoc class
//


#pragma once
#include "portativ.h"

class CArpegioDoc : public CDocument
{
protected: // create from serialization only
	CArpegioDoc() noexcept;
	DECLARE_DYNCREATE(CArpegioDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CArpegioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Document operations
public:
	void UpdateMasura(Durata masura);
	void UpdateCheie(Cheie cheie);
	void UpdateTitlu(CString titlu);
	void SetSelected(int i);
	int GetSelected();

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

private:
	int sel;

public:
	Portativ p;
	afx_msg void OnElementAdaugare();
	afx_msg void OnElementStergere();
	afx_msg void OnElementModificare();
	afx_msg void OnElementRemSel();

	void ModNotaSel(bool lower = false);
};
