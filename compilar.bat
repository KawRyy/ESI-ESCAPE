@echo off
echo ==========================================
echo Compilando el proyecto ESI ESCAPE...
echo ==========================================

:: Compilación de todos los módulos del repositorio
gcc main.c condiciones.c ficheros.c mapa.c menus.c usuarios.c gestion.c -o esi_escape.exe

:: Verificación de si la compilación fue exitosa
if %ERRORLEVEL% EQU 0 (
    echo [OK] Compilacion exitosa.
    echo ------------------------------------------
    echo Iniciando el juego...
    esi_escape.exe
) else (
    echo ------------------------------------------
    echo [ERROR] Hubo errores durante la compilacion.
    echo Revisa que GCC este instalado y en el PATH.
)

pause