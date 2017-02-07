@echo.
@echo   Comando para hacer limpieza del proyecto LEETIMER.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (LEETIMER.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\LEETIMER.%%i erase /Q ..\LEETIMER.%%i  
  @if exist ..\LEETIMER.%%i erase /Q ..\LEETIMER.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\LEETIMER.bin erase /Q ..\..\..\DISQUETE\LEETIMER.bin  
@if exist ..\..\..\DISQUETE\LEETIMER.bin erase /Q ..\..\..\DISQUETE\LEETIMER.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
