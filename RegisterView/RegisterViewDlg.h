// RegisterViewDlg.h : header file
//

#if !defined(AFX_REGISTERVIEWDLG_H__C93BEFA1_7F81_48F2_91A7_EE6CDCC72285__INCLUDED_)
#define AFX_REGISTERVIEWDLG_H__C93BEFA1_7F81_48F2_91A7_EE6CDCC72285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRegisterViewDlg dialog
#include "RegisterTreeCtrl.h"

class CRegisterViewDlg : public CDialog
{
// Construction
public:
	CRegisterViewDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRegisterViewDlg)
	enum { IDD = IDD_REGISTERVIEW_DIALOG };
	CRegisterTreeCtrl	m_tree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRegisterViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList m_imgList;
	CImageList m_imgState;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERVIEWDLG_H__C93BEFA1_7F81_48F2_91A7_EE6CDCC72285__INCLUDED_)
