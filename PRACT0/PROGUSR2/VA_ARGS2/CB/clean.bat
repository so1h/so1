@echo.
@echo   Comando para hacer limpieza del proyecto VER_ARGS.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (VER_ARGS.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\VER_ARGS.%%i erase /Q ..\VER_ARGS.%%i  
  @if exist ..\VER_ARGS.%%i erase /Q ..\VER_ARGS.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\VER_ARGS.bin erase /Q ..\..\..\DISQUETE\VER_ARGS.bin  
@if exist ..\..\..\DISQUETE\VER_ARGS.bin erase /Q ..\..\..\DISQUETE\VER_ARGS.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
