# 4. Instalación del entorno

## Software
1. Instala Arduino IDE 2.3.x o posterior.
2. En Preferencias, configura la URL oficial del gestor de tarjetas de Espressif si aún no está añadida.
3. Abre el Gestor de tarjetas e instala **esp32 by Espressif Systems**.
4. Consulta la guía oficial: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html

## Descargar
```bash
git clone https://github.com/Dasan26/Happy_bird.git
cd Happy_bird
```

También puedes descargar el ZIP desde GitHub.

## Abrir la sketch
Abre `CF-Drone-main/CF-Drone.ino`. Mantén los archivos `.ino` juntos en la carpeta de sketch.

## Dependencias
El proyecto utiliza APIs del core ESP32 y drivers locales en `CF-Drone-main/drivers/`. Compila y registra el primer error real antes de instalar bibliotecas adicionales. La lista exacta de dependencias y versiones debe documentarse después de una compilación limpia.

## Registro reproducible
Anota versión de Arduino IDE, core ESP32, placa seleccionada, bibliotecas externas y sus versiones, y salida completa de compilación.
