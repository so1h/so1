@echo.
@echo   Comando para hacer limpieza del proyecto DATE.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (DATE.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\DATE.%%i erase /Q ..\DATE.%%i  
  @if exist ..\DATE.%%i erase /Q ..\DATE.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\DATE.bin erase /Q ..\..\..\DISQUETE\DATE.bin  
@if exist ..\..\..\DISQUETE\DATE.bin erase /Q ..\..\..\DISQUETE\DATE.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
