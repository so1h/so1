@echo.
@echo   Comando para hacer limpieza del proyecto INIC.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj ..\..\INIC\PROGUSR ..\..\INIC\SO1) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (INIC.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\INIC.%%i erase /Q ..\INIC.%%i  
  @if exist ..\INIC.%%i erase /Q ..\INIC.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\INIC.bin erase /Q ..\..\..\DISQUETE\INIC.bin  
@if exist ..\..\..\DISQUETE\INIC.bin erase /Q ..\..\..\DISQUETE\INIC.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
