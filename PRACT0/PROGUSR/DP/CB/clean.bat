@echo.
@echo   Comando para hacer limpieza del proyecto DP.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (DP.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\DP.%%i erase /Q ..\DP.%%i  
  @if exist ..\DP.%%i erase /Q ..\DP.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\DP.bin erase /Q ..\..\..\DISQUETE\DP.bin  
@if exist ..\..\..\DISQUETE\DP.bin erase /Q ..\..\..\DISQUETE\DP.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
