#include "stdafx.h"
#include "SYNetCardManger.h"


SYNetCardManger* SYNetCardManger::m_instance = NULL;

SYNetCardManger* SYNetCardManger::Instance()
{
	if (m_instance == NULL){
		m_instance = new SYNetCardManger(); 
	}
	return m_instance;
}

SYNetCardManger::SYNetCardManger()
{
	m_hModule				= NULL;
	m_DLLFuncTestAdd		= NULL;
	m_DLLFuncScan			= NULL;
	m_DLLFuncEnableOrDisableWithGUID = NULL;
		//-------------------------------------------------------------------------------
	m_hModule = LoadLibrary(L".\\SYNetCard.dll"); 
	//-------------------------------------------------------------------------------
	if ( m_hModule != NULL )
	{
		//-------------------------------------------------------------------------------
		m_DLLFuncTestAdd = (SYNetCard_TestAdd)GetProcAddress( m_hModule, "TestAdd" );
		if (m_DLLFuncTestAdd == NULL)
		{
			OutputDebugString(L"GetProc fault:TestAdd\n");
			::MessageBox(NULL, L"GetProc fault:TestAdd", L"", MB_OK);
			FreeLibrary(m_hModule);
			return;
		}

		m_DLLFuncScan = (SYNetCard_Scan)GetProcAddress( m_hModule, "Scan" );
		if (m_DLLFuncScan == NULL)
		{
			OutputDebugString(L"GetProc fault:Scan\n");
			::MessageBox(NULL, L"GetProc fault:Scan", L"", MB_OK);
			FreeLibrary(m_hModule);
			return;
		}


		m_DLLFuncEnableOrDisableWithGUID = (SYNetCard_EnableOrDisableWithGUID)GetProcAddress( m_hModule, "EnableOrDisableWithGUID" );
		if (m_DLLFuncEnableOrDisableWithGUID == NULL)
		{
			OutputDebugString(L"GetProc fault:EnableOrDisableWithGUID\n");
			::MessageBox(NULL, L"GetProc fault:EnableOrDisableWithGUID", L"", MB_OK);
			FreeLibrary(m_hModule);
			return;
		}
		

	}else{
		OutputDebugString(L"SYNetCardManger fault:Can't find SYNetCard.dll\n");
		::MessageBox(NULL, L"SYNetCardManger fault:Can't find SYNetCard.dll", L"", MB_OK);
	}
}

SYNetCardManger::~SYNetCardManger()
{
	FreeLibrary(m_hModule);
}


int SYNetCardManger::TestAdd(int a, int b)
{
	return m_DLLFuncTestAdd(a, b);
}

std::vector<SYNetCardInfo*> *SYNetCardManger::Scan(void)
{
	return m_DLLFuncScan();
}

bool SYNetCardManger::EnableOrDisableWithGUID(WCHAR *wszGUID, bool enable)
{
	return m_DLLFuncEnableOrDisableWithGUID(wszGUID, enable);
}