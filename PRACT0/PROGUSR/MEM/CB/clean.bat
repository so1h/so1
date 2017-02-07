@echo.
@echo   Comando para hacer limpieza del proyecto MEM.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (MEM.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\MEM.%%i erase /Q ..\MEM.%%i  
  @if exist ..\MEM.%%i erase /Q ..\MEM.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\MEM.bin erase /Q ..\..\..\DISQUETE\MEM.bin  
@if exist ..\..\..\DISQUETE\MEM.bin erase /Q ..\..\..\DISQUETE\MEM.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
