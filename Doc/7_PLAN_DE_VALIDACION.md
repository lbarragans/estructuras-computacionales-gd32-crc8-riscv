# 7. Plan de validacion

## Casos comunes

Todas las implementaciones deben comprobar:

| Caso | Resultado esperado |
|---|---|
| `"123456789"` | `0xF4` |
| `"123456788"` | diferente de `0xF4` |
| longitud cero | `0x00` |

Parametros: polinomio `0x07`, init `0x00`, sin reflexion y xorout `0x00`.

## Referencia original

1. Compilar con `build-debug`.
2. Verificar `g_crc_c == 0xF4` y `g_crc_asm == 0xF4`.
3. Confirmar `g_error_detected == 1` y `g_results_match == 1`.
4. Programar la placa y comprobar la trama luminosa.

## Ensamblador RISC-V puro

1. Integrar `Ensamblador_RISCV_Puro/main.S` sin C de aplicacion.
2. Observar los bytes cargados con `lbu` y el residuo parcial en `t0`.
3. Verificar `g_crc_ensamblador == 0xF4`.
4. Verificar que `g_crc_alterado` sea diferente.
5. Comprobar `g_resultados_ok == 1` y validar PC13 en placa.

## FreeRTOS puro

1. Integrar kernel, port RISC-V, heap, configuración y tick.
2. Verificar la creacion de las tres tareas y las dos queues.
3. Confirmar el flujo productor–procesador–validador.
4. Verificar `g_freertos_crc == 0xF4` y `g_freertos_ok == 1`.
5. Confirmar que la temporizacion usa `vTaskDelay` y no espera activa.
6. Programar la placa y comprobar la trama luminosa.

## Criterio de cierre

Las tres rutas deben producir el mismo CRC principal y detectar el mensaje
alterado. No se declara una ruta validada en hardware sin evidencia de
compilacion, programacion y medicion en la placa.

## Comandos de validacion en VS Code

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build_variant.ps1 -Variant original -Flash
powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build_variant.ps1 -Variant assembly -Flash
powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build_freertos.ps1 -Clean -Flash
```

Cada programacion debe terminar con `Verified OK`. La salida correcta muestra
ocho pulsos correspondientes a `11110100`; una secuencia rapida de error no
debe aparecer.
