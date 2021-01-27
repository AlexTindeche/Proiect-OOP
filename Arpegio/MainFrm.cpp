
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "Arpegio.h"

#include "MainFrm.h"
#include "PropertiesView.h"

#include "MIDIPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_COMMAND(ID_PLAYBACK_START, &CMainFrame::OnPlaybackStart)
	ON_COMMAND(ID_PLAYBACK_STOP, &CMainFrame::OnPlaybackStop)

	ON_REGISTERED_MESSAGE(WM_PLAYBACK_START, &CMainFrame::OnPlaybackStartMsg)
	ON_REGISTERED_MESSAGE(WM_PLAYBACK_UPDATE, &CMainFrame::OnPlaybackUpdateMsg)
	ON_REGISTERED_MESSAGE(WM_PLAYBACK_END, &CMainFrame::OnPlaybackEndMsg)

	ON_MESSAGE(WM_SETMESSAGESTRING, &CMainFrame::OnSetMessageString)
	ON_UPDATE_COMMAND_UI(ID_PLAYBACK_START, &CMainFrame::OnUpdatePlaybackStart)
	ON_UPDATE_COMMAND_UI(ID_PLAYBACK_STOP, &CMainFrame::OnUpdatePlaybackStop)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	playbackData = new PlaybackThreadData();
	playbackThread = NULL;
}

CMainFrame::~CMainFrame()
{
	delete playbackData;
}


// ######################################
// # View creation and closing

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

void CMainFrame::OnClose()
{
	// if playback thread existing
	if (playbackThread != NULL)
	{
		// send playback stop
		playbackMutex.Lock();
		playbackData->running = false;
		playbackMutex.Unlock();

		// wait for thread to end
		::WaitForSingleObject(playbackThread->m_hThread, INFINITE);
	}

	CFrameWnd::OnClose();
}


// ######################################
// # Overrides

// Modify the Window class or styles
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return TRUE;
}

// Initialize splitter window (Document and properties views)
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.CreateView(0, 0, pContext->m_pNewViewClass, CSize(160, 160), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CPropertiesView), CSize(0, 0), pContext);
	m_bSplitterCreated = true;
	return true;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers


// ######################################
// # Keep aspect ratio between Document and Properties views on window resize

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetWindowRect(&rect);

	if (m_bSplitterCreated)
	{
		m_wndSplitter.SetColumnInfo(0, rect.Width() * 4 / 5, 10);
		m_wndSplitter.SetColumnInfo(1, rect.Width() / 5, 10);
		m_wndSplitter.RecalcLayout();
	}
}


// ######################################
// # Playback worker thread process

bool running = false;
UINT PlaybackThread(LPVOID pParam)
{
	// get main frame
	PlaybackThreadData* pThreadData = (PlaybackThreadData*)pParam;
	if (pThreadData == NULL)
		return 0;

	// get document
	CArpegioDoc* pDoc = pThreadData->pDoc;
	if (pDoc == NULL || !pDoc->IsKindOf(RUNTIME_CLASS(CArpegioDoc)))
		return 0;

	// set running status to true
	pThreadData->mutex->Lock();
	pThreadData->running = true;
	pThreadData->mutex->Unlock();

	// get initial position
	int initial = pDoc->GetSelected();

	// send playback start message (so that UI changes state)
	PostMessage(pThreadData->hWnd, WM_PLAYBACK_START, 0, NULL);

	// initialize playback
	MIDIPlayer p;
	p.play(&pDoc->p, &pThreadData->running, pThreadData->mutex, pThreadData->hWnd);

	// send playback ended message (so that UI reverts state to normal)
	PostMessage(pThreadData->hWnd, WM_PLAYBACK_END, initial, NULL);

	// reset running
	pThreadData->mutex->Lock();
	pThreadData->running = false;
	pThreadData->mutex->Unlock();

	return 1;
}


// ######################################
// # Handle playback menu items commands

void CMainFrame::OnPlaybackStart()
{
	// get document
	CArpegioDoc* pDoc = (CArpegioDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;

	// set data to be sent to the thread and start playback thread
	playbackData->hWnd = m_hWnd;
	playbackData->pDoc = pDoc;
	playbackData->mutex = &playbackMutex;
	playbackData->running = false;

	// start thread
	playbackThread = AfxBeginThread(PlaybackThread, playbackData);
}


void CMainFrame::OnPlaybackStop()
{
	playbackMutex.Lock();
	playbackData->running = false;
	playbackMutex.Unlock();
}


// ######################################
// # Handle playback thread messages

LRESULT CMainFrame::OnPlaybackStartMsg(WPARAM wParam, LPARAM lParam)
{
	// get document
	CArpegioDoc* pDoc = (CArpegioDoc*) GetActiveDocument();
	if (pDoc == NULL)
		return -1;

	// update status
	pDoc->SetDisabled(true);

	pDoc->UpdateAllViews(NULL, 0, NULL);
	SetMessageText(IDS_PLAYINGMESSAGE);
	return 0;
}


LRESULT CMainFrame::OnPlaybackUpdateMsg(WPARAM wParam, LPARAM lParam)
{
	// get document
	CArpegioDoc* pDoc = (CArpegioDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return -1;

	// update playback position
	pDoc->SetPlay((int)wParam);
	pDoc->UpdateAllViews(NULL, 0, NULL);
	return 0;
}


LRESULT CMainFrame::OnPlaybackEndMsg(WPARAM wParam, LPARAM lParam)
{
	// get document
	CArpegioDoc* pDoc = (CArpegioDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return -1;

	// update values
	playbackThread = NULL;
	pDoc->SetDisabled(false);
	pDoc->SetPlay((int)wParam);

	// update views
	pDoc->UpdateAllViews(NULL, 0, NULL);
	SetMessageText(AFX_IDS_IDLEMESSAGE);
	return 0;
}


// ######################################
// # Update UI state (menu items and status message)

LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	// change status idle message when playing
	if (wParam == AFX_IDS_IDLEMESSAGE && playbackThread != NULL)
		wParam = IDS_PLAYINGMESSAGE;

	return CFrameWnd::OnSetMessageString(wParam, lParam);
}


void CMainFrame::OnUpdatePlaybackStart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(playbackThread == NULL);
}


void CMainFrame::OnUpdatePlaybackStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(playbackThread != NULL);
}
