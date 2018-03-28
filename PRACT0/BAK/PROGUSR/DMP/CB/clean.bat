@echo.
@echo   Comando para hacer limpieza del proyecto DMP.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (DMP.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\DMP.%%i erase /Q ..\DMP.%%i  
  @if exist ..\DMP.%%i erase /Q ..\DMP.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\DMP.bin erase /Q ..\..\..\DISQUETE\DMP.bin  
@if exist ..\..\..\DISQUETE\DMP.bin erase /Q ..\..\..\DISQUETE\DMP.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
