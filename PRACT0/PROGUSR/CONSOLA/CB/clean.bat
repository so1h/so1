@echo.
@echo   Comando para hacer limpieza del proyecto CONSOLA.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (CONSOLA.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\CONSOLA.%%i erase /Q ..\CONSOLA.%%i  
  @if exist ..\CONSOLA.%%i erase /Q ..\CONSOLA.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\CONSOLA.bin erase /Q ..\..\..\DISQUETE\CONSOLA.bin  
@if exist ..\..\..\DISQUETE\CONSOLA.bin erase /Q ..\..\..\DISQUETE\CONSOLA.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
