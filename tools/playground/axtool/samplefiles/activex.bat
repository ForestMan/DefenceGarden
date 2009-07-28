REM variables for creating the ActiveX wrapper

REM the name MUST be less than 12 characters
set NAME=GameName
set EXE=GameExeName.exe

REM you can create these GUIDS in Dev Studio using Tools->Create Guid (registry format option)
REM so an example GUID would look like:
REM GUID0=xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx

set GUID0=<FILL IN GUID 0 HERE>
set GUID1=<FILL IN GUID 1 HERE>
set GUID2=<FILL IN GUID 2 HERE>
set GUID3=<FILL IN GUID 3 HERE--this is the encryption key for makeCertificate.exe>


