@echo.
@echo   Comando para hacer limpieza del proyecto DFS.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (DFS.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\DFS.%%i erase /Q ..\DFS.%%i  
  @if exist ..\DFS.%%i erase /Q ..\DFS.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\DFS.bin erase /Q ..\..\..\DISQUETE\DFS.bin  
@if exist ..\..\..\DISQUETE\DFS.bin erase /Q ..\..\..\DISQUETE\DFS.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
