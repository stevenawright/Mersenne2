// M_TestDlg.h : header file
//

#if !defined(AFX_M_TESTDLG_H__ED29BB1A_CDBD_46FC_8DBC_658FAEE13B6B__INCLUDED_)
#define AFX_M_TESTDLG_H__ED29BB1A_CDBD_46FC_8DBC_658FAEE13B6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CM_TestDlg dialog
#include	"PrimeList.h"
#include	"ExponentTest.h"

class CM_TestDlg : public CDialog
{
	// Construction
	public:
		CM_TestDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
		//{{AFX_DATA(CM_TestDlg)
	enum { IDD = IDD_M_TEST_DIALOG };
	CSpinButtonCtrl	m_spin_hundreds;
	CSpinButtonCtrl	m_spin_thousands;
		CString						m_edit_exponent;
	CString	m_edit_message;
	BOOL	m_check_fermat;
	BOOL	m_check_lebec;
	CString	m_edit_start_range;
	CString	m_edit_stop_range;
	//}}AFX_DATA

		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CM_TestDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		HICON						m_hIcon;

		// Generated message map functions
		//{{AFX_MSG(CM_TestDlg)
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnDeltaposSpinExponent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonExecute();
	afx_msg void OnDeltaposSpinThousands(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinHundreds(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonExeRange();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	private:
		PrimeList					m_prime_list;
		ExponentTest				m_exponent_test;

		unsigned long				m_exponent;

		bool						OtherTest();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_M_TESTDLG_H__ED29BB1A_CDBD_46FC_8DBC_658FAEE13B6B__INCLUDED_)
