
// TransBitmapTestDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CTransBitmapTestDlg ��ȭ ����
class CTransBitmapTestDlg : public CDialog
{
// �����Դϴ�.
public:
	CTransBitmapTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TRANSBITMAPTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CStatic m_stOutImage;
	CStatic m_stSrcImage;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	int m_bYUV422p;
};
