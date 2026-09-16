# Integracion funcional con el MSDK oficial

Esta variante usa el FreeRTOS, port Nuclei/ECLIC, heap, tick, startup y linker
ya integrados y probados por GigaDevice en `GD32VW55x_RELEASE_V1.0.3g`.

Todo se ejecuta desde la terminal integrada o las tareas de VS Code:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\build_freertos.ps1 `
  -Clean -Flash
```

El script respalda la aplicacion original del MSDK, copia `main.c` y
`app_cfg.h`, compila MBL + MSDK, genera `image-all.bin` y lo programa por
WCH-Link/CMSIS-DAP. No se necesita CH340, BOOT0 ni un IDE del fabricante.

`main.c` llama `platform_init()`, crea las tareas/objetos FreeRTOS y entrega
el control a `sys_os_start()`. No descargue otro kernel ni mezcle un port
RISC-V generico con el ECLIC de este dispositivo.

La trama correcta representa `0xF4 = 11110100`: pulso largo para 1 y corto
para 0. En el depurador deben observarse `g_freertos_crc = 0xF4` y
`g_freertos_ok = 1`.
