// rtftest2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Crtftest2App:
// �йش����ʵ�֣������ rtftest2.cpp
//

class Crtftest2App : public CWinApp
{
public:
	Crtftest2App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Crtftest2App theApp;