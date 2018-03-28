@echo.
@echo   Comando para hacer limpieza del proyecto MEAPILA.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (MEAPILA.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\MEAPILA.%%i erase /Q ..\MEAPILA.%%i  
  @if exist ..\MEAPILA.%%i erase /Q ..\MEAPILA.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\MEAPILA.bin erase /Q ..\..\..\DISQUETE\MEAPILA.bin  
@if exist ..\..\..\DISQUETE\MEAPILA.bin erase /Q ..\..\..\DISQUETE\MEAPILA.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
