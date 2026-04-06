@echo off
REM Script para compilar y ejecutar test_ficheros.c

REM Moverse a la raiz del proyecto para que las rutas a "ficheros/X.txt" funcionen
cd ..

echo [*] Compilando test_ficheros.c...
gcc -Wall -Wextra -o testeo\test_ficheros.exe testeo\test_ficheros.c ficheros.c -I.

if errorlevel 1 (
    echo.
    echo ✗ ERROR en la compilacion de test_ficheros.c
    pause
    exit /b 1
)

echo [v] test_ficheros compilado correctamente. Iniciando...
echo.

testeo\test_ficheros.exe

pause
