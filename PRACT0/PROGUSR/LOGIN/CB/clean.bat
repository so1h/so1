@echo.
@echo   Comando para hacer limpieza del proyecto LOGIN.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (LOGIN.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\LOGIN.%%i erase /Q ..\LOGIN.%%i  
  @if exist ..\LOGIN.%%i erase /Q ..\LOGIN.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\LOGIN.bin erase /Q ..\..\..\DISQUETE\LOGIN.bin  
@if exist ..\..\..\DISQUETE\LOGIN.bin erase /Q ..\..\..\DISQUETE\LOGIN.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
