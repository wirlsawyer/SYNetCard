#ifndef _SYNETCARDMANGER_H_
#define _SYNETCARDMANGER_H_

#include "SYNetCardDefine.h"
#include <vector>

typedef int (WINAPI* SYNetCard_TestAdd)(int a, int b);
typedef std::vector<SYNetCardInfo*> *(WINAPI* SYNetCard_Scan)(void);
typedef bool (WINAPI* SYNetCard_EnableOrDisableWithGUID)(WCHAR *wszGUID, bool enable);

class SYNetCardManger
{

public:	
	//creat
	static SYNetCardManger* Instance(void);
	~SYNetCardManger(void);
	//method
	int TestAdd(int a, int b);
	std::vector<SYNetCardInfo*> *Scan(void);
	bool EnableOrDisableWithGUID(WCHAR *wszGUID, bool enable);
private:
	//method
	SYNetCardManger(void);	


private:
	//var
	//creat
	static SYNetCardManger*				m_instance;		
	//dll
	HMODULE  							m_hModule;

	SYNetCard_TestAdd					m_DLLFuncTestAdd;
	SYNetCard_Scan						m_DLLFuncScan;
	SYNetCard_EnableOrDisableWithGUID	m_DLLFuncEnableOrDisableWithGUID;
};

#endif