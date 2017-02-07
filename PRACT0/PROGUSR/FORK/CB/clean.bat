@echo.
@echo   Comando para hacer limpieza del proyecto FORK.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (FORK.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\FORK.%%i erase /Q ..\FORK.%%i  
  @if exist ..\FORK.%%i erase /Q ..\FORK.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\FORK.bin erase /Q ..\..\..\DISQUETE\FORK.bin  
@if exist ..\..\..\DISQUETE\FORK.bin erase /Q ..\..\..\DISQUETE\FORK.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
