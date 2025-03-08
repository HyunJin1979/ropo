
// MFCApplication1Dlg.h: 헤더 파일
//

#pragma once

#include <vector>

using namespace std;

#define IMG_WIDTH			640
#define IMG_HEIGHT		480


// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	CImage m_imgViewer;

	vector<CPoint> m_vecCpCicle;

	int m_iRadius;
	int m_iLineThickness;


	int m_iSelectPoint;
	bool m_bLBtnDown;
	CPoint m_cpLBtnDown;

	int m_iSelectCicle;

	bool m_bTheadState;

public:
	void InsertPoint(CPoint cp);
	void Draw(bool bClear = false);
	void DrawCicle(BYTE* pViewer, int iPitch, int iLineThickness);
	bool IsDrawCicle(int x, int y, CPoint cpCenter, int iRadius);

	int InCicle(CPoint cp, int iRadius);
	void RandomMove();
	void EnableControl(BOOL bEnable);
	

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnInit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnRandom();
	afx_msg void OnBnClickedBtnAdjust();
	afx_msg void OnBnClickedBtnRandomContinue();
};
