pfaddin.dll is a plug-in extension for Visual Studio that improves its ability
to watch Playground str variables.

To install pfaddin to Visual Studio, you need to add the following line to autoexp.dat under the [AutoExpand] section of the file:

str=$ADDIN(pfaddin.dll,_AddIn_pfstr@28)

And copy pfaddin.dll into the same folder as devenv.exe, or into your path.

In Devstudio.NET, autoexp.dat lives in:

  {visual studio install folder}\Common7\Packages\Debugger\autoexp.dat

And devenv.exe lives in:

  {visual studio install folder}\Common7\IDE

YMMV.  --Tim