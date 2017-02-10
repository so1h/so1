@echo.
@echo   Comando para hacer limpieza del proyecto BOOT.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (BOOT.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (asm bin bs exe lst map mbr obj) do @(
  @echo   if exist ..\BOOT.%%i erase /Q ..\BOOT.%%i  
  @if exist ..\BOOT.%%i erase /Q ..\BOOT.%%i    
)
:fin
@echo.
@pause

