
// KalorPickerDlg.h : header file
//

#pragma once

#define HK_GET 1616

// CKalorPickerDlg dialog
class CKalorPickerDlg : public CDialogEx
{
// Construction
public:
	CKalorPickerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KALORPICKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
public:
	int WIDTH, HEIGHT;
	int zoomRate;
	CPen pen;
	CPoint cursorPos;
	BOOL dragging;
	CPoint offset;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnZExit();
};
