#include "pch.h"
#include "afxdialogex.h"

#include "Arpegio.h"
#include "AppAboutDlg.h"

// AppAboutDlg dialog

CAppAboutDlg::CAppAboutDlg() noexcept : CDialogEx(IDD_AppAboutBox)
{
}

void CAppAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAppAboutDlg, CDialogEx)
END_MESSAGE_MAP()
