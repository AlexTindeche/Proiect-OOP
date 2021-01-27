
// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "ArpegioDoc.h"


// Register custom messages for communication from playback thread to UI thread
static UINT WM_PLAYBACK_START = ::RegisterWindowMessage(L"WM_PLAYBACK_START");
static UINT WM_PLAYBACK_UPDATE = ::RegisterWindowMessage(L"WM_PLAYBACK_UPDATE");
static UINT WM_PLAYBACK_END = ::RegisterWindowMessage(L"WM_PLAYBACK_END");


// Helper struct to pass data to playback thread
struct PlaybackThreadData
{
	HWND hWnd;
	CArpegioDoc* pDoc;
	CMutex* mutex;
	bool running;
};


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CStatusBar m_wndStatusBar;
	CSplitterWnd m_wndSplitter;
	bool m_bSplitterCreated = false;

	//bool playbackRunning;
	CMutex playbackMutex;
	CWinThread* playbackThread;
	PlaybackThreadData* playbackData;

public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnPlaybackStart();
	afx_msg void OnPlaybackStop();

	afx_msg LRESULT OnPlaybackStartMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlaybackUpdateMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlaybackEndMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnUpdatePlaybackStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlaybackStop(CCmdUI* pCmdUI);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
};


