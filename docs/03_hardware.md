# 3. Hardware y configuración

## Variantes en board_config.h
El firmware incluye configuraciones para ESP32 clásico, ESP32-S3 y ESP32-C3. La placa seleccionada en Arduino IDE debe corresponder al microcontrolador real.

## Pines definidos por el código
Orden de motores: RL, RR, FR, FL. Verifica la tabla con el esquemático de tu placa antes de cablear.

| Señal | ESP32 | ESP32-S3 | ESP32-C3 |
|---|---:|---:|---:|
| Motores | 12,13,15,14 | 4,5,6,7 | 3,10,0,1 |
| ADC batería | 36 | 1 | 2 |
| RC RX | 4 | 8 | 9 |
| SPI SCK/MISO/MOSI/CS | 18/19/23/5 | 12/13/11/10 | 4/5/6/7 |
| I2C SDA/SCL | 21/22 | 35/36 | -1/-1 |
| LED | 2 | 2 | 8 |

## Sensores referenciados
El README original menciona MPU6500/MPU9250. El código también incorpora el barómetro DPS310. Confirma modelo, revisión, alimentación y orientación del sensor de tu montaje.

## Verificaciones
- Confirmar la placa y su revisión.
- Revisar tensión lógica, alimentación y masa común.
- Verificar orientación de IMU y continuidad.
- Revisar pines reservados para flash, PSRAM, USB o arranque.
- Mantener motores desconectados durante la configuración inicial.

Esta tabla representa valores del código, no sustituye el esquemático eléctrico.
