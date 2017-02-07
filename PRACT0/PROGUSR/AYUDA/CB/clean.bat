@echo.
@echo   Comando para hacer limpieza del proyecto AYUDA.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (AYUDA.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\AYUDA.%%i erase /Q ..\AYUDA.%%i  
  @if exist ..\AYUDA.%%i erase /Q ..\AYUDA.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\AYUDA.bin erase /Q ..\..\..\DISQUETE\AYUDA.bin  
@if exist ..\..\..\DISQUETE\AYUDA.bin erase /Q ..\..\..\DISQUETE\AYUDA.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
