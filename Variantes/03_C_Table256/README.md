# 03 - CRC con tabla de 256 entradas

## Lenguaje

C.

## Metodo

Cada combinacion posible de byte tiene precomputada su transformacion:

```c
crc = table[crc ^ byte];
```

## Ventaja

Elimina el bucle de 8 bits en tiempo de ejecucion.

## Costo

La tabla ocupa 256 bytes.

## Estado

**Fuente lista; pendiente de benchmark.**
