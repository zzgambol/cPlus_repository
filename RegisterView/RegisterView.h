// RegisterView.h : main header file for the REGISTERVIEW application
//

#if !defined(AFX_REGISTERVIEW_H__348C918C_797F_4D91_9098_EEAEB50A32EC__INCLUDED_)
#define AFX_REGISTERVIEW_H__348C918C_797F_4D91_9098_EEAEB50A32EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRegisterViewApp:
// See RegisterView.cpp for the implementation of this class
//

class CRegisterViewApp : public CWinApp
{
public:
	CRegisterViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRegisterViewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERVIEW_H__348C918C_797F_4D91_9098_EEAEB50A32EC__INCLUDED_)
