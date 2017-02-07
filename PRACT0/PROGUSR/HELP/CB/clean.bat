@echo.
@echo   Comando para hacer limpieza del proyecto HELP.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (HELP.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\HELP.%%i erase /Q ..\HELP.%%i  
  @if exist ..\HELP.%%i erase /Q ..\HELP.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\HELP.bin erase /Q ..\..\..\DISQUETE\HELP.bin  
@if exist ..\..\..\DISQUETE\HELP.bin erase /Q ..\..\..\DISQUETE\HELP.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
