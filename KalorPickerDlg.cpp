
// KalorPickerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "KalorPicker.h"
#include "KalorPickerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKalorPickerDlg dialog

CKalorPickerDlg::CKalorPickerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KALORPICKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKalorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKalorPickerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CKalorPickerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKalorPickerDlg::OnBnClickedCancel)
	ON_WM_HOTKEY()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_3_ZOOMIN, &CKalorPickerDlg::OnZoomin)
	ON_COMMAND(ID_3_ZOOMOUT, &CKalorPickerDlg::OnZoomout)
	ON_COMMAND(ID_3_EXIT, &CKalorPickerDlg::OnZExit)
END_MESSAGE_MAP()


// CKalorPickerDlg message handlers

BOOL CKalorPickerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	WIDTH = 60; HEIGHT = 36;
	zoomRate = 5;
	dragging = false;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	RegisterHotKey(this->m_hWnd, HK_GET, MOD_CONTROL, VK_OEM_3); // " Ctrl + ` "
	RegisterHotKey(this->m_hWnd, HK_GETRGB, MOD_CONTROL | MOD_SHIFT, VK_OEM_3); // " Ctrl + Shift + ` "

	CRect rect;
	CWnd* dWnd;
	dWnd = GetDesktopWindow();
	dWnd->GetWindowRect(rect);
	delete[] dWnd;

	MoveWindow(rect.right - 300, 0, WIDTH, HEIGHT, 1);

	SetTimer(7837, 100, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKalorPickerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKalorPickerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKalorPickerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKalorPickerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}

void CKalorPickerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();
}


void CKalorPickerDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: Add your message handler code here and/or call default

	if (nHotKeyId == HK_GET || nHotKeyId == HK_GETRGB) {
		CPoint curPos;
		GetCursorPos(&curPos);
		HDC hDesktopDC = ::GetDC(NULL);
		if (hDesktopDC != NULL) {
			COLORREF color = GetPixel(hDesktopDC, cursorPos.x, cursorPos.y);

			wchar_t colorStr[20];
			if(nHotKeyId == HK_GET) 
				wsprintf(colorStr, L"#%.2X%.2X%.2X", GetRValue(color), GetGValue(color), GetBValue(color));
			else
				wsprintf(colorStr, L"RGB(%d, %d, %d)", GetRValue(color), GetGValue(color), GetBValue(color));
			CString col(colorStr);
			
			if (::OpenClipboard(NULL))
			{
				EmptyClipboard();

				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (col.GetLength() + 1) * sizeof(TCHAR));
				if (hMem != NULL)
				{
					LPTSTR lpMem = (LPTSTR)GlobalLock(hMem);
					_tcscpy_s(lpMem, col.GetLength() + 1, col);
					GlobalUnlock(hMem);

					SetClipboardData(CF_UNICODETEXT, hMem);
				}

				CloseClipboard();

				GlobalFree(hMem);
			}
		}
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CKalorPickerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//Invalidate();

	if (nIDEvent == 7837) {
		GetCursorPos(&cursorPos);
		CRect rect(cursorPos.x - WIDTH / 2, cursorPos.y - HEIGHT / 2, cursorPos.x + WIDTH / 2, cursorPos.y + HEIGHT / 2);
		CBitmap bmp;
		CDC dc;
		CDC memDC;

		dc.Attach(::GetDC(NULL));
		memDC.CreateCompatibleDC(&dc);
		bmp.CreateCompatibleBitmap(&dc, WIDTH, HEIGHT);
		CBitmap* pOldBitmap = memDC.SelectObject(&bmp);
		memDC.BitBlt(0, 0, WIDTH, HEIGHT, &dc, rect.left, rect.top, SRCCOPY);

		CDC* pDC = GetDC();
		pDC->StretchBlt(0, 0, WIDTH, HEIGHT, &memDC, WIDTH / 2 - WIDTH / 2.0 / zoomRate, HEIGHT / 2 - HEIGHT / 2.0 / zoomRate, WIDTH / zoomRate, HEIGHT / zoomRate, SRCCOPY);

		pDC->SelectObject(&pen);
		pDC->MoveTo(0, HEIGHT / 2);
		pDC->LineTo(WIDTH, HEIGHT / 2);
		pDC->MoveTo(WIDTH / 2, 0);
		pDC->LineTo(WIDTH / 2, HEIGHT);


		CRect wndRect;
		GetWindowRect(&wndRect);
		COLORREF color = GetPixel(dc, cursorPos.x, cursorPos.y);
		CBrush* brush;
		brush = new CBrush(color);
		dc.SelectObject(brush);
		dc.SelectStockObject(NULL_PEN);
		dc.Rectangle(wndRect.left - 10, wndRect.top, wndRect.left, wndRect.top + HEIGHT + 1);
		delete brush;

		memDC.SelectObject(pOldBitmap);
		ReleaseDC(pDC);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CKalorPickerDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu menu;
	menu.LoadMenu(IDR_MENU); // Load the context menu resource
	CMenu* pContextMenu = menu.GetSubMenu(0); // Assuming the context menu is the first submenu
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CKalorPickerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1012);

	CPoint pt = point;
	CRect rect;
	GetWindowRect(&rect);
	ClientToScreen(&pt);
	offset.x = pt.x - rect.left;
	offset.y = pt.y - rect.top;

	dragging = true;
	SetCapture();
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CKalorPickerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	dragging = false;
	ReleaseCapture();
	SetTimer(1012, 100, NULL);
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CKalorPickerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (dragging) {
		CPoint pt = point;
		ClientToScreen(&pt);

		MoveWindow(pt.x - offset.x, pt.y - offset.y, WIDTH, HEIGHT, 1);
		//SetWindowPos(NULL, origin.x + (point.x - prev.x), origin.y + (point.y - prev.y), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CKalorPickerDlg::OnZoomin()
{
	// TODO: Add your command handler code here
	if(zoomRate <= 5) {
		zoomRate++;
	}
}


void CKalorPickerDlg::OnZoomout()
{
	// TODO: Add your command handler code here
	if (zoomRate >= 2) {
		zoomRate--;
	}
}


void CKalorPickerDlg::OnZExit()
{
	// TODO: Add your command handler code here
	KillTimer(7837);
	PostQuitMessage(WM_QUIT);

//	CDialogEx::OnCancel();
}
