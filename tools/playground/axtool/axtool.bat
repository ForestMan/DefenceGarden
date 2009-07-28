pushd %~p0

set A=%1
set B=%2
if not defined HOSTS (
  set HOSTS=test_hosts.txt
)

set OUTPUT=c:\axtool_tmp

set PGSDK=%~p0%..\..

REM establish environment
if defined VSNETPATH (
	call %VSNETPATH%\Common7\Tools\vsvars32.bat
) else (
	call "C:\Program Files\Microsoft Visual Studio 8\Common7\Tools\vsvars32.bat"
)

if not "%2"=="test" (
	pushd C:\depot\tools\drm\bin
	call loadTools.bat
popd
)

REM create fresh output folder
rmdir /s /q %OUTPUT%
mkdir %OUTPUT%

REM MSN ZONE SIZE
set MSNWIDTH=533
set MSNHEIGHT=400

REM clear open version vars to be sure game really sets them
set OPENVERSION=
set OPENVERSIONID=
set OPENVERSIONREFCODE=

if not "%2"=="test" (
	set INPUT=%A%\%B%
)

if not "%2"=="test" (
	REM extract the build into the game folder
	%ZIP% -o%OUTPUT% x %INPUT%
	%DRIVE% sleep 2000
	dir /b /AD %OUTPUT% >%OUTPUT%\dirname.txt
	set /P DIRNAME=<%OUTPUT%\dirname.txt
)

if not "%2"=="test" (
	erase %OUTPUT%\dirname.txt
	move %OUTPUT%\%DIRNAME% %OUTPUT%\game

	REM gather game specific variables
	call %OUTPUT%\game\activex.bat
	erase /F %OUTPUT%\game\activex.bat

	set VERSIONH=%OUTPUT%\game\version.h
) else (
	REM gather game specific variables

	call %1\activex.bat
	call %1\activextest.bat

	REM overwrite the guids with some temporary ones for test mode so that it
	REM doesn't conflict with actual real build GUIDs
	uuidgen -c > %OUTPUT%\tempguid.txt
	set /P GUID0=<%OUTPUT%\tempguid.txt
	uuidgen -c > %OUTPUT%\tempguid.txt
	set /P GUID1=<%OUTPUT%\tempguid.txt
	uuidgen -c > %OUTPUT%\tempguid.txt
	set /P GUID2=<%OUTPUT%\tempguid.txt
	erase %OUTPUT%\tempguid.txt


)

if "%GUID3%"=="" (
    set GUID3=%GUID0%
)


if "%2"=="test" (
	set VERSIONH=%1\%VERSIONH%
	set OVERRIDEDIR=%1\%TESTDIR%
)

printversion.exe %VERSIONH% -comma  > %OUTPUT%\versionstr.txt
set /P VERSION=<%OUTPUT%\versionstr.txt

printversion.exe %VERSIONH%  > %OUTPUT%\versionstr.txt
set /P VERSIONDOT=<%OUTPUT%\versionstr.txt



erase %OUTPUT%\versionstr.txt

if not "%2"=="test" (
	erase /F %OUTPUT%\game\version.h
)

REM unique name to version to avoid upgrade issues
set SHORTNAME=%NAME:~0,10%
set LONGNAME=%NAME%
set NAME=%NAME%.%VERSIONDOT%

if not "%2"=="test" (
	REM move game folder into folder named after the game
	mkdir %OUTPUT%\%NAME%
	move %OUTPUT%\game %OUTPUT%\%NAME%\game
)

REM create certificate
makeCertificate.EXE %GUID3% %HOSTS% > %OUTPUT%\axcert.txt

set /P AXCERT=<%OUTPUT%\axcert.txt
erase %OUTPUT%\axcert.txt

REM build customized ActiveX control with game GUIDS and name

if defined HTMLFILE set PFHTML=%HTMLFILE%
if not defined HTMLFILE set PFHTML=%OUTPUT%\axcontrol\PlayFirstGameControl.htm

xcopy /e /i axTemplate %OUTPUT%\axcontrol
copy /y %PFHTML% %OUTPUT%\%NAME%.html


tokensub.exe %OUTPUT%\%NAME%.html %OUTPUT%\%NAME%_temp.html $guid0 %GUID0%
tokensub.exe %OUTPUT%\%NAME%_temp.html %OUTPUT%\%NAME%.html $name %NAME%
tokensub.exe %OUTPUT%\%NAME%.html %OUTPUT%\%NAME%_temp.html $versionDot %VERSIONDOT%
tokensub.exe %OUTPUT%\%NAME%_temp.html %OUTPUT%\%NAME%.html $version %VERSION%
tokensub.exe %OUTPUT%\%NAME%.html %OUTPUT%\%NAME%_temp.html $axcert %AXCERT%
copy /y %OUTPUT%\%NAME%_temp.html %OUTPUT%\%NAME%.html
erase %OUTPUT%\%NAME%_temp.html

tokensub.exe %OUTPUT%\axcontrol\activex.cpp %OUTPUT%\axcontrol\activex_temp.cpp $guid2 %GUID2%
copy /y %OUTPUT%\axcontrol\activex_temp.cpp %OUTPUT%\axcontrol\activex.cpp
erase %OUTPUT%\axcontrol\activex_temp.cpp

tokensub.exe %OUTPUT%\axcontrol\ExtractGame.cpp %OUTPUT%\axcontrol\ExtractGame_temp.cpp $name %NAME%
copy /y %OUTPUT%\axcontrol\ExtractGame_temp.cpp %OUTPUT%\axcontrol\ExtractGame.cpp
erase %OUTPUT%\axcontrol\ExtractGame_temp.cpp

tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl.h %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h $guid0 %GUID0%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h %OUTPUT%\axcontrol\PlayFirstGameControl.h $guid1 %GUID1%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl.h %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h $exe %EXE%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h %OUTPUT%\axcontrol\PlayFirstGameControl.h $name %SHORTNAME% -checkSubStrings
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl.h %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h $fullname %NAME%





if not "%2"=="test" (
	tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h %OUTPUT%\axcontrol\PlayFirstGameControl.h $dirname %NAME% -checkSubStrings
) else (
	tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h %OUTPUT%\axcontrol\PlayFirstGameControl.h $dirname . -checkSubStrings
)
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl.h %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h $longname %LONGNAME%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h %OUTPUT%\axcontrol\PlayFirstGameControl.h $guid3 %GUID3%

tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl.h %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h $openversion "%OPENVERSION%"
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h %OUTPUT%\axcontrol\PlayFirstGameControl.h $openversionid "%OPENVERSIONID%"
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl.h %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h $openversionrefcode "%OPENVERSIONREFCODE%"
copy /y %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h %OUTPUT%\axcontrol\PlayFirstGameControl.h
erase %OUTPUT%\axcontrol\PlayFirstGameControl_temp.h

tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNConfig.xml %OUTPUT%\axcontrol\PlayFirstMSNConfig_temp.xml $shortName %SHORTNAME% -checkSubStrings
tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNConfig_temp.xml %OUTPUT%\axcontrol\PlayFirstMSNConfig.xml $width %MSNWIDTH%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNConfig.xml %OUTPUT%\axcontrol\PlayFirstMSNConfig_temp.xml $height %MSNHEIGHT%
copy /y %OUTPUT%\axcontrol\PlayFirstMSNConfig_temp.xml %OUTPUT%\axcontrol\PlayFirstMSNConfig.xml
erase %OUTPUT%\axcontrol\PlayFirstMSNConfig_temp.xml


tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNLoader.js %OUTPUT%\axcontrol\PlayFirstMSNLoader_temp.js $guid0 %GUID0%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNLoader_temp.js %OUTPUT%\axcontrol\PlayFirstMSNLoader.js $width %MSNWIDTH%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNLoader.js %OUTPUT%\axcontrol\PlayFirstMSNLoader_temp.js $height %MSNHEIGHT%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNLoader_temp.js %OUTPUT%\axcontrol\PlayFirstMSNLoader.js $axcert %AXCERT%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNLoader.js %OUTPUT%\axcontrol\PlayFirstMSNLoader_temp.js $name %NAME%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstMSNLoader_temp.js %OUTPUT%\axcontrol\PlayFirstMSNLoader.js $version %VERSION%
erase %OUTPUT%\axcontrol\PlayFirstMSNLoader_temp.js



tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl.cpp %OUTPUT%\axcontrol\PlayFirstGameControl_temp.cpp $name %SHORTNAME% -checkSubStrings
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl_temp.cpp %OUTPUT%\axcontrol\PlayFirstGameControl.cpp $depot %DEPOTSED%
tokensub.exe %OUTPUT%\axcontrol\PlayFirstGameControl.cpp %OUTPUT%\axcontrol\PlayFirstGameControl_temp.cpp $sdk %PGSDK%
copy /y %OUTPUT%\axcontrol\PlayFirstGameControl_temp.cpp %OUTPUT%\axcontrol\PlayFirstGameControl.cpp
erase %OUTPUT%\axcontrol\PlayFirstGameControl_temp.cpp

tokensub.exe %OUTPUT%\axcontrol\activex.rc %OUTPUT%\axcontrol\activex_temp.rc $name %NAME%
tokensub.exe %OUTPUT%\axcontrol\activex_temp.rc %OUTPUT%\axcontrol\activex.rc $versionDot %VERSIONDOT%
tokensub.exe %OUTPUT%\axcontrol\activex.rc %OUTPUT%\axcontrol\activex_temp.rc $version %VERSION%
copy /y %OUTPUT%\axcontrol\activex_temp.rc %OUTPUT%\axcontrol\activex.rc
erase %OUTPUT%\axcontrol\activex_temp.rc


tokensub.exe %OUTPUT%\axcontrol\activex.inf %OUTPUT%\axcontrol\activex_temp.inf $guid0 %GUID0%
tokensub.exe %OUTPUT%\axcontrol\activex_temp.inf %OUTPUT%\axcontrol\activex.inf $name %NAME%
tokensub.exe %OUTPUT%\axcontrol\activex.inf %OUTPUT%\axcontrol\activex_temp.inf $version %VERSION%
copy /y %OUTPUT%\axcontrol\activex_temp.inf %OUTPUT%\axcontrol\activex.inf
erase %OUTPUT%\axcontrol\activex_temp.inf
copy /y %OUTPUT%\axcontrol\activex.inf %OUTPUT%\%NAME%\%NAME%.inf

if "%3"=="" devenv /build release /project activex %OUTPUT%\axcontrol\activex.vcproj
if "%3"=="debug" devenv /build debug /project activex %OUTPUT%\axcontrol\activex.vcproj

if not "%2"=="test" (
	REM gather everything together and build/sign the cab file
	copy %OUTPUT%\axcontrol\release\activex.dll %OUTPUT%\%NAME%\%NAME%.dll

	REM sign the exe first

	%DEPOT%\tools\drm\codesigningx86\signcode -spc %DEPOT%\tools\drm\cert\mycredentials.spc -v %DEPOT%\tools\drm\cert\myprivatekey.pvk -n %NAME% -i http://www.playfirst.com -t 	http://timestamp.verisign.com/scripts/timstamp.dll %OUTPUT%\%NAME%\game\%EXE%

	REM create game cab
	pushd %OUTPUT%\%NAME%\game
	cabarc.exe -r -p -s 614 N %OUTPUT%\%NAME%\%NAME%_game.cab *
	popd %OUTPUT%\%NAME%\game

	REM sign game cab
	%DEPOT%\tools\drm\codesigningx86\signcode -spc %DEPOT%\tools\drm\cert\mycredentials.spc -v %DEPOT%\tools\drm\cert\myprivatekey.pvk -n %NAME% -i http://www.playfirst.com -t 	http://timestamp.verisign.com/scripts/timstamp.dll %OUTPUT%\%NAME%\%NAME%_game.cab

	REM remove game directory
	rmdir /s /q %OUTPUT%\%NAME%\game

	REM add game cab as a resource to the activeX dll
	devenv /build release /project AxResource %PGSDK%\utilities\src\AxResource\AxResource.vcproj
	%PGSDK%\utilities\src\AxResource\release\AxResource.exe %OUTPUT%\%NAME%\%NAME%.dll %OUTPUT%\%NAME%\%NAME%_game.cab

	REM remove the game cab
	erase %OUTPUT%\%NAME%\%NAME%_game.cab

	REM sign the activex dll
	%DEPOT%\tools\drm\codesigningx86\signcode -spc %DEPOT%\tools\drm\cert\mycredentials.spc -v %DEPOT%\tools\drm\cert\myprivatekey.pvk -n %NAME% -i http://www.playfirst.com -t 	http://timestamp.verisign.com/scripts/timstamp.dll %OUTPUT%\%NAME%\%NAME%.dll


	pushd %OUTPUT%\%NAME%
	cabarc.exe -r -p -s 614 N %OUTPUT%\%NAME%.cab *
	popd


	%DEPOT%\tools\drm\codesigningx86\signcode -spc %DEPOT%\tools\drm\cert\mycredentials.spc -v %DEPOT%\tools\drm\cert\myprivatekey.pvk -n %NAME% -i http://www.playfirst.com -t 	http://timestamp.verisign.com/scripts/timstamp.dll %OUTPUT%\%NAME%.cab

	move %OUTPUT%\%NAME%.cab %A%\%NAME%.cab
	if "%3"=="debug" copy %OUTPUT%\%NAME%.html \\Intranet.playsecond.com\playfirst\dept\eng\Brad\%NAME%.html
	move %OUTPUT%\%NAME%.html %A%\%NAME%.html

	REM copy over the MSN scripts
	mkdir %A%\msnconfig\msn_%VERSIONDOT%
	copy /y %OUTPUT%\axcontrol\PlayFirstMSNConfig.xml %A%\msnconfig\msn_%VERSIONDOT%\%SHORTNAME%Config.xml
	copy /y %OUTPUT%\axcontrol\PlayFirstMSNLoader.js %A%\msnconfig\msn_%VERSIONDOT%\%SHORTNAME%Loader.js


	if "%3"=="" rmdir /s /q %OUTPUT%
	if "%3"=="debug" move C:\axtool_tmp\%NAME%\game C:\axtool_tmp\axcontrol\Debug\%NAME%
	if "%3"=="debug" regsvr32 /s C:\axtool_tmp\axcontrol\Debug\activex.dll
) else (
	REM put the control in the game directory and register it, put test page there too
	if "%3"=="debug" (
		copy %OUTPUT%\axcontrol\debug\activex.dll %1\%NAME%.dll
	) else (
		copy %OUTPUT%\axcontrol\release\activex.dll %1\%NAME%.dll
	)
	regsvr32 /s %1\%NAME%.dll
	copy %OUTPUT%\%NAME%.html %1\%NAME%.html

	copy %OUTPUT%\axcontrol\PlayFirstMSNConfig.xml C:\Pachinko\GameFiles\%SHORTNAME%Config.xml
	copy %OUTPUT%\axcontrol\PlayFirstMSNLoader.js C:\Pachinko\GameFiles\%SHORTNAME%Loader.js
)

popd
