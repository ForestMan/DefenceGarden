//------------------------------------------------------------------
// Copyright (c) 2006 PlayFirst, Inc.
//
// This material is the confidential trade secret and proprietary
// information of PlayFirst, Inc. It may not be reproduced, used,
// sold or transferred to any third party without the prior written
// consent of PlayFirst, Inc. All rights reserved.
//------------------------------------------------------------------

#include "ExtractGame.h"
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <setupapi.h>

// Link with SetupAPI.Lib.
#pragma comment (lib, "setupapi.lib")

#include <shellapi.h>

//------------------------------------------------------------------
// note that SHFileOperation won't handle read-only files, but
// in this usage, this is never the case
void DeleteDirectory(const char* _path)
{
	char path[MAX_PATH]={0};
	strcpy(path, _path);

	// if there's a trailing backslash, remove it
	int len = strlen(path);
	if (path[len-1] == '\\') {
		path[len-1] = 0;
	}

	SHFILEOPSTRUCT op = {0};
	op.wFunc = FO_DELETE;
	op.pFrom = path;
	op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

	// try really hard to delete the directory, as we don't want to leave it behind and flood
	// the disk with copies of the game.  if the browser just gets closed, the game exited, but the
	// file handle is quite released yet
	DWORD maxWait = 2000;
	DWORD wait = 0;
	DWORD step = 20;
	while (wait < maxWait) {
		DWORD result = SHFileOperation(&op);
		if (result == 0) {
			break;
		}
		Sleep(step);
		wait += step;
	}
}

//------------------------------------------------------------------
void EnsureDirectory(const char* path)
{
	const char* x = path;
	while (true) {
		const char* sep = strchr(x, '\\');
		if (!sep) break;
		x = sep+1;

		char dir[MAX_PATH]={0};
		strcpy(dir, path);
		dir[sep-path+1]=0;

		DWORD attributes = GetFileAttributes(dir);
		if (attributes == 0xFFFFFFFF) {
			CreateDirectory(dir,0);
		}
	}
}

//------------------------------------------------------------------
LRESULT WINAPI CabinetCallback(IN PVOID context, IN UINT event, IN UINT param1, IN UINT param2)
{
	if (event == SPFILENOTIFY_FILEINCABINET) {
		FILE_IN_CABINET_INFO *pInfo = (FILE_IN_CABINET_INFO *)param1;
		strcpy(pInfo->FullTargetName, (char*)context);
		strcat(pInfo->FullTargetName, pInfo->NameInCabinet);
		EnsureDirectory(pInfo->FullTargetName);
		return FILEOP_DOIT;
	} else {
		return NO_ERROR;
	}
}

//------------------------------------------------------------------
TExtractGame::TExtractGame(HMODULE module)
{
	// get a temp directory
	char temp[MAX_PATH];
	if (0 == GetTempPath(MAX_PATH, temp)) {
		return;
	}
	sprintf(mDir, "%spf_$name\\", temp, GetTickCount());

	// make sure temp directory is ready to use
	EnsureDirectory(mDir);

	// pull cab file out of resources
	HRSRC rsrc = FindResource(module, "DATA", "GAME");
	DWORD size = SizeofResource(module, rsrc);
	HGLOBAL global = LoadResource(module, rsrc);
	LPVOID data = LockResource(global);
	char cab[MAX_PATH];
	strcpy(cab, mDir);
	strcat(cab, "__game.cab");
	FILE* fp = fopen(cab, "wb");
	if (fp && data && (size > 0)) {
		fwrite(data, 1, size, fp);
		fclose(fp);
	} 
	UnlockResource(global);

	// extract files from the cab file
	SetupIterateCabinet(cab, 0, (PSP_FILE_CALLBACK)CabinetCallback, (PVOID)mDir);

	// set current directory to be extracted game directory, be prepared to restore
	GetCurrentDirectory(MAX_PATH, mSave);
	SetCurrentDirectory(mDir);
}

TExtractGame::~TExtractGame()
{
	// restore directory so that extract dir can be deleted
	SetCurrentDirectory(mSave); 
	DeleteDirectory(mDir);
}
