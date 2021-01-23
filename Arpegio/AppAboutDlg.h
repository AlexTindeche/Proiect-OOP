#pragma once


// CAboutDlg dialog used for App About

class CAppAboutDlg : public CDialogEx
{
public:
	CAppAboutDlg() noexcept;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AppAboutBox };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};