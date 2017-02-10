@echo.
@echo   Comando para hacer limpieza de SO1.workspace (Codeblocks)
@echo.
@rem ------------------------------------------------------------------------
@echo   call clean.bat
@call clean.bat
@rem ------------------------------------------------------------------------
@rem ------------------------------------------------------------------------
@for /D %%i in (..\..\PRACT2\BOOT\CB ..\_MBR\CB ..\_PBRFAT16\CB
..\PROGUSR\ATEXIT\CB ..\PROGUSR\AYUDA\CB ..\PROGUSR\C2C\CB ..\PROGUSR\CLOCK\CB
..\PROGUSR\CONRAT\CB ..\PROGUSR\CONSOLA\CB ..\PROGUSR\DATE\CB ..\PROGUSR\DFS\CB 
..\PROGUSR\DMP\CB ..\PROGUSR\DP\CB ..\PROGUSR\DR\CB ..\PROGUSR\EXEC\CB
..\PROGUSR\FORK\CB ..\PROGUSR\HELLO\CB ..\PROGUSR\HELLO2\CB ..\PROGUSR\HELP\CB
..\PROGUSR\INFO\CB ..\PROGUSR\INIC\CB ..\PROGUSR\KILL\CB ..\PROGUSR\LEETIMER\CB
..\PROGUSR\LOGIN\CB ..\PROGUSR\MEAPILA\CB ..\PROGUSR\MEM\CB ..\PROGUSR\MOUSE\CB
..\PROGUSR\PS\CB ..\PROGUSR\RATON\CB ..\PROGUSR\RELOJ\CB ..\PROGUSR\RETARDO\CB
..\PROGUSR\SH\CB ..\PROGUSR\TESTSEGS\CB ..\PROGUSR\TIME\CB ..\PROGUSR\TIMER\CB
..\PROGUSR\VER_ARGS\CB
) do @(
  @echo.
  @echo   pushd %%i
  @pushd %%i
  @if %errorlevel% == 1 @( 
  popd
  goto fin
  )
  @echo   call clean.bat
  @call clean.bat
  popd
)
:fin
@echo.
@pause 
