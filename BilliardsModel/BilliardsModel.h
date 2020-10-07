
// BilliardsModel.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CBilliardsModelApp:
// Сведения о реализации этого класса: BilliardsModel.cpp
//

class CBilliardsModelApp : public CWinApp
{
public:
	CBilliardsModelApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CBilliardsModelApp theApp;
