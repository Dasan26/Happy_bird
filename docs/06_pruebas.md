# 6. Plan de pruebas y seguridad

| ID | Prueba | Condición | Evidencia |
|---|---|---|---|
| T01 | Compilación | Placa seleccionada | Registro del IDE |
| T02 | Arranque | Potencia de motores desconectada | Monitor serie |
| T03 | Detección IMU | Equipo inmóvil | Modelo, estado y datos |
| T04 | Orientación IMU | Movimiento manual, sin hélices | Signo y ejes |
| T05 | Barómetro | Sensor conectado | Lecturas estables |
| T06 | RC | Motores desconectados | Canales y pérdida de señal |
| T07 | PWM | Motores físicamente desconectados | Frecuencia y duty |
| T08 | Failsafe | Simulación controlada, sin hélices | Registro de estado |
| T09 | Batería | Fuente de laboratorio adecuada | Medición y umbrales |

Registra estado Pendiente/Aprobada/Fallida, fecha, placa, versión y evidencia. No marques una prueba aprobada sin evidencia reproducible.

## Seguridad
- Retira las hélices durante las pruebas iniciales.
- No confíes solo en software para detener motores: desconecta físicamente la potencia.
- Usa fuente con límite de corriente en las primeras comprobaciones.
- Verifica asignación y sentido de giro sin hélices.
- Comprueba pérdida de RC y batería baja sin riesgo para personas.
- No realices vuelos hasta validar estructura, centro de gravedad, alimentación, calibración, motores, control y failsafes.
- Mantén personas fuera del área de ensayo y cumple la normativa local.

Una compilación exitosa no valida el control; una prueba de banco tampoco certifica vuelo.
