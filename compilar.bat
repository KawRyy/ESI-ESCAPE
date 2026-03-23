@echo off
echo Compilando el proyecto ESI ESCAPE...
gcc main.c ficheros.c juego.c usuarios.c menus.c -o esi_escape.exe
if %ERRORLEVEL% EQU 0 (
    echo Compilacion exitosa. Ejecutando el juego...
    echo ------------------------------------------
    esi_escape.exe
) else (
    echo ------------------------------------------
    echo Hubo errores durante la compilacion.
)
pause
