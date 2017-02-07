@echo.
@echo   Comando para hacer limpieza del proyecto RETARDO.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (RETARDO.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\RETARDO.%%i erase /Q ..\RETARDO.%%i  
  @if exist ..\RETARDO.%%i erase /Q ..\RETARDO.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\RETARDO.bin erase /Q ..\..\..\DISQUETE\RETARDO.bin  
@if exist ..\..\..\DISQUETE\RETARDO.bin erase /Q ..\..\..\DISQUETE\RETARDO.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
