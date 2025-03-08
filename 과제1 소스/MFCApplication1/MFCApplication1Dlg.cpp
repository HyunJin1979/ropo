
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


double Deg2Rad(double dDegree)
{
	return dDegree * M_PI / 180;
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_iSelectPoint = -1;
	m_bLBtnDown = false;
	m_iSelectCicle = -1;

	m_iRadius = 6;
	m_iLineThickness = 3;

	m_bTheadState = false;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_RADIUS, m_iRadius);
	DDX_Text(pDX, IDC_EDIT_LINE_THICKNESS, m_iLineThickness);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_INIT, &CMFCApplication1Dlg::OnBnClickedBtnInit)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CMFCApplication1Dlg::OnBnClickedBtnRandom)
	ON_BN_CLICKED(IDC_BTN_ADJUST, &CMFCApplication1Dlg::OnBnClickedBtnAdjust)
	ON_BN_CLICKED(IDC_BTN_RANDOM_CONTINUE, &CMFCApplication1Dlg::OnBnClickedBtnRandomContinue)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_imgViewer.Create(IMG_WIDTH, -IMG_HEIGHT, 8, BI_RGB);

	RGBQUAD rgb[256];
	for (int i = 0; i < 256; i++)
		rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;

	m_imgViewer.SetColorTable(0, 256, rgb);

	UpdateData(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::InsertPoint(CPoint cp)
{
	if (m_vecCpCicle.size() < 3 && cp.x < IMG_WIDTH && cp.y < IMG_HEIGHT)
	{
		m_vecCpCicle.push_back(cp);

		Draw();
	}
}

void CMFCApplication1Dlg::Draw(bool bClear)
{
	int iPitch = m_imgViewer.GetPitch();
	BYTE* pViewer = (BYTE*)m_imgViewer.GetBits();

	if (bClear)
		memset(pViewer, 0x00, iPitch * IMG_HEIGHT);

	DrawCicle(pViewer, iPitch, m_iLineThickness);

	for (int i = 0; i < m_vecCpCicle.size(); i++)
	{
		CPoint cpCenter = m_vecCpCicle[i];

		for (int y = cpCenter.y - m_iRadius; y <= cpCenter.y + m_iRadius; y++)
		{
			if (y >= 0 && y < IMG_HEIGHT)
			{
				for (int x = cpCenter.x - m_iRadius; x <= cpCenter.x + m_iRadius; x++)
				{
					if (x >= 0 && x < IMG_WIDTH)
					{
						if (IsDrawCicle(x, y, cpCenter, m_iRadius) == true)
							pViewer[y * iPitch + x] = 255;
					}
				}
			}
		}
	}

	CClientDC dc(this);
	m_imgViewer.Draw(dc, 0, 0);
}

void CMFCApplication1Dlg::DrawCicle(BYTE* pViewer, int iPitch, int iLineThickness)
{
	if (m_vecCpCicle.size() > 2)
	{
		double dGapX1 = m_vecCpCicle[1].x - m_vecCpCicle[0].x;
		double dGapY1 = m_vecCpCicle[1].y - m_vecCpCicle[0].y;
		double dGapX2 = m_vecCpCicle[2].x - m_vecCpCicle[0].x;
		double dGapY2 = m_vecCpCicle[2].y - m_vecCpCicle[0].y;
		double dTemp1 = dGapX1 * (m_vecCpCicle[0].x + m_vecCpCicle[1].x) + dGapY1 * (m_vecCpCicle[0].y + m_vecCpCicle[1].y);
		double dTemp2 = dGapX2 * (m_vecCpCicle[0].x + m_vecCpCicle[2].x) + dGapY2 * (m_vecCpCicle[0].y + m_vecCpCicle[2].y);
		double dTemp3 = 2.0 * (dGapX1 * (m_vecCpCicle[2].y - m_vecCpCicle[1].y) - dGapY1 * (m_vecCpCicle[2].x - m_vecCpCicle[1].x));

		if (dTemp3 != 0)
		{
			double dX = (dGapY2 * dTemp1 - dGapY1 * dTemp2) / dTemp3;
			double dY = (dGapX1 * dTemp2 - dGapX2 * dTemp1) / dTemp3;
			double dRadius = sqrt(pow(dX - m_vecCpCicle[0].x, 2) + pow(dY - m_vecCpCicle[0].y, 2));


			float radian;
			int x;
			int y;
			for (int i = 0; i < iLineThickness; i++)
			{
				for (double degree = 0; degree < 360; degree += 0.1)
				{
					radian = Deg2Rad(degree);
					x = dX + (dRadius - i) * cos(radian) + 0.5;
					y = dY + (dRadius - i) * sin(radian) + 0.5;

					if (y >= 0 && y < IMG_HEIGHT && x >= 0 && x < IMG_WIDTH)
						pViewer[y * iPitch + x] = 255;
				}
			}
		}
	}
}

bool CMFCApplication1Dlg::IsDrawCicle(int x, int y, CPoint cpCenter, int iRadius)
{
	bool bDraw = false;

	if (((x - cpCenter.x) * (x - cpCenter.x)) + ((y - cpCenter.y) * (y - cpCenter.y))  < iRadius * iRadius)
		bDraw = true;

	return bDraw;
}

int CMFCApplication1Dlg::InCicle(CPoint cp, int iRadius)
{
	int iIndex = -1;

	if (m_vecCpCicle.size() > 2)
	{
		CRect rc;
		for (int i = 0; i < m_vecCpCicle.size(); i++)
		{
			rc.left = m_vecCpCicle[i].x - iRadius;
			rc.top = m_vecCpCicle[i].y - iRadius;
			rc.right = m_vecCpCicle[i].x + iRadius;
			rc.bottom = m_vecCpCicle[i].y + iRadius;

			if (rc.PtInRect(cp) == true)
			{
				iIndex = i;
				break;
			}
		}
	}

	return iIndex;
}

void CMFCApplication1Dlg::RandomMove()
{
	if (m_vecCpCicle.size() > 2)
	{
		for (int i = 0; i < m_vecCpCicle.size(); i++)
		{
			int iMove = (rand() % 50)- 25;
			m_vecCpCicle[i].x += iMove;
			m_vecCpCicle[i].y += iMove;
		}

		Draw(true);
	}
}

void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_bLBtnDown = true;
	m_cpLBtnDown = point;

	m_iSelectCicle = InCicle(point, m_iRadius);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_bLBtnDown)
	{
		if (m_iSelectCicle < 0)
			InsertPoint(point);
		else
		{
			m_vecCpCicle[m_iSelectCicle] = point;
			m_iSelectCicle = -1;
			Draw(true);
		}
		m_bLBtnDown = false;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_iSelectCicle >= 0)
	{
		m_vecCpCicle[m_iSelectCicle] = point;

		Draw(true);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMFCApplication1Dlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMFCApplication1Dlg::OnBnClickedBtnInit()
{
	m_vecCpCicle.clear();
	Draw(true);
}


BOOL CMFCApplication1Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Draw();

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CMFCApplication1Dlg::OnBnClickedBtnRandom()
{
	RandomMove();
}


void CMFCApplication1Dlg::OnBnClickedBtnAdjust()
{
	UpdateData(true);
	Draw(true);
}

void CMFCApplication1Dlg::EnableControl(BOOL bEnable)
{
	GetDlgItem(IDC_BTN_ADJUST)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_INIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(bEnable);
}

void RandomMoveThread(CMFCApplication1Dlg *pMain)
{
	for (int i = 0; i < 10; i++)
	{
		pMain->RandomMove();
		Sleep(500);
	}
}

void CMFCApplication1Dlg::OnBnClickedBtnRandomContinue()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableControl(FALSE);

	thread thread1(RandomMoveThread, this);
	thread1.join();

	EnableControl(TRUE);
}


