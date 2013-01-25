#if !defined(AFX_REGISTERTREECTRL_H__86DE0A0A_E4E2_481F_B9B0_D50797344D92__INCLUDED_)
#define AFX_REGISTERTREECTRL_H__86DE0A0A_E4E2_481F_B9B0_D50797344D92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegisterTreeCtrl window

class CRegisterTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CRegisterTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRegisterTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRegisterTreeCtrl)
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT m_uFlags;
	CStringList m_strlist;

private:
	BOOL DisplayDrives(BOOL);
	BOOL IsExist(HKEY,LPCTSTR);
	HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR strPath , HKEY hKey);
	BOOL FindSubDir(HKEY hKey,LPCTSTR lpSubKey);
	void ExpandItem(HTREEITEM hItem, UINT nCode);
	void DisplayPath(HTREEITEM hParent, LPCTSTR lpSubKey , HKEY hKey);
	CString GetSubKey(HTREEITEM hItem);
	BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch = TRUE);
	void TravelChild(HTREEITEM hItem, int nState);
	void TravelSiblingAndParent(HTREEITEM hItem, int nState);

	void CalcuSelectPath();
	void CalcuSelectSubPath(HTREEITEM hParent,CString strPath);
public:
	BOOL DisplayTree(BOOL);
	CStringList* GetSelectKey();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERTREECTRL_H__86DE0A0A_E4E2_481F_B9B0_D50797344D92__INCLUDED_)
