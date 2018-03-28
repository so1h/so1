@echo.
@echo   Comando para hacer limpieza del proyecto CLOCK.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (CLOCK.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\CLOCK.%%i erase /Q ..\CLOCK.%%i  
  @if exist ..\CLOCK.%%i erase /Q ..\CLOCK.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\CLOCK.bin erase /Q ..\..\..\DISQUETE\CLOCK.bin  
@if exist ..\..\..\DISQUETE\CLOCK.bin erase /Q ..\..\..\DISQUETE\CLOCK.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
