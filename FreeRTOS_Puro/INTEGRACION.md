# Integracion pendiente

Para compilar este apartado se requieren:

- FreeRTOS-Kernel;
- port RISC-V compatible con GD32VW553;
- `FreeRTOSConfig.h`;
- una implementacion de heap;
- tick y cambio de contexto;
- incorporacion de fuentes e includes al sistema de construccion.

No se debe marcar como validado hasta compilar, enlazar, arrancar el scheduler
y comprobar en la placa que `g_freertos_crc == 0xF4`.
