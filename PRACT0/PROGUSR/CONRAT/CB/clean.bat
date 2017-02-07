@echo.
@echo   Comando para hacer limpieza del proyecto CONRAT.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (CONRAT.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\CONRAT.%%i erase /Q ..\CONRAT.%%i  
  @if exist ..\CONRAT.%%i erase /Q ..\CONRAT.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\CONRAT.bin erase /Q ..\..\..\DISQUETE\CONRAT.bin  
@if exist ..\..\..\DISQUETE\CONRAT.bin erase /Q ..\..\..\DISQUETE\CONRAT.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
