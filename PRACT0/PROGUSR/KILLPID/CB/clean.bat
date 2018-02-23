@echo.
@echo   Comando para hacer limpieza del proyecto KILL.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (KILL.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\KILL.%%i erase /Q ..\KILL.%%i  
  @if exist ..\KILL.%%i erase /Q ..\KILL.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\KILL.bin erase /Q ..\..\..\DISQUETE\KILL.bin  
@if exist ..\..\..\DISQUETE\KILL.bin erase /Q ..\..\..\DISQUETE\KILL.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
