@echo.
@echo   Comando para hacer limpieza del proyecto TIME.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (TIME.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\TIME.%%i erase /Q ..\TIME.%%i  
  @if exist ..\TIME.%%i erase /Q ..\TIME.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\TIME.bin erase /Q ..\..\..\DISQUETE\TIME.bin  
@if exist ..\..\..\DISQUETE\TIME.bin erase /Q ..\..\..\DISQUETE\TIME.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
