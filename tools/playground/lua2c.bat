rem lua2c  file.lua structName intermediateFile file.out

for %%x in (%0) do set BatchPath=%%~dpsx
for %%x in (%BatchPath%) do set BatchPath=%%~dpsx
cd %BatchPath%

LuaPlusC.exe %5 -o %3 %1 
if errorlevel 1 goto error
bin2c.exe %2 %3 %4
goto done
 
:error
echo Error processing Lua file!
exit -1
:done
