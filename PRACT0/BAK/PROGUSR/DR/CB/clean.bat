@echo.
@echo   Comando para hacer limpieza del proyecto DR.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (DR.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
@rem ------------------------------------------------------------------------
@for /D %%i in (bin exe map obj) do @(
  @echo   if exist ..\DR.%%i erase /Q ..\DR.%%i  
  @if exist ..\DR.%%i erase /Q ..\DR.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist ..\..\..\DISQUETE\DR.bin erase /Q ..\..\..\DISQUETE\DR.bin  
@if exist ..\..\..\DISQUETE\DR.bin erase /Q ..\..\..\DISQUETE\DR.bin   
@rem ------------------------------------------------------------------------ 
:fin
@echo.
@pause
