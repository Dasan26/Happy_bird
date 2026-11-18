# 5. Compilación, carga y ejecución

## Compilación
1. Desconecta hélices y alimentación de potencia de motores.
2. Abre `CF-Drone-main/CF-Drone.ino`.
3. Selecciona el modelo exacto de placa y el puerto serie.
4. Pulsa **Verificar**.
5. Guarda el registro y los errores si aparecen.

## Carga
Solo después de verificar la compilación y manteniendo la etapa de potencia desconectada:
1. Conecta la placa por USB.
2. Pulsa **Subir**.
3. Si la placa requiere modo de descarga, sigue el procedimiento del fabricante.
4. Espera el mensaje de carga completada.

## Monitor serie
Abre el Monitor Serie a **115200 baudios**, valor configurado en `Serial.begin(115200)`. Revisa mensajes de parámetros, IMU, barómetro, receptor y motores. Un fallo de inicialización de IMU impide considerar el sistema listo para armado.

## Objetivo de esta etapa
Comprobar arranque estable, mensajes legibles, detección de IMU y ausencia de calentamiento anormal. La carga correcta no valida el control ni el vuelo.
