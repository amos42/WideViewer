
// TransBitmapTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTransBitmapTestApp:
// �� Ŭ������ ������ ���ؼ��� TransBitmapTest.cpp�� �����Ͻʽÿ�.
//

class CTransBitmapTestApp : public CWinApp
{
public:
	CTransBitmapTestApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CTransBitmapTestApp theApp;