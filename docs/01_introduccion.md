# 1. Introducción

## Propósito
Documentar el firmware de control de vuelo de un cuadricóptero basado en ESP32 y orientar la preparación del entorno, comprensión del código y verificación inicial.

## Alcance observado
Adquisición IMU, estimación de orientación, control de actitud y velocidad angular, generación PWM para cuatro motores, receptor RC, comunicaciones opcionales, barómetro, estimación vertical y funciones failsafe.

## Límites
La presencia de una función en el código no demuestra que esté validada en hardware. El funcionamiento depende de la placa, sensores, cableado, alimentación, versiones del core y configuración.

## Punto de entrada
`CF-Drone-main/CF-Drone.ino` contiene `setup()` y `loop()`. La inicialización configura parámetros y periféricos; el ciclo principal lee sensores y receptor, estima el estado, actualiza protecciones y control, escribe motores y procesa comunicaciones.
