@echo.
@echo   Comando para hacer limpieza del proyecto SO1.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (so1d.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@echo   if exist ..\SO1\glide.* erase /Q ..\SO1\glide.* 
@if exist ..\SO1\glide.* erase /Q ..\SO1\glide.*
@rem ------------------------------------------------------------------------
@echo   if exist ..\SO1\std???.txt erase /Q ..\SO1\std???.txt 
@if exist ..\SO1\std???.txt erase /Q ..\SO1\std???.txt
@rem ------------------------------------------------------------------------
@echo   if exist ..\DISQUETE\so1*.* erase /Q ..\DISQUETE\so1*.* 
@if exist ..\DISQUETE\so1*.* erase /Q ..\DISQUETE\so1*.*
@rem ------------------------------------------------------------------------
:fin
@echo.
@pause
