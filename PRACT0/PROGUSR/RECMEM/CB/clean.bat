@echo.
@echo   Comando para hacer limpieza del proyecto RELOJ.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (RELOJ.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\RELOJ.%%i erase /Q ..\RELOJ.%%i  
  @if exist ..\RELOJ.%%i erase /Q ..\RELOJ.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\RELOJ.bin erase /Q ..\..\..\DISQUETE\RELOJ.bin  
@if exist ..\..\..\DISQUETE\RELOJ.bin erase /Q ..\..\..\DISQUETE\RELOJ.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
