@echo.
@echo   Comando para hacer limpieza del proyecto EXEC.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (EXEC.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\EXEC.%%i erase /Q ..\EXEC.%%i  
  @if exist ..\EXEC.%%i erase /Q ..\EXEC.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\EXEC.bin erase /Q ..\..\..\DISQUETE\EXEC.bin  
@if exist ..\..\..\DISQUETE\EXEC.bin erase /Q ..\..\..\DISQUETE\EXEC.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
