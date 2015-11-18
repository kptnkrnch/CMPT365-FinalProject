
// CMPT365-A2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCMPT365A2App:
// See CMPT365-A2.cpp for the implementation of this class
//

class CCMPT365A2App : public CWinApp
{
public:
	CCMPT365A2App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCMPT365A2App theApp;