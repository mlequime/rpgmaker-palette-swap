:: Change the paths below to the paths you use to store the project and your game folder.
copy /b/v/y "C:\Repos\GitHub\rpgmaker-palette-swap\Debug\PaletteSwap.dll" %UserProfile%\Dropbox\pokemon\Project\PaletteSwap.dll
IF %ERRORLEVEL% NEQ 0 Echo An error was found
IF %ERRORLEVEL% EQU 0 Echo No error found