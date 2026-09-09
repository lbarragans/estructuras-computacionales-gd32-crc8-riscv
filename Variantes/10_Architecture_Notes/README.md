# 10 - CRC con IRQ, DMA y FreeRTOS: que debe hacer cada capa

## Interrupcion

Una ISR deberia hacer el minimo trabajo necesario.

Ejemplo:

```text
UART IRQ -> guarda byte / marca evento
main     -> actualiza CRC
```

No conviene ejecutar un CRC largo dentro de una ISR si eso aumenta la latencia
de otras interrupciones.

## DMA

Una arquitectura comun es:

```text
periferico
   ↓ DMA
buffer
   ↓ IRQ: bloque listo
procesador/main/task
   ↓
crc8_update(buffer)
```

La interrupcion anuncia disponibilidad; el CRC procesa datos fuera de la ISR.

## FreeRTOS

Puede ser util si:

- existen varios flujos de datos;
- hay productor/consumidor;
- se necesitan colas;
- el procesamiento compite con otras actividades.

Ejemplo:

```text
UART/DMA task
     ↓ Queue
CRC task
     ↓
validator task
```

## Cuando NO hace falta

Para calcular una sola vez el CRC de `"123456789"` no se necesita un RTOS.

## Leccion

La tecnica correcta depende de como llegan los datos, no del deseo de usar todas
las herramientas disponibles.
