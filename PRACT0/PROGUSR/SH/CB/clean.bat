@echo.
@echo   Comando para hacer limpieza del proyecto SH.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (SH.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\SH.%%i erase /Q ..\SH.%%i  
  @if exist ..\SH.%%i erase /Q ..\SH.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\SH.bin erase /Q ..\..\..\DISQUETE\SH.bin  
@if exist ..\..\..\DISQUETE\SH.bin erase /Q ..\..\..\DISQUETE\SH.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
