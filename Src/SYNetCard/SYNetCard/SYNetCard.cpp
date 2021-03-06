// SYNetCard.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

bool flag_Debug = !true;

//[Scan]
#include<Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
//---------------------------------------
//[GUID]
#include <Rpc.h>
//---------------------------------------
//[EnableOrDisable]
#include <netcon.h>
//---------------------------------------
//[Var]
#include "SYNetCardDefine.h"
#include <Vector>
std::vector<SYNetCardInfo*> *g_pVecNetCard;



#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	
		g_pVecNetCard = new std::vector<SYNetCardInfo*>;			
		break;

	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		//clear
		while(!g_pVecNetCard->empty())
		{
			delete g_pVecNetCard->back();
			g_pVecNetCard->pop_back();
		}
		delete g_pVecNetCard;
		break;
	}

	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif



//======================================================================================================================
int GetMediaSubTypeWithGUID(WCHAR *wszGUID)
{


	//OLECHAR* bstrGuid;
	//StringFromCLSID(guid, &bstrGuid);

	wchar_t wszSubKey[MAX_PATH];
	wsprintf(wszSubKey, L"SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\%s\\Connection", wszGUID);


	HKEY hKey;
	DWORD dwType = REG_DWORD;
	DWORD dwLen = sizeof(DWORD);
	DWORD dwValue = 0;

	if (RegOpenKey(HKEY_LOCAL_MACHINE, wszSubKey, &hKey) == ERROR_SUCCESS)
	{			
		//cal len
		if (RegQueryValueEx( hKey, L"MediaSubType", NULL, &dwType, (LPBYTE)&dwValue, &dwLen) == ERROR_SUCCESS)
		{			
			//if (RegQueryValueEx(hKey, L"MediaSubType", NULL, &dwType, (LPBYTE)dwValue, &dwLen) == ERROR_SUCCESS)
			//{		
			//	OutputDebugString(L"strValue.c_str()");				
			//}		 
		}	     
		RegCloseKey(hKey);      
	}

	int iResult = SY_NATCARD_UNKNOW;

	switch (dwValue)
	{
	case 1:
		iResult = SY_NATCARD_LAN;
		break;
	case 2:
		iResult = SY_NATCARD_WIRLESS;
		break;
	case 7:
		iResult = SY_NATCARD_BLUETOOTH;
		break;
	default:
		iResult = SY_NATCARD_UNKNOW;
		break;
	}
	return iResult;
}


//======================================================================================================================
int WINAPI TestAdd(int a, int b)
{
	return a+b;
}


//======================================================================================================================
std::vector<SYNetCardInfo*> *WINAPI Scan(void)
{
	//clear
	while(!g_pVecNetCard->empty())
	{
		delete g_pVecNetCard->back();
		g_pVecNetCard->pop_back();
	}



	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	ULONG ulOutBufLen;
	pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	// 第一次调用GetAdapterInfo获取ulOutBufLen大小
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) 
		{

			SYNetCardInfo *newInfo = new SYNetCardInfo();
			g_pVecNetCard->push_back(newInfo);

			//[GUID]
			// ANSI(char) to Unicode(WCHAR)
			char* pAnsi = pAdapter->AdapterName;
			int nIndex = MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, NULL, 0);
			TCHAR *pUnicode = new TCHAR[nIndex + 1];
			MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, pUnicode, nIndex);
			wsprintf(newInfo->wszGUID, L"%s", pUnicode);
			delete pUnicode;
			pAnsi	 = NULL;
			pUnicode = NULL;

			//[Description]
			pAnsi = pAdapter->Description;
			nIndex = MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, NULL, 0);
			pUnicode = new TCHAR[nIndex + 1];
			MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, pUnicode, nIndex);
			wsprintf(newInfo->wszDescription, L"%s", pUnicode);
			delete pUnicode;
			pAnsi	 = NULL;
			pUnicode = NULL;
			
			//[Mac]
			newInfo->MAC_ADDRESS[0] = pAdapter->Address[0];
			newInfo->MAC_ADDRESS[1] = pAdapter->Address[1];
			newInfo->MAC_ADDRESS[2] = pAdapter->Address[2];
			newInfo->MAC_ADDRESS[3] = pAdapter->Address[3];
			newInfo->MAC_ADDRESS[4] = pAdapter->Address[4];
			newInfo->MAC_ADDRESS[5] = pAdapter->Address[5];
			

			
			//[IP Address]
			pAnsi = pAdapter->IpAddressList.IpAddress.String;
			nIndex = MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, NULL, 0);
			pUnicode = new TCHAR[nIndex + 1];
			MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, pUnicode, nIndex);
			wsprintf(newInfo->wszIP_ADDRESS, L"%s", pUnicode);
			delete pUnicode;
			pAnsi	 = NULL;
			pUnicode = NULL;

			//[Mask Address]
			pAnsi = pAdapter->IpAddressList.IpMask.String;
			nIndex = MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, NULL, 0);
			pUnicode = new TCHAR[nIndex + 1];
			MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, pUnicode, nIndex);
			wsprintf(newInfo->wszMASK_ADDRESS, L"%s", pUnicode);
			delete pUnicode;
			pAnsi	 = NULL;
			pUnicode = NULL;

			
			//[Gateway Address]
			pAnsi = pAdapter->GatewayList.IpAddress.String;
			nIndex = MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, NULL, 0);
			pUnicode = new TCHAR[nIndex + 1];
			MultiByteToWideChar(CP_ACP, 0, pAnsi, -1, pUnicode, nIndex);
			wsprintf(newInfo->wszGateWay_ADDRESS, L"%s", pUnicode);
			delete pUnicode;
			pAnsi	 = NULL;
			pUnicode = NULL;


			//[Type]
			newInfo->iType = GetMediaSubTypeWithGUID(newInfo->wszGUID);

			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		if (flag_Debug)
		{
			OutputDebugString(L"Call to GetAdaptersInfo failed.\n");
		}		
	}

	free(pAdapterInfo);
	return g_pVecNetCard;
}

bool WINAPI EnableOrDisableWithGUID(WCHAR *wszGUID, bool enable)
{
	HRESULT hr = E_FAIL;

	CoInitialize(NULL);

	INetConnectionManager *pNetConnectionManager = NULL;
	hr = CoCreateInstance(CLSID_ConnectionManager,
		NULL,
		CLSCTX_LOCAL_SERVER | CLSCTX_NO_CODE_DOWNLOAD,
		IID_INetConnectionManager,
		reinterpret_cast<LPVOID *>(&pNetConnectionManager)
		);
	if (SUCCEEDED(hr))
	{
		/*
		Get an enumurator for the set of connections on the system
		*/
		IEnumNetConnection* pEnumNetConnection;
		pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection);

		ULONG ulCount = 0;
		BOOL fFound = FALSE;

		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

		HRESULT hrT = S_OK;

		/*  
		Enumurate through the list of adapters on the system and look for the one we want
		NOTE: To include per-user RAS connections in the list, you need to set the COM
		Proxy Blanket on all the interfaces. This is not needed for All-user RAS
		connections or LAN connections.
		*/
		do
		{
			NETCON_PROPERTIES* pProps = NULL;
			INetConnection *   pConn;

			/*
			Find the next (or first connection)
			*/
			hrT = pEnumNetConnection->Next(1, &pConn, &ulCount);

			if (SUCCEEDED(hrT) && 1 == ulCount)
			{
				/*
				Get the connection properties
				*/
				hrT = pConn->GetProperties(&pProps);

				if (S_OK == hrT)
				{
					
					OLECHAR* bstrGuid;
					StringFromCLSID(pProps->guidId, &bstrGuid);
					
					WCHAR buf[256];
					wsprintf(buf, L"adapter:%s\n GUID1:%s\n GUID2:%s\n", pProps->pszwDeviceName, bstrGuid, wszGUID);
					if (_wcsicmp(bstrGuid, wszGUID) == 0)
					{
						OutputDebugString(buf);
						if (enable)
						{

							//OutputDebugString(L"Enabling adapter: %S...\n",pProps->pszwName);
							hr = pConn->Connect();
							//break;
						}
						else
						{
							//*printf("      Disabling adapter: %S...\n",pProps->pszwName);
							hr = pConn->Disconnect();
							//break;
						}
					}

					


					CoTaskMemFree (pProps->pszwName);
					CoTaskMemFree (pProps->pszwDeviceName);
					CoTaskMemFree (pProps);
				}

				pConn->Release();
				pConn = NULL;
			}

		} while (SUCCEEDED(hrT) && 1 == ulCount && !fFound);

		if (FAILED(hrT))
		{
			hr = hrT;
		}

		pEnumNetConnection->Release();
	}

	//if (FAILED(hr) && hr != HRESULT_FROM_WIN32(ERROR_RETRY))
	{
		//printf("Could not enable or disable connection (0x%08x)\r\n", hr);
	}

	pNetConnectionManager->Release();

	CoUninitialize();

	if (hr == E_FAIL)
	{
		return false;
	}
	return true;
}

