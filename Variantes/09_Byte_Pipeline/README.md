# 09 - Pipeline productor de bytes -> CRC

## Lenguaje

C.

## Idea

La interfaz recibe un byte cada vez:

```text
push_byte(0x31)
push_byte(0x32)
...
```

y mantiene:

```text
crc
bytes_processed
```

## Por que es util

Se parece a un sistema donde los bytes llegan progresivamente desde un
periferico o protocolo.

## Estado

**Fuente lista.**
