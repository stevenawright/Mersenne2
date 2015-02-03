//
// M_TestDlg.cpp : implementation file
//

#include	"stdafx.h"
#include	"M_Test.h"
#include	"M_TestDlg.h"

#include	<fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

extern ofstream						dout;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
	public:
		CAboutDlg();

	// Dialog Data
		//{{AFX_DATA(CAboutDlg)
		enum { IDD = IDD_ABOUTBOX };
		//}}AFX_DATA

		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CAboutDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		//{{AFX_MSG(CAboutDlg)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

};	// class CAboutDlg : public CDialog

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void 
CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CM_TestDlg dialog

CM_TestDlg::CM_TestDlg
(
	CWnd							* pParent /*=NULL*/
)
	: CDialog(CM_TestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CM_TestDlg)
	m_edit_exponent = _T("2");
	m_edit_message = _T("");
	m_check_fermat = FALSE;
	m_check_lebec = FALSE;
	m_edit_start_range = _T("");
	m_edit_stop_range = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_exponent = 0;
}

void 
CM_TestDlg::DoDataExchange
(
	CDataExchange					* pDX
)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CM_TestDlg)
	DDX_Control(pDX, IDC_SPIN_HUNDREDS, m_spin_hundreds);
	DDX_Control(pDX, IDC_SPIN_THOUSANDS, m_spin_thousands);
	DDX_Text(pDX, IDC_EDIT_EXPONENT, m_edit_exponent);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_edit_message);
	DDX_Check(pDX, IDC_CHECK_FERMAT, m_check_fermat);
	DDX_Check(pDX, IDC_CHECK_LEBEC, m_check_lebec);
	DDX_Text(pDX, IDC_EDIT_START_RANGE, m_edit_start_range);
	DDX_Text(pDX, IDC_EDIT_STOP_RANGE, m_edit_stop_range);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CM_TestDlg, CDialog)
	//{{AFX_MSG_MAP(CM_TestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, OnButtonExecute)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THOUSANDS, OnDeltaposSpinThousands)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HUNDREDS, OnDeltaposSpinHundreds)
	ON_BN_CLICKED(IDC_BUTTON_EXE_RANGE, OnButtonExeRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CM_TestDlg message handlers

BOOL 
CM_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu							* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString						strAboutMenu;

		strAboutMenu.LoadString(IDS_ABOUTBOX);

		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_spin_hundreds.SetRange32(0, 999);
	m_spin_hundreds.SetPos(2);

	m_spin_thousands.SetRange32(0, 999);
	m_spin_thousands.SetPos(0);

	m_exponent = 2;

	return(TRUE);  // return TRUE  unless you set the focus to a control

}	// OnInitDialog

void 
CM_TestDlg::OnSysCommand
(
	UINT							nID, 
	LPARAM							lParam
)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}

}	// OnSysCommand

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void 
CM_TestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC					dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int							cxIcon = GetSystemMetrics(SM_CXICON);
		int							cyIcon = GetSystemMetrics(SM_CYICON);
		CRect						rect;

		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}	// OnPaint

// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR 
CM_TestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;

}	// OnQueryDragIcon

void 
CM_TestDlg::OnDeltaposSpinHundreds
(
	NMHDR							* pNMHDR, 
	LRESULT							* pResult
) 
{
	const int						MAX_BUF_SIZE = 20;

	NM_UPDOWN						* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	char							buf[MAX_BUF_SIZE];

	short							value = m_spin_hundreds.GetPos();

	//dout << "SpinHundreds m_spin_exponent.GetPos ()" << (short)m_spin_hundreds.GetPos () << endl;
	//dout << "SpinHundreds pNMUpDown->iPos " << pNMUpDown->iPos << endl;
	//dout << "SpinHundreds m_exponent " << m_exponent << endl;
	//dout << "value " << value << endl;

	m_exponent -= (m_exponent % 1000);
	m_exponent += value;

	//dout << "SpinHundreds  m_exponent " << m_exponent << endl;

	sprintf (buf, "%d", m_exponent);
	m_edit_exponent = _T(buf);
		
	UpdateData(false);
	
	*pResult = 0;
}

void 
CM_TestDlg::OnDeltaposSpinThousands
(
	NMHDR							* pNMHDR, 
	LRESULT							* pResult
) 
{
	const int						MAX_BUF_SIZE = 20;

	NM_UPDOWN						* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	char							buf[MAX_BUF_SIZE];

	unsigned long					value = (short)m_spin_thousands.GetPos();

	//dout << "SpinThousands m_spin_thousands.GetPos ()" << (short)m_spin_thousands.GetPos () << endl;
	//dout << "SpinThousands pNMUpDown->iPos " << pNMUpDown->iPos << endl;
	//dout << "SpinThousands m_exponent " << m_exponent << endl;
	//dout << "value" << value << endl;

	m_exponent %= 1000;
	m_exponent += value * 1000L;

	//dout << "SpinThousands  m_exponent " << m_exponent << endl;

	sprintf (buf, "%d", m_exponent);
	m_edit_exponent = _T(buf);
		
	UpdateData(false);

	*pResult = 0;

}	// OnDeltaposSpinThousands

#include	<time.h>

void 
CM_TestDlg::OnButtonExecute() 
{

	UpdateData(true);

	if (!m_check_lebec && !m_check_fermat)
	{
		m_edit_message = _T("No Test Was Checked");
		UpdateData(false);

		return;

	}

	dout << "OnButtonExecute m_exponent " << m_exponent << endl;

	if (!m_prime_list.is_prime(m_exponent))
	{
		m_edit_message = _T("Exponent Must Be Prime");
		UpdateData(false);

		return;
	}
	
	m_edit_message = _T("Exponent Is Prime");
	UpdateData(false);

	if (m_check_lebec)
	{
		/*
		if (OtherTest())
		{
			char						buf[80];

			sprintf (buf, "M(%d) Prime According To Other Test", m_exponent);
			dout << buf << endl;

			m_edit_message = _T(buf);
			UpdateData(false);
		}
		else
		{
			char						buf[80];

			sprintf (buf, "M(%d) **NOT** Prime, According To Other Test", m_exponent);
			dout << buf << endl;

			m_edit_message = _T(buf);
			UpdateData(false);
		}

		return;
		*/

		time_t							start_time;

		time(&start_time);

		if (!m_exponent_test.Lebec(m_exponent))
		{
			char						buf[80];

			sprintf (buf, "M(%d) **NOT** Prime, According To Lucas-Lamere Test", m_exponent);
			dout << buf << endl;

			m_edit_message = _T(buf);
			UpdateData(false);

		}
		else
		{
			char						buf[80];

			sprintf (buf, "M(%d) Prime According To Lucas-Lamere Test", m_exponent);
			dout << buf << endl;

			m_edit_message = _T(buf);
			UpdateData(false);
		}

		time_t						end_time;

		time(&end_time);

		double						elapsed_time = difftime(end_time, start_time);

		unsigned long				hours = elapsed_time/3600;
		unsigned long				minutes = (elapsed_time - hours * 60) / 60;
		unsigned long				seconds = elapsed_time - hours * 3600 - minutes * 60;

		dout << hours << ":" << minutes << ":" << seconds << endl;
	}	// if (m_check_lebec)

	if (m_check_fermat)
	{
		time_t							start_time;

		time(&start_time);

		if (!m_exponent_test.FermatTest(3, m_exponent))
		{
			char						buf[80];

			sprintf (buf, "M(%d) **NOT** Prime, According To Little Fermat(3) Test", m_exponent);
			dout << buf << endl;

			m_edit_message = _T(buf);
			UpdateData(false);		
		}
		else
		{
			char						buf[80];

			sprintf (buf, "M(%d) Prime, According To Little Fermat(3) Test", m_exponent);
			dout << buf << endl;

			m_edit_message = _T(buf);
			UpdateData(false);
		}

		time_t						end_time;

		time(&end_time);

		double						elapsed_time = difftime(end_time, start_time);

		unsigned long				hours = elapsed_time/3600;
		unsigned long				minutes = (elapsed_time - hours * 60) / 60;
		unsigned long				seconds = elapsed_time - hours * 3600 - minutes * 60;

		dout << hours << ":" << minutes << ":" << seconds << endl;

	}	// if (m_check_fermat)

}	// OnButtonExecute

void 
CM_TestDlg::OnButtonExeRange() 
{
		
	UpdateData(true);

	if (!m_check_lebec && !m_check_fermat)
	{
		m_edit_message = _T("No Test Was Checked");
		UpdateData(false);

		return;

	}

	unsigned long				start_exponent = atol(m_edit_start_range);
	unsigned long				stop_exponent  = atol(m_edit_stop_range);

	//dout << "start exp " << start_exponent << " stop exp " << stop_exponent << endl;

/*
	if (start_exponent > stop_exponent)
	{
		m_edit_message = _T("Stop Exponent MUST Be Greater Than Start Exponent");
		UpdateData(false);

		return;
	}
*/

//	BigInteger						numerator = start_exponent;
//	BigInteger						denominator = stop_exponent;
//	BigInteger						quotent;
//	BigInteger						remainder;
//
//	std::pair<BigInteger, BigInteger>	result;
//
//	result = numerator / denominator;
//	dout << result.first << ", " << result.second << endl;
//	return;
	
	for (unsigned long i = start_exponent; i <= stop_exponent; ++i)
	{
		if (!m_prime_list.is_prime(i))
		{
			continue;
		}
		
		if (m_check_lebec)
		{
			time_t							start_time;

			time(&start_time);

			if (!m_exponent_test.Lebec(i))
			{
				char						buf[80];

				sprintf (buf, "M(%d) **NOT** Prime, According To Lucas-Lamere Test", i);
				//dout << buf << endl;

				m_edit_message = _T(buf);
				UpdateData(false);

			}
			else
			{
				char						buf[80];

				sprintf (buf, "M(%d) Prime According To Lucas-Lamere Test", i);
				dout << buf << endl;

				m_edit_message = _T(buf);
				UpdateData(false);
			}

			time_t						end_time;

			time(&end_time);

			double						elapsed_time = difftime(end_time, start_time);

			unsigned long				hours = elapsed_time/3600;
			unsigned long				minutes = (elapsed_time - hours * 60) / 60;
			unsigned long				seconds = elapsed_time - hours * 3600 - minutes * 60;

			dout << hours << ":" << minutes << ":" << seconds << endl;
		}	// if (m_check_lebec)

		if (m_check_fermat)
		{
			time_t							start_time;

			time(&start_time);

			if (!m_exponent_test.FermatTest(3, i))
			{
				char						buf[80];

				sprintf (buf, "M(%d) **NOT** Prime, According To Little Fermat(3) Test", i);
				//dout << buf << endl;
				
				m_edit_message = _T(buf);
				UpdateData(false);

				continue;
			}
			else
			{
				char						buf[80];

				sprintf (buf, "M(%d) Prime, According To Little Fermat(3) Test", i);
				dout << buf << endl;

				m_edit_message = _T(buf);
				UpdateData(false);
			}

			time_t						end_time;

			time(&end_time);

			double						elapsed_time = difftime(end_time, start_time);

			unsigned long				hours = elapsed_time/3600;
			unsigned long				minutes = (elapsed_time - hours * 60) / 60;
			unsigned long				seconds = elapsed_time - hours * 3600 - minutes * 60;

			dout << hours << ":" << minutes << ":" << seconds << endl;

		}	// if (m_check_fermat)

	}	// for range

}	// OnButtonExeRange

#include	<algorithm>

bool
CM_TestDlg::OtherTest() 
{

	vector<BigInteger>			lu;
	vector<BigInteger>			lu_2;
	BigInteger					max_value = 0;
	bool						first_round = true;
	BigInteger					mod_value = 1;

	mod_value.power2(m_exponent);
	--mod_value;

dout << "Other - mod_value " << mod_value << endl;

	unsigned long				upper_bound = 4000000L;

	for (unsigned long i = 2; i < upper_bound; ++i)
	{
		BigInteger				t = i;

		t.square();

		if (first_round)
		{
			if (t > mod_value)
			{
				first_round = false;
				upper_bound = i * 4;
dout << "Upper Bound " << upper_bound << endl;
dout << "lu size " << lu.size() << endl;
for (int ii = 0; ii < lu.size(); ++ii) dout << " lu[" << ii << "] = " << lu[ii] << endl;
			}
			else
			{
				lu.push_back(t);
				continue;
			}
		}
		
		t.Mod_2_Pow_N_Minus_1(m_exponent);

dout << " t " << t << endl;

		vector<BigInteger>::iterator	lu_itr;

		lu_itr = find(lu.begin(), lu.end(), t);
		if (lu_itr != lu.end())
		{
			return(false);
		}

		for (unsigned long j = 0; j != lu_2.size(); ++j)
		{
			if (lu_2[j] == t)
			{
				return(false);
			}
		}
		
		lu_2.push_back(t);
		
	}

for (int ii = 0; ii < lu_2.size(); ++ii) dout << "lu_2[" << ii << "] == " << lu_2[ii] << endl;

	return(true);

}	// OtherTest