@echo.
@echo   Comando para hacer limpieza del proyecto INFO.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (INFO.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\INFO.%%i erase /Q ..\INFO.%%i  
  @if exist ..\INFO.%%i erase /Q ..\INFO.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\INFO.bin erase /Q ..\..\..\DISQUETE\INFO.bin  
@if exist ..\..\..\DISQUETE\INFO.bin erase /Q ..\..\..\DISQUETE\INFO.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
