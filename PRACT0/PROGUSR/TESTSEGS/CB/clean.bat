@echo.
@echo   Comando para hacer limpieza del proyecto TESTSEGS.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (TESTSEGS.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\TESTSEGS.%%i erase /Q ..\TESTSEGS.%%i  
  @if exist ..\TESTSEGS.%%i erase /Q ..\TESTSEGS.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\TESTSEGS.bin erase /Q ..\..\..\DISQUETE\TESTSEGS.bin  
@if exist ..\..\..\DISQUETE\TESTSEGS.bin erase /Q ..\..\..\DISQUETE\TESTSEGS.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
