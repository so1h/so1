@echo.
@echo   Comando para hacer limpieza del proyecto PS.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (PS.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\PS.%%i erase /Q ..\PS.%%i  
  @if exist ..\PS.%%i erase /Q ..\PS.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\PS.bin erase /Q ..\..\..\DISQUETE\PS.bin  
@if exist ..\..\..\DISQUETE\PS.bin erase /Q ..\..\..\DISQUETE\PS.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
