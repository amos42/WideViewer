
// TransBitmapTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TransBitmapTest.h"
#include "TransBitmapTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "TransBitmapTable.h"
#include "TransBitmapAux.h"
#include "yuvrgb.h"

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

static HBITMAP genBitmap(HDC hDC, int width, int height, const void *buf)
{
	//BITMAPINFO bmi;
	BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * 3;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

	HBITMAP hBmp = CreateDIBitmap(hDC, &bmi.bmiHeader, CBM_INIT, buf, &bmi, DIB_RGB_COLORS);

	return hBmp;
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CTransBitmapTestDlg ��ȭ ����



CTransBitmapTestDlg::CTransBitmapTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransBitmapTestDlg::IDD, pParent)
	, m_bYUV422p(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransBitmapTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUT_IMAGE, m_stOutImage);
	DDX_Control(pDX, IDC_SRC_IMAGE, m_stSrcImage);
	DDX_Check(pDX, IDC_CHECK1, m_bYUV422p);
}

BEGIN_MESSAGE_MAP(CTransBitmapTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CTransBitmapTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTransBitmapTestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTransBitmapTestDlg �޽��� ó����

BOOL CTransBitmapTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CTransBitmapTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CTransBitmapTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CTransBitmapTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CTransBitmapTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}


void CTransBitmapTestDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), "testpic.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
	if(hBitmap == NULL || hBitmap == INVALID_HANDLE_VALUE){
		return;
	}

	m_stSrcImage.SetBitmap(hBitmap);

	int nVideoWidth = 320;
	int nVideoHeight = 240;
	BYTE *pRGBbuffer = (BYTE *)malloc(320*240*4);

	BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = nVideoWidth;
    bmi.bmiHeader.biHeight = -nVideoHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = nVideoWidth * nVideoHeight * 3;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;
	GetDIBits(GetDC()->GetSafeHdc(), hBitmap, 0, nVideoHeight, pRGBbuffer, &bmi, DIB_RGB_COLORS);

	::DeleteObject(hBitmap);


	int outWidth = (int)(2 * 3.14 * 100);
	int outHeight = 120 - 30;
	//int outWidth = 320;
	//int outHeight = 240; //980-280;
	outWidth += 4 - outWidth % 4;

	int sz = GetRequireHandleSize(outWidth, outHeight, TRANS_OPT_RGB24 | TRANS_OPT_SPEED);
	void* handle = malloc(sz);
	memset(handle, 0, sz);

	int srcStride[1];
	srcStride[0] = nVideoWidth;
	InitTransBitmapHandle(handle, outWidth, outHeight, nVideoWidth/2, nVideoHeight/2, 30, 120, 0, 360, nVideoWidth, nVideoHeight, 
					      srcStride, 1,
						  1, 1, 0,
						  TRANS_OPT_RGB24 | TRANS_OPT_SPEED, NULL);

	BYTE *pRGBbufferOut = (BYTE *)new char[outWidth * outHeight * 4];
	memset(pRGBbufferOut, 0, outWidth * outHeight * 3);

	TransBitmapByHandle(handle, pRGBbufferOut, pRGBbuffer, 0);

	free(handle);

	HBITMAP hBmp = genBitmap(GetDC()->GetSafeHdc(), outWidth, outHeight, (const void *)pRGBbufferOut);
	if(hBmp != NULL && hBmp != INVALID_HANDLE_VALUE){
		m_stOutImage.SetBitmap(hBmp);
	}
	if(hBmp != NULL) ::DeleteObject(hBmp);
}


void CTransBitmapTestDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();

	char *filename;
	if(m_bYUV422p){
		filename = "testpic_320x240_yuv422p.yuv";
	} else {
		filename = "testpic_320x240_yuv420p.yuv";
	}

	FILE *fp;
	errno_t err = fopen_s(&fp, filename, "rb"); 
	if(err != 0) return;
	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *pYUVbuffer = (char *)malloc(len);
	fread(pYUVbuffer, len, 1, fp);
	fclose(fp);

	int nVideoWidth = 320;
	int nVideoHeight = 240;

	BYTE *pRGBbuffer = (BYTE *)malloc(nVideoWidth * nVideoHeight * 3);

	if(m_bYUV422p){
		YUV422_to_RGB24(nVideoWidth, nVideoHeight, (const BYTE *)pYUVbuffer, pRGBbuffer, 1);
	} else {
		YUV420_to_RGB24(nVideoWidth, nVideoHeight, (const BYTE *)pYUVbuffer, pRGBbuffer, 1);
	}

	HBITMAP hBmp = genBitmap(GetDC()->GetSafeHdc(), nVideoWidth, nVideoHeight, (const void *)pRGBbuffer);
	if(hBmp != NULL && hBmp != INVALID_HANDLE_VALUE){
		m_stSrcImage.SetBitmap(hBmp);
		::DeleteObject(hBmp);
	}
	free(pRGBbuffer);

	int outWidth = (int)(2 * 3.14 * 100);
	int outHeight = 120 - 30;
	outWidth += 4 - outWidth % 4;

	int sz;
	if(m_bYUV422p){
		sz = outWidth * outHeight + outWidth * outHeight / 2 + outWidth * outHeight / 2;
	} else {
		sz = outWidth * outHeight + outWidth * outHeight / 4 + outWidth * outHeight / 4;
	}
	BYTE *pYUVbufferOut = (BYTE *)malloc(sz);
	//memset(pYUVbufferOut, 0, outWidth * outHeight * 3);

#if 1
	DWORD dwOp = TRANS_OPT_SPEED;
	if(m_bYUV422p){
		dwOp |= TRANS_OPT_YUV422;
	} else {
		dwOp |= TRANS_OPT_YUV420;
	}
	int handleSize = GetRequireHandleSize(outWidth, outHeight, dwOp);
	void* handle = malloc(handleSize);
	//memset(handle, 0, sz);

	int srcStride[2];
	srcStride[0] = nVideoWidth;
	srcStride[1] = nVideoWidth/2;
	InitTransBitmapHandle(handle, outWidth, outHeight, nVideoWidth/2, nVideoHeight/2, 30, 120, 0, 360, nVideoWidth, nVideoHeight, 
					      srcStride, 2,
						  1, 1, 0,
						  dwOp, NULL);

	TransBitmapByHandle(handle, pYUVbufferOut, (BYTE *)pYUVbuffer, 0);

	free(handle);
#else 
	int tblSz = outWidth * outHeight * sizeof(int) + outWidth * outHeight * sizeof(int);
	int *transTbl = (int *)malloc(tblSz);
	int *transTbl2 = transTbl + outWidth * outHeight;

	if(m_bYUV422p){
		MakeYUV422TransTable0(transTbl, transTbl2, transTbl2, outWidth, outHeight, nVideoWidth/2, nVideoHeight/2, 30, 120,
							nVideoWidth, nVideoHeight,
							nVideoWidth, nVideoWidth/2, nVideoWidth/2,
							0, 360,
							0, 0, outWidth, outHeight, 
							NULL);
		MakeYUV422Bitmap0(transTbl, transTbl2, transTbl2, 
						pYUVbufferOut, pYUVbufferOut + outWidth * outHeight, pYUVbufferOut + outWidth * outHeight + outWidth * outHeight / 2,
						outWidth, outHeight, 
						(BYTE *)pYUVbuffer, (BYTE *)pYUVbuffer + nVideoWidth * nVideoHeight, (BYTE *)pYUVbuffer + nVideoWidth * nVideoHeight + nVideoWidth * nVideoHeight / 2, 
						0, 0);
	} else {
		MakeYUV420TransTable0(transTbl, transTbl2, transTbl2, outWidth, outHeight, nVideoWidth/2, nVideoHeight/2, 30, 120,
							nVideoWidth, nVideoHeight,
							nVideoWidth, nVideoWidth/2, nVideoWidth/2,
							0, 360,
							0, 0, outWidth, outHeight, 
							NULL);
		MakeYUV420Bitmap0(transTbl, transTbl2, transTbl2, 
						pYUVbufferOut, pYUVbufferOut + outWidth * outHeight, pYUVbufferOut + outWidth * outHeight + outWidth * outHeight / 4,
						outWidth, outHeight, 
						(BYTE *)pYUVbuffer, (BYTE *)pYUVbuffer + nVideoWidth * nVideoHeight, (BYTE *)pYUVbuffer + nVideoWidth * nVideoHeight + nVideoWidth * nVideoHeight / 4, 
						0, 0);
	}

	free(transTbl);
#endif

	char *pRGBbufferOut = (char *)malloc(outWidth * outHeight * 3);

	if(m_bYUV422p){
		YUV422_to_RGB24(outWidth, outHeight, pYUVbufferOut, (BYTE *)pRGBbufferOut, 1);
	} else {
		YUV420_to_RGB24(outWidth, outHeight, pYUVbufferOut, (BYTE *)pRGBbufferOut, 1);
	}

	//BITMAPINFO bmi;
	hBmp = genBitmap(GetDC()->GetSafeHdc(), outWidth, outHeight, (const void *)pRGBbufferOut);
	if(hBmp != NULL && hBmp != INVALID_HANDLE_VALUE){
		m_stOutImage.SetBitmap(hBmp);
		::DeleteObject(hBmp);
	} else {
		m_stOutImage.SetBitmap(NULL);
	}
	free(pRGBbufferOut);

	free(pYUVbufferOut);
	free(pYUVbuffer);
}
