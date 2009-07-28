// PlayFirstGameControl.cpp : Implementation of CPlayFirstGameControl
#include "stdafx.h"
#include "PlayFirstGameControl.h"
#include "ExtractGame.cpp"

// CPlayFirstGameControl

// FIX ME - need to migrate ActiveX Host code to be part of the SDK
// this choice was made over having two copies of the constants
// this could fail to compile, but will never mismatch the constants
#include "$sdk\include\pf\msnzonemessage.h"

void CPlayFirst$nameControl::CallScript(const char* _script)
{
	USES_CONVERSION;
	BSTR script = A2BSTR(_script);
	CComPtr<IOleContainer> spContainer; 
	m_spClientSite->GetContainer(&spContainer); 
	if (spContainer) {
		CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> spDoc(spContainer); 
		if (spDoc) {
			IHTMLWindow2* window= NULL;
			spDoc->get_parentWindow(&window);
			if (window) {
				VARIANT v = {0};
				window->execScript(script, NULL, &v);
				window->Release();
			}
		}
	}
}

void CPlayFirst$nameControl::SendToGame(unsigned long msg, unsigned long value)
{
	::PostMessage(::GetWindow(m_hWndCD, GW_CHILD), WM_USER, msg, value);
}

STDMETHODIMP CPlayFirst$nameControl::GameContinue(BSTR xml)
{
	SendToGame(MSNZONE_GAMECONTINUE, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::GameMenu()
{
	SendToGame(MSNZONE_GAMEMENU, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::MuteOn()
{
	SendToGame(MSNZONE_MUTEON, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::MuteOff()
{
	SendToGame(MSNZONE_MUTEOFF, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::PauseOn()
{
	SendToGame(MSNZONE_PAUSEON, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::PauseOff()
{
	SendToGame(MSNZONE_PAUSEOFF, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::GameStart()
{
	SendToGame(MSNZONE_GAMESTART, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::RestartGame()
{
	SendToGame(MSNZONE_RESTARTGAME, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::SessionStart()
{
	SendToGame(MSNZONE_SESSIONSTART, 0);
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::LoadGame()
{
	return S_OK;
}

STDMETHODIMP CPlayFirst$nameControl::CustomReturn(BSTR xml)
{
	SendToGame(MSNZONE_CUSTOMRETURN, 0);
	return S_OK;
}

bool CPlayFirst$nameControl::MsnStrAppend(unsigned long x)
{
	if (mMsnStrLen == MAX_MSNSTR) return false;

	bool result = false;
	char* x2 = (char*)(&x);
	for (int i=0; i<4; i++) {
		mMsnStr[mMsnStrLen++] = x2[i];
		if (mMsnStr[mMsnStrLen-1] == 0) {
			result = true;
			break;
		}
	}
	return result;
}

LRESULT CPlayFirst$nameControl::OnUserCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&bHandled)
{
	// on Windows Vista, other messages seem to arrive here.
	if (uMsg != WM_USER) {
		return 0;
	}

	bool bComplete = false;
	bComplete = MsnStrAppend((DWORD)wParam);
	if (!bComplete) {
		bComplete = MsnStrAppend((DWORD)lParam);
	}
	if (!bComplete) {
		return TRUE;
	}

	CallScript(mMsnStr);
	mMsnStrLen = 0;
	mMsnStr[0] = 0;

	return TRUE;
}


