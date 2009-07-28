// PlayFirst$nameControl.h : Declaration of the CPlayFirst$nameControl
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include <string.h>
#include "blowfish.h"
#include "base64.h"
#include "ExtractGame.h"

// TODO: all project guids names, need to be unqiue per $name control
#define MAX_MSNSTR 1024

// IPlayFirst$nameControl
[
	object,
	uuid($guid1),
	dual,
	helpstring("IPlayFirst$nameControl Interface"),
	pointer_default(unique)
]
__interface IPlayFirst$nameControl : public IDispatch
{
	[id(1), helpstring("method GameContinue")] HRESULT GameContinue([in] BSTR xml);
	[id(2), helpstring("method GameMenu")] HRESULT GameMenu();
	[id(3), helpstring("method MuteOn")] HRESULT MuteOn();
	[id(4), helpstring("method MuteOff")] HRESULT MuteOff();
	[id(5), helpstring("method PauseOn")] HRESULT PauseOn();
	[id(6), helpstring("method PauseOff")] HRESULT PauseOff();
	[id(7), helpstring("method GameStart")] HRESULT GameStart();
	[id(8), helpstring("method RestartGame")] HRESULT RestartGame();
	[id(9), helpstring("method SessionStart")] HRESULT SessionStart();
	[id(10), helpstring("method LoadGame")] HRESULT LoadGame();
	[id(11), helpstring("method CustomReturn")] HRESULT CustomReturn([in] BSTR xml);
};

class NormalString {
public:
	char* mStr;
	NormalString()
	{
		mStr = new char[1];
		mStr[0] = 0;
	}

	void Set(char* s)
	{
		delete [] mStr;
		mStr = new char[strlen(s)+1];
		strcpy(mStr, s);
	}

	NormalString(BSTR s)
	{
		bool ok = false;

		if (s) {
			size_t len = wcslen(s);

			if (len < 1024*1024) { // cap at 1 MB worth of data
				mStr = new char[len+1];
				if (mStr) {
					ok = true;
					size_t i=0;
					for (i=0; i<len; i++) {
						mStr[i] = (char)s[i];
					}
					mStr[i]= 0;
				}
			}
		}

		if (!ok) {
			mStr = new char[1];
			mStr[0] = 0;
		}
	}

	~NormalString()
	{
		delete [] mStr;
	}
};

// CPlayFirst$nameControl
[
	coclass,
	threading("apartment"),
	vi_progid("activex.PlayFirst$nameControl"),
	progid("activex.PlayFirst$nameControl.1"),
	version(1.0),
	uuid("$guid0"),
	helpstring("PlayFirst$nameControl Class"),
	support_error_info(IPlayFirst$nameControl),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CPlayFirst$nameControl :
	public IPlayFirst$nameControl,
	public IPersistStreamInitImpl<CPlayFirst$nameControl>,
	public IOleControlImpl<CPlayFirst$nameControl>,
	public IOleObjectImpl<CPlayFirst$nameControl>,
	public IOleInPlaceActiveObjectImpl<CPlayFirst$nameControl>,
	public IViewObjectExImpl<CPlayFirst$nameControl>,
	public IOleInPlaceObjectWindowlessImpl<CPlayFirst$nameControl>,
	public IPersistStorageImpl<CPlayFirst$nameControl>,
	public ISpecifyPropertyPagesImpl<CPlayFirst$nameControl>,
	public IQuickActivateImpl<CPlayFirst$nameControl>,
	public IDataObjectImpl<CPlayFirst$nameControl>,
	public IProvideClassInfo2Impl<&__uuidof(CPlayFirst$nameControl), NULL>,
	public CComControl<CPlayFirst$nameControl>,
	public IPersistPropertyBagImpl<CPlayFirst$nameControl>,
	public IObjectSafety
{
public:
	bool IsCertificateValid(NormalString* pServer)
	{
		if (!mCertificate) {
			return false;
		}

		NormalString page(mPageUrl.m_str);
		NormalString encryptedCert64(mCertificate);

		unsigned long encryptedLen = Base64DecodeGetLength(strlen(encryptedCert64.mStr));
		char* encryptedCert = new char[encryptedLen];
		encryptedLen = Base64Decode(encryptedCert, encryptedCert64.mStr, strlen(encryptedCert64.mStr));

		TBlowfish blowfish;
		blowfish.Init((unsigned char*)"$guid3", 36);
		int outLength = 0;
		char* certTemp = blowfish.DecryptString(encryptedCert, encryptedLen, &outLength);
		char* cert = new char[outLength+1];
		memcpy(cert, certTemp, outLength);
		cert[outLength] = 0;
		delete [] certTemp;

		if (strstr(cert, "*AOL*")) {
			mbAOL = true;
		}

		if (FILE* fp = fopen("c:\\pfaxdebug.txt", "r")) {
			fclose(fp);
			return true;
		}

		// certificate is a list of domains or hostnames with a $ after every one, including the last one
		// e.g.
		// games.yahoo.com playfirst.com

		const char* kPre = "http://";
		char* host = strstr(page.mStr, kPre);
		if (!host) {
			const char* kPreSecure = "https://";
			host = strstr(page.mStr, kPreSecure);
			if (!host) {
				return false;
			} else {
				host += strlen(kPreSecure);
			}
		} else {
			host += strlen(kPre);
		}
		char* x = strstr(host, "/");
		if (!x) {
			return false;
		}

		int len = x-host;
		char* match = new char[len+2];
		strncpy(match, host, len);
		match[len]= 0;
		pServer->Set(match);
		match[len]= '$';
		match[len+1]= 0;

		bool ok = false;
		char* s = cert;
		while (true) {
			char* e = strstr(s, "$");
			if (!e) {
				break;
			}

			char save = *(e+1);
			*(e+1) = 0;

			char* q = strstr(match, s);
			if (q) {
				// the match must either be complete, or proceeded by a "."
				// prevents fooplayfirst.com from matching playfirst.com
				if (q == match || *(q-1) == '.') {
					ok = true;
					break;
				}
			}

			*(e+1) = save;
			s = e+1;
		}

		delete [] match;
		delete [] cert;
		delete [] encryptedCert;

		return ok;
	}

    STDMETHOD(SetClientSite)(IOleClientSite *pClientSite)
    {
        if (pClientSite != NULL)
        {
            // Obtain URL from container moniker.
            CComPtr<IMoniker> spmk;
            LPOLESTR pszDisplayName;

            if (SUCCEEDED(pClientSite->GetMoniker(
                                            OLEGETMONIKER_TEMPFORUSER,
                                            OLEWHICHMK_CONTAINER,
                                            &spmk)))
            {
                if (SUCCEEDED(spmk->GetDisplayName(
                                        NULL, NULL, &pszDisplayName)))
                {
                    mPageUrl = pszDisplayName;
                    CoTaskMemFree((LPVOID)pszDisplayName);
                }
            }
		}

	    return IOleObject_SetClientSite(pClientSite);
    }

	STDMETHOD(Load)(IPropertyBag *pPropBag, IErrorLog *pErrorLog)
	{
		USES_CONVERSION;
		VARIANT v={0};
		v.vt = VT_BSTR;
		v.bstrVal = 0;
		if (S_OK == pPropBag->Read(A2W("DownloadUrl"), &v, pErrorLog)) {
			mDownloadUrl = v.bstrVal;
		}
		v.vt = VT_BSTR;
		v.bstrVal = 0;
		if (S_OK == pPropBag->Read(A2W("Options"), &v, pErrorLog)) {
			mOptions = v.bstrVal;
		}
		v.vt = VT_BSTR;
		v.bstrVal = 0;
		if (S_OK == pPropBag->Read(A2W("AnonymousScores"), &v, pErrorLog)) {
			mAnon = v.bstrVal;
		}
		v.vt = VT_BSTR;
		v.bstrVal = 0;
		if (S_OK == pPropBag->Read(A2W("HideLogos"), &v, pErrorLog)) {
			mHideLogo = v.bstrVal;
		}
		v.vt = VT_BSTR;
		v.bstrVal = 0;
		if (S_OK == pPropBag->Read(A2W("Certificate"), &v, pErrorLog)) {
			mCertificate = v.bstrVal;
		}
		v.vt = VT_BSTR;
		v.bstrVal = 0;
		if (S_OK == pPropBag->Read(A2W("MSN"), &v, pErrorLog)) {
			mMSN = v.bstrVal;
		}
		v.vt = VT_BSTR;
		v.bstrVal = 0;
		if (S_OK == pPropBag->Read(A2W("GameData"), &v, pErrorLog)) {
			mGameData = v.bstrVal;
		}

		return 0;
	}

	STDMETHOD (GetInterfaceSafetyOptions)(const IID &,DWORD *,DWORD *)
	{
		return 0;
	}

	STDMETHOD( SetInterfaceSafetyOptions)(const IID &,DWORD,DWORD)
	{
		return 0;
	}

	CPlayFirst$nameControl()
	{
		m_bWindowOnly = TRUE;
		mInit = FALSE;
		mDone = FALSE;
		mbDidRun = FALSE;
		mTestMode = FALSE;
		mbAOL = false;
		mMsnStr[0] = 0;
		mMsnStrLen = 0;
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CPlayFirst$nameControl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	PROP_DATA_ENTRY("DownloadUrl", mDownloadUrl, VT_BSTR)
	PROP_DATA_ENTRY("Options", mOptions, VT_BSTR)
	PROP_DATA_ENTRY("Certificate", mCertificate, VT_BSTR)
	PROP_DATA_ENTRY("MSN", mMSN, VT_BSTR)
	PROP_DATA_ENTRY("AnonymousScores", mAnon, VT_BSTR)
	PROP_DATA_ENTRY("HideLogos", mHideLogo, VT_BSTR)
	PROP_DATA_ENTRY("GameData", mGameData, VT_BSTR)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CPlayFirst$nameControl)
	CHAIN_MSG_MAP(CComControl<CPlayFirst$nameControl>)
	MESSAGE_HANDLER(WM_USER, OnUserCommand)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPlayFirst$nameControl
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
		RECT& rc = *(RECT*)di.prcBounds;
		// Set Clip region to the rectangle specified by di.prcBounds
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("Loading $longname...");
		char msg[256];
		if (mTestMode) {
			sprintf(msg, "Run with -wnd=%x", m_hWndCD);
			pszText = msg;
		} else if (mbDidRun) {
			pszText = _T("");
		} else if (mDone) {
			pszText = _T("Sorry, the game cannot run.");
		}

		TextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			pszText,
			lstrlen(pszText));

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		// in AOL shutdown order, game already quit and PostMessage crashes on bad window
		HWND child = ::GetWindow(m_hWndCD, GW_CHILD);
		if (::IsWindow(child)) {
			::PostMessage(child, WM_QUIT, 0, 0);
		}

		if (!mTestMode) {
			while (!mDone) {
				Sleep(10);
			}
		}

#ifdef _DEBUG
		DeleteFile("c:\\axtestharness.txt");
#endif
	}

	static DWORD WINAPI ThreadProc(void* param)
	{
		CPlayFirst$nameControl* self = (CPlayFirst$nameControl*)param;

		// run before we work on the cmd line args so that the *AOL* mode is parsed
		NormalString server;
		bool certValid = self->IsCertificateValid(&server);

		PROCESS_INFORMATION info;

		int cmdLineLen = strlen("-wnd=01234567 -url=");

		NormalString url(self->mDownloadUrl);
		if (!certValid && !strcmp("$openversion", "true")) {
			// if the cert is invalid, but this was compiled as an "open version"
			// then we put in our own download url with this site encoded in the ref code
			// and allow it to play anyway
			certValid = true;

			const char* id = "$openversionid";
			const char* refcode = "$openversionrefcode";
			const char* buyurl_part1 = "http://www.playfirst.com/r/";
			const char* buyurl_part2 = "/game/";
			const char* buyurl_part3 = "/buy";

			int len = strlen(buyurl_part1) + strlen(refcode) + strlen(server.mStr) + strlen(buyurl_part2) + strlen(id) + strlen(buyurl_part3) + 1;
			char* openDownloadUrl = new char[len];
			sprintf(openDownloadUrl, "%s%s%s%s%s%s", buyurl_part1, refcode, server.mStr, buyurl_part2, id, buyurl_part3);
			url.Set(openDownloadUrl);
			delete [] openDownloadUrl;
		}
		if (strlen(url.mStr)) {
			cmdLineLen += strlen(url.mStr);
		}

		NormalString options(self->mOptions);
		if (strlen(options.mStr)) {
			cmdLineLen += strlen(" -options=");
			cmdLineLen += strlen(options.mStr);
		}

		const char* kAnonParam = " -anon ";
		bool bAnon = false;
		NormalString anon(self->mAnon);
		if (strlen(anon.mStr)) {
			bAnon = true;
			cmdLineLen += strlen(kAnonParam);
			cmdLineLen += strlen(anon.mStr);
		}

		const char* kHideLogoParam = " -hidelogo ";
		bool bHideLogo = false;
		NormalString hidelogo(self->mHideLogo);
		if (strlen(hidelogo.mStr)) {
			bHideLogo = true;
			cmdLineLen += strlen(kHideLogoParam);
			cmdLineLen += strlen(hidelogo.mStr);
		}

		bool bGameData = false;
		NormalString gameData(self->mGameData);
		if (strlen(gameData.mStr)) {
			bGameData = true;
			cmdLineLen += strlen(gameData.mStr) + 1;
		}

		bool bMSN = false;
		const char* kMSN = " -msn";
		NormalString msn(self->mMSN);
		if (strlen(msn.mStr)) {
			bMSN = true;
			cmdLineLen += strlen(kMSN);
		}

		const char* kHost = " -host=";

		char host[MAX_PATH] = {0};
		char hostArg[MAX_PATH] = {0};
		if (!bMSN) { // MSN mode is always iexplore - don't pass in Pachinko, let game default to iexplore
			::GetModuleFileName(NULL, hostArg, MAX_PATH);
			::GetShortPathName(hostArg, host, MAX_PATH);
			strcpy(hostArg, kHost);
			strcat(hostArg, host);
			cmdLineLen += (int)strlen(hostArg);
		}

		const char* kAOL = " -aol";
		if (self->mbAOL) {
			cmdLineLen += (int)strlen(kAOL);
		}

		char* cmdLine = new char[cmdLineLen+1];
		sprintf(cmdLine, "-wnd=%x", self->m_hWndCD);

		if (strlen(url.mStr)) {
			strcat(cmdLine, " -url=");
			strcat(cmdLine, url.mStr);
		}

		if (strlen(options.mStr))
		{
			strcat(cmdLine, " -options=");
			strcat(cmdLine, options.mStr);
		}

		if (bAnon) {
			strcat(cmdLine, kAnonParam);
			strcat(cmdLine, anon.mStr);
		}

		if (bHideLogo) {
			strcat(cmdLine, kHideLogoParam);
			strcat(cmdLine, hidelogo.mStr);
		}

		if (bGameData) {
			strcat(cmdLine, " ");
			strcat(cmdLine, gameData.mStr);
		}

		if (bMSN) {
			strcat(cmdLine, kMSN);
		}

		strcat(cmdLine, hostArg);

		if (self->mbAOL) {
			strcat(cmdLine, kAOL);
		}

		STARTUPINFO s={0};

#ifdef _DEBUG
		{
			HINSTANCE i = LoadLibrary("$fullname.dll");
			if (!i) {
				i = LoadLibrary("activex.dll");
			}
			if (i) {
				char dir[MAX_PATH]={0};
				GetModuleFileName(i, dir, MAX_PATH);
				FreeLibrary(i);

				char* x = strrchr(dir, '\\');
				if (x) {
					*(x+1) = 0;
					strcat(dir, "$dirname");
					SetCurrentDirectory(dir);
					if (FILE* fp = fopen("c:\\pfaxdebug2.txt", "r")) {
						fclose(fp);
						::MessageBox(NULL, dir, "dir", MB_OK);
					}
				}
			}
		}
#endif

		const char* exe = "$exe";

		int size = strlen(exe)+strlen(cmdLine)+3;
		char* cmd = new char[size];
		sprintf(cmd, "%s %s", exe, cmdLine);

		delete [] cmdLine;

		bool bLaunch = true;
#ifdef _DEBUG
		if (FILE* fp = fopen("c:\\pfaxdebug.txt", "r")) {
			fclose(fp);
			if (!bMSN) {
				self->mTestMode = TRUE;
				self->Invalidate();
				bLaunch = false;
			}
		}
#endif
		if (bLaunch)
		{
			if (certValid) {
#ifdef _DEBUG
				if (FILE* fp = fopen("c:\\pfaxdebug2.txt", "r")) {
					fclose(fp);
					::MessageBox(NULL, cmd, "cmd", MB_OK);
				}
#endif

#ifndef _DEBUG
				HINSTANCE i = LoadLibrary("$fullname.dll");
				TExtractGame extract(i);
				FreeLibrary(i);
#endif

				// be sure window is still valid - could've gotten destroyed while we were unpacking the game
				if (::IsWindow(self->m_hWndCD)) {
					// Get the Windows version.
					DWORD dwVersion = GetVersion();
					DWORD dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
					DWORD dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
					if (dwWindowsMajorVersion >= 6) {
						// Vista
						self->mbDidRun = CreateProcessAsUser(NULL, 0, cmd, 0,0,0,0,0,0, &s, &info);
					} else {
						self->mbDidRun = CreateProcess(0, cmd, 0,0,0,0,0,0, &s, &info);
					}
				}

				if (::IsWindow(self->m_hWndCD)) {
					self->Invalidate();
				}

				while (true) {
					Sleep(100);
					DWORD code=0;
					BOOL ok = GetExitCodeProcess(info.hProcess, &code);
					bool bRunning = (ok && code == STILL_ACTIVE);
					if (!bRunning) {
						break;
					}
				}
			}
			delete [] cmd;

			if (::IsWindow(self->m_hWndCD)) {
				self->Invalidate();
			}

			self->mDone = true;
		}

		return 0;
	}

	virtual HWND CreateControlWindow(HWND hWndParent, RECT& rcPos)
	{
		HWND hwnd = CComControl<CPlayFirst$nameControl>::CreateControlWindow(hWndParent, rcPos);

#ifdef _DEBUG
		FILE* fp = fopen("c:\\axtestharness.txt", "w");
		if (fp) {
			fprintf(fp, "%x\n", m_hWndCD);
			fclose(fp);
		}
#endif

		if (!mInit) {
			mInit = true;
			DWORD id=0;
			HANDLE t = CreateThread(NULL, 0, ThreadProc, this, 0, &id);
		}

		return hwnd;
	}

	LRESULT OnUserCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&bHandled);
	void SendToGame(unsigned long msg, unsigned long value);
	void CallScript(const char* _script);
	bool MsnStrAppend(unsigned long x);

	STDMETHOD(GameContinue)(BSTR xml);
	STDMETHOD(GameMenu)();
	STDMETHOD(MuteOn)();
	STDMETHOD(MuteOff)();
	STDMETHOD(PauseOn)();
	STDMETHOD(PauseOff)();
	STDMETHOD(GameStart)();
	STDMETHOD(RestartGame)();
	STDMETHOD(SessionStart)();
	STDMETHOD(LoadGame)();
	STDMETHOD(CustomReturn)(BSTR xml);

	bool mInit;
	bool mDone;
	bool mbDidRun;
	bool mbAOL;
	bool mTestMode;
	CComBSTR mDownloadUrl;
	CComBSTR mOptions;
	CComBSTR mCertificate;
	CComBSTR mMSN;
	CComBSTR mAnon;
	CComBSTR mHideLogo;
	CComBSTR mGameData;
    CComBSTR mPageUrl;
	char mMsnStr[MAX_MSNSTR];
	int mMsnStrLen;
};

