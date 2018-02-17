@echo.
@echo   Comando para hacer limpieza del proyecto PBRFAT16.cbp (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@for /D %%i in (bin lib obj) do @(
  @echo   if exist %%i rd /S /Q %%i 
  @if exist %%i rd /S /Q %%i 
)  
@rem ------------------------------------------------------------------------
@for /D %%i in (bpbfat16.res pbrfat16.res stderr.txt stdout.txt) do @(
  @echo   if exist %%i erase /Q %%i  
  @if exist %%i erase /Q %%i    
)
@rem ------------------------------------------------------------------------
@for /D %%i in (asm lst obj) do @(
  @echo   if exist ..\_bpbfat16\bpbfat16.%%i erase /Q ..\_bpbfat16\bpbfat16.%%i  
  @if exist ..\_bpbfat16\bpbfat16.%%i erase /Q ..\_bpbfat16\bpbfat16.%%i    
)
@rem ------------------------------------------------------------------------
@for /D %%i in (asm lst obj) do @(
  @echo   if exist ..\_bpbfat16\bootcode.%%i erase /Q ..\_bpbfat16\bootcode.%%i  
  @if exist ..\_bpbfat16\bootcode.%%i erase /Q ..\_bpbfat16\bootcode.%%i    
)
@rem ------------------------------------------------------------------------
@for /D %%i in (bs exe map) do @(
  @echo   if exist ..\bpbfat16.%%i erase /Q ..\bpbfat16.%%i  
  @if exist ..\bpbfat16.%%i erase /Q ..\bpbfat16.%%i    
)
@rem ------------------------------------------------------------------------
@for /D %%i in (asm bs exe lst map obj) do @(
  @echo   if exist ..\pbrfat16.%%i erase /Q ..\pbrfat16.%%i  
  @if exist ..\pbrfat16.%%i erase /Q ..\pbrfat16.%%i    
)
@rem ------------------------------------------------------------------------
@echo   if exist glide.* erase /Q glide.* 
@if exist glide.* erase /Q glide.*
:fin
@echo.
@pause
