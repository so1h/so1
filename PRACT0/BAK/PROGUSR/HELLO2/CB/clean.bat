@echo.
@echo   Comando para hacer limpieza del proyecto HELLO2.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (HELLO2.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\HELLO2.%%i erase /Q ..\HELLO2.%%i  
  @if exist ..\HELLO2.%%i erase /Q ..\HELLO2.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\HELLO2.bin erase /Q ..\..\..\DISQUETE\HELLO2.bin  
@if exist ..\..\..\DISQUETE\HELLO2.bin erase /Q ..\..\..\DISQUETE\HELLO2.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
