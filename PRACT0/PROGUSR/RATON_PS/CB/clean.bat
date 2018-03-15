@echo.
@echo   Comando para hacer limpieza del proyecto RATON.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (RATON.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\RATON.%%i erase /Q ..\RATON.%%i  
  @if exist ..\RATON.%%i erase /Q ..\RATON.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\RATON.bin erase /Q ..\..\..\DISQUETE\RATON.bin  
@if exist ..\..\..\DISQUETE\RATON.bin erase /Q ..\..\..\DISQUETE\RATON.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
