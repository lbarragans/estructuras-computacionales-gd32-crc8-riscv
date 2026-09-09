# 04 - CRC con tabla de 16 entradas

## Lenguaje

C.

## Metodo

Cada byte se procesa como dos nibbles.

## Compromiso

- tabla: 16 bytes;
- dos lookups por byte;
- menos ROM que tabla 256;
- normalmente mas rapido que bitwise, pero debe medirse.

## Estado

**Fuente lista.**
