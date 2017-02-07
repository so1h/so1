@echo.
@echo   Comando para hacer limpieza del proyecto MOUSE.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (MOUSE.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\MOUSE.%%i erase /Q ..\MOUSE.%%i  
  @if exist ..\MOUSE.%%i erase /Q ..\MOUSE.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\MOUSE.bin erase /Q ..\..\..\DISQUETE\MOUSE.bin  
@if exist ..\..\..\DISQUETE\MOUSE.bin erase /Q ..\..\..\DISQUETE\MOUSE.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
