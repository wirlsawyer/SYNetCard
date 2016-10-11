#ifndef _SYNETCARDDEFINE_H_
#define _SYNETCARDDEFINE_H_

enum 
{
	SY_NATCARD_UNKNOW = 0,
	SY_NATCARD_LAN,
	SY_NATCARD_WIRLESS,
	SY_NATCARD_BLUETOOTH	
};

class SYNetCardInfo
{
public:

	SYNetCardInfo()
	{
		memset(wszGUID, 0, sizeof(wszGUID));
		memset(wszDescription, 0, sizeof(wszDescription));

		MAC_ADDRESS[0] = -1;
		MAC_ADDRESS[1] = -1;
		MAC_ADDRESS[2] = -1;
		MAC_ADDRESS[3] = -1;
		MAC_ADDRESS[4] = -1;
		MAC_ADDRESS[5] = -1;

		memset(wszIP_ADDRESS, 0, sizeof(wszIP_ADDRESS));
		memset(wszMASK_ADDRESS, 0, sizeof(wszMASK_ADDRESS));
		memset(wszGateWay_ADDRESS, 0, sizeof(wszGateWay_ADDRESS));

		iType = SY_NATCARD_UNKNOW;
		/*
		
		
		uLinkQuality   = -1;
		uSignalQuality = -1;

		bCurrentConnected = false;
		*/
	}

	WCHAR wszGUID[260];
	WCHAR wszDescription[260];
	int	  MAC_ADDRESS[6];
	WCHAR wszIP_ADDRESS[4 * 4];
	WCHAR wszMASK_ADDRESS[4 * 4];
	WCHAR wszGateWay_ADDRESS[4 * 4];
	int	  iType;

	void Print(void)	
	{
		WCHAR wszBuf[256];
		memset(wszBuf, 0, sizeof(wszBuf));
		wsprintf(wszBuf, L"GUID:%s\n", wszGUID);
		OutputDebugString(wszBuf);

		memset(wszBuf, 0, sizeof(wszBuf));
		wsprintf(wszBuf, L"Description=%s\n", wszDescription);
		OutputDebugString(wszBuf);

		memset(wszBuf, 0, sizeof(wszBuf));
		wsprintf(wszBuf, L"MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", MAC_ADDRESS[0], MAC_ADDRESS[1], MAC_ADDRESS[2], MAC_ADDRESS[3], MAC_ADDRESS[4], MAC_ADDRESS[5]);
		OutputDebugString(wszBuf);

		memset(wszBuf, 0, sizeof(wszBuf));
		wsprintf(wszBuf, L"IP:%s\n", wszIP_ADDRESS);
		OutputDebugString(wszBuf);

		memset(wszBuf, 0, sizeof(wszBuf));
		wsprintf(wszBuf, L"Mask:%s\n", wszMASK_ADDRESS);
		OutputDebugString(wszBuf);

		memset(wszBuf, 0, sizeof(wszBuf));
		wsprintf(wszBuf, L"GetWary:%s\n", wszGateWay_ADDRESS);
		OutputDebugString(wszBuf);


		switch (iType)
		{
		case SY_NATCARD_LAN:
			OutputDebugString(L"Type:LAN\n");
			break;
		case SY_NATCARD_WIRLESS:
			OutputDebugString(L"Type:WIRLESS\n");
			break;
		case SY_NATCARD_BLUETOOTH:
			OutputDebugString(L"Type:BLUETOOTH\n");
			break;
		default:
			OutputDebugString(L"Type:UNKNOW\n");
			break;
		}

		
	}
	
};

#endif