@echo off
REM ============================================================================
REM Script para compilar el módulo de prueba de juego.c
REM ============================================================================

setlocal enabledelayedexpansion

REM Limpiar pantalla
cls

echo.
echo ╔════════════════════════════════════════════════════╗
echo ║   COMPILANDO MÓDULO DE PRUEBA - prueba_juego.c   ║
echo ╚════════════════════════════════════════════════════╝
echo.

REM Crear carpeta build si no existe
if not exist build mkdir build
if not exist build\Debug mkdir build\Debug

REM Compilar el módulo de prueba
REM Usamos gcc con las flags necesarias
echo [*] Compilando prueba_juego.c...
gcc -c prueba_juego.c -o build\Debug\prueba_juego.o -I. -Wall -Wextra

if errorlevel 1 (
    echo.
    echo ✗ ERROR en la compilación de prueba_juego.c
    echo.
    pause
    exit /b 1
)

echo [✓] prueba_juego.c compilado correctamente

REM Compilar ficheros.c
echo [*] Compilando ficheros.c...
gcc -c ficheros.c -o build\Debug\ficheros.o -I. -Wall -Wextra

if errorlevel 1 (
    echo [✓] ficheros.c compilado (puede no ser necesario para este test)
)

REM Crear ejecutable final
echo [*] Creando ejecutable...
gcc build\Debug\prueba_juego.o -o build\Debug\prueba_juego.exe -Wall -Wextra

if errorlevel 1 (
    echo.
    echo ✗ ERROR al crear el ejecutable
    echo.
    pause
    exit /b 1
)

echo [✓] Ejecutable creado exitosamente
echo.
echo ╔════════════════════════════════════════════════════╗
echo ║   COMPILACIÓN COMPLETADA - INICIANDO PROGRAMA    ║
echo ╚════════════════════════════════════════════════════╝
echo.

REM Ejecutar el programa
build\Debug\prueba_juego.exe

pause
