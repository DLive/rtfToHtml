// rtftest2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rtftest2.h"
#include "rtftest2Dlg.h"
#include "DLrtfhtml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
#define tochina(s) s
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Crtftest2Dlg �Ի���




Crtftest2Dlg::Crtftest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Crtftest2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Crtftest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Crtftest2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &Crtftest2Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Crtftest2Dlg ��Ϣ�������

BOOL Crtftest2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Crtftest2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Crtftest2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Crtftest2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Crtftest2Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hDLL;
	hDLL=LoadLibrary(_T("DLRtfToHtml.dll"));
	typedef CString(*pf)(CString rtfpath);
	pf mpf;
	mpf=(pf)GetProcAddress(hDLL,"rtfToHtml");
	CString str;
	str=mpf(_T("F:\\\\itbook2.tit"));
	DLrtfhtml r;
	r.beginChange(_T("F:\\\\itbook2.tit"));
	//char *src,*endstr;
	//char *s=new char[3];
	//memset(s,0,sizeof(s));
	
	////sprintf(
	//CString a;
	//a.Format(_T("%s"),s);
	//
//	LPSTR   pUnicode   =   "\\'ce\\'d2\'c0\\'b4\\'c1\\'cb"; 
////��һ�����󳤶�,�������Ҫ����ȥ�� 
//int   targetLen=MultiByteToWideChar(936,0,pUnicode,-1,NULL,0);
//
//
//wchar_t*   pTargetData=new   wchar_t[targetLen+1]; 
//memset(pTargetData,0,targetLen+1); 
//
//MultiByteToWideChar(936,0,pUnicode,-1,pTargetData,targetLen);
//	LPWSTR   pUnicode   =   L"������"; 
////��һ�����󳤶�,�������Ҫ����ȥ�� 
//	UINT targetCodepage=CP_SYMBOL;
//	int   targetLen=WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,   NULL,0,NULL,NULL); 
//
//	BYTE*   pTargetData=new   BYTE[targetLen+1]; 
//	memset(pTargetData,0,targetLen+1); 
//
//	WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char   *)pTargetData,targetLen,NULL,NULL);
}
