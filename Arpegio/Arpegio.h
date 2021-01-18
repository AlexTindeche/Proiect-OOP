
// Arpegio.h : main header file for the Arpegio application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CArpegioApp:
// See Arpegio.cpp for the implementation of this class
//

class CArpegioApp : public CWinApp
{
public:
	CArpegioApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CArpegioApp theApp;
