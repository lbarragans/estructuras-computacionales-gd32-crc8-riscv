# 06 - CRC incremental / streaming

## Lenguaje

C.

## Prueba

```text
update("123")
update("456")
update("789")
```

debe dar el mismo resultado que:

```text
CRC("123456789") = 0xF4
```

## Aplicacion

UART, DMA, red, archivos y buffers parciales.

## Estado

**Fuente lista.**
