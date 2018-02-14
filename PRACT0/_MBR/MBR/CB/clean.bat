@echo.
@echo   Comando para hacer limpieza del proyecto MBR.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (tpart.res mbr.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@for /D %%i in (..\tpart.map ..\tpart.tp) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (asm bin exe lst map mbr obj) do @(
  @echo   if exist ..\mbr.%%i erase /Q ..\mbr.%%i  
  @if exist ..\mbr.%%i erase /Q ..\mbr.%%i    
)
:fin
@echo.
@pause
