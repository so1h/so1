@echo.
@echo   Comando para hacer limpieza del proyecto C2C.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (C2C.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\C2C.%%i erase /Q ..\C2C.%%i  
  @if exist ..\C2C.%%i erase /Q ..\C2C.%%i    
  @echo   if exist ..\C2CPRG.%%i erase /Q ..\C2CPRG.%%i  
  @if exist ..\C2CPRG.%%i erase /Q ..\C2CPRG.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\C2C.bin erase /Q ..\..\..\DISQUETE\C2C.bin  
@if exist ..\..\..\DISQUETE\C2C.bin erase /Q ..\..\..\DISQUETE\C2C.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
