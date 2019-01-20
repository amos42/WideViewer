
// TransBitmapTestDlg.cpp : 구현 파일
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

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

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

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CTransBitmapTestDlg 대화 상자



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


// CTransBitmapTestDlg 메시지 처리기

BOOL CTransBitmapTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTransBitmapTestDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTransBitmapTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CTransBitmapTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CTransBitmapTestDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
