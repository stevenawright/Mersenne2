// M_Test.h : main header file for the M_TEST application
//

#if !defined(AFX_M_TEST_H__BB607601_F83E_4726_963D_D7B8BAA4D5F7__INCLUDED_)
#define AFX_M_TEST_H__BB607601_F83E_4726_963D_D7B8BAA4D5F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CM_TestApp:
// See M_Test.cpp for the implementation of this class
//

class CM_TestApp : public CWinApp
{
public:
	CM_TestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CM_TestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CM_TestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_M_TEST_H__BB607601_F83E_4726_963D_D7B8BAA4D5F7__INCLUDED_)
