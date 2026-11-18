# Happy_bird | Firmware de control de vuelo para cuadricóptero

Firmware basado en ESP32 con módulos de adquisición de sensores, estimación de actitud, control, salidas de motor, comunicaciones y seguridad.

> **Seguridad:** la documentación no certifica el firmware para vuelo. Las primeras pruebas deben realizarse sin hélices y con la etapa de potencia desconectada. No armes el dron hasta validar hardware, alimentación, calibración, motores y failsafes.

## Funciones identificadas en el código
- IMU por SPI: lectura, calibración y transformación de ejes.
- Estimación de actitud con cuaterniones y corrección del acelerómetro.
- Control en cascada: actitud y velocidad angular mediante PID.
- Mezcla de cuatro motores y generación PWM.
- Receptor RC (SBUS/CRSF según configuración).
- Wi-Fi, control web y MAVLink en placas compatibles.
- Barómetro DPS310 y estimación vertical EKF.
- Protecciones por pérdida de RC, batería e inversión.

## Arquitectura
El firmware Arduino se encuentra en [CF-Drone-main](CF-Drone-main/CF-Drone.ino). Los archivos `.ino` se mantienen juntos para conservar compatibilidad con Arduino IDE; los drivers locales están en `CF-Drone-main/drivers/`.

| Subsistema | Archivos |
|---|---|
| Aplicación | `CF-Drone.ino` |
| Control | `control.ino`, `control.h`, `pid.h` |
| Sensores | `imu.ino`, `barometer.ino`, `battery.ino` |
| Estimación | `estimate.ino`, `position_ekf.ino`, `quaternion.h`, `vector.h` |
| Motores | `motors.ino` |
| Comunicaciones | `rc.ino`, `wifi.ino`, `web_rc.ino`, `mavlink.ino` |
| Seguridad | `safety.ino` |
| Configuración | `board_config.h`, `parameters.ino` |
| Drivers | `drivers/` |

## Hardware
El código contempla ESP32 clásico, ESP32-S3 y ESP32-C3. La asignación de pines se define en `CF-Drone-main/board_config.h`. Comprueba siempre la variante exacta de placa y su esquemático; no conectes usando una tabla genérica. El README original menciona sensores MPU6500/MPU9250 y DPS310; confirma las referencias físicas de tu montaje.

## Requisitos
- Arduino IDE 2.3.x o posterior.
- Paquete de placas **esp32 by Espressif Systems**.
- Cable USB de datos y controlador correspondiente.
- Placa y periféricos compatibles con `board_config.h`.

## Instalación y ejecución
1. Clona o descarga el repositorio:
   ```bash
   git clone https://github.com/Dasan26/Happy_bird.git
   cd Happy_bird
   ```
2. En Arduino IDE abre `CF-Drone-main/CF-Drone.ino`.
3. Instala **esp32 by Espressif Systems** desde el Gestor de tarjetas.
4. Selecciona la placa real y el puerto serie.
5. Revisa la configuración de pines en `CF-Drone-main/board_config.h`.
6. Con motores y hélices desconectados, pulsa **Verificar**.
7. Cuando compile, carga el firmware con la etapa de potencia desconectada.
8. Abre el Monitor Serie a **115200 baudios** y revisa los mensajes de inicio y el estado de la IMU.

Las dependencias externas y sus versiones deben fijarse tras realizar una compilación limpia. No se debe asumir compatibilidad de cualquier versión de biblioteca.

## Documentación
- [Introducción](docs/01_introduccion.md)
- [Arquitectura](docs/02_arquitectura.md)
- [Hardware](docs/03_hardware.md)
- [Instalación](docs/04_instalacion.md)
- [Ejecución](docs/05_ejecucion.md)
- [Plan de pruebas y seguridad](docs/06_pruebas.md)

## Estado
La estructura y algunos módulos se han inspeccionado; compilación reproducible, pruebas en hardware y validación de vuelo están pendientes. Una compilación exitosa no demuestra que el dron sea seguro para volar.

## Licencia y atribución
Consulta `CF-Drone-main/LICENSE` antes de reutilizar o redistribuir el firmware. Conserva avisos de autoría y condiciones de la licencia original.
