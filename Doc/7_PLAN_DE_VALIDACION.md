# 7. Plan de validacion

## Vector principal

```text
Entrada:  31 32 33 34 35 36 37 38 39
ASCII:    1  2  3  4  5  6  7  8  9
CRC-8:    F4
```

Parametros:

```text
poly   = 0x07
init   = 0x00
refin  = false
refout = false
xorout = 0x00
```

## Casos adicionales

### Longitud cero

Con init `0x00` y xorout `0x00`:

```text
CRC(empty) = 0x00
```

### Un byte

Comparar todas las implementaciones para varios valores:

```text
0x00
0x01
0x80
0xFF
```

### Mensaje alterado

```text
"123456788"
```

Debe producir un CRC distinto al del mensaje original.

## Equivalencia

Para cada vector:

```text
crc_bitwise
crc_table256
crc_nibble
crc_asm
crc_streaming
```

deben coincidir cuando usan los mismos parametros.

## Streaming

Probar:

```text
"123" + "456" + "789"
```

y verificar:

```text
CRC("123456789") == CRC_streaming("123","456","789")
```

## Assembly

Observar:

- `a0` puntero/retorno;
- `a1` longitud;
- `t0` CRC;
- `t1` byte;
- `t2` contador;
- `t3` bit/mascara.

## Rendimiento

No afirmar que una version es mas rapida sin medir.

Posibles metricas:

- ciclos;
- instrucciones en `.lst`;
- tiempo mediante GPIO;
- bytes de codigo;
- bytes de tabla.

## Madurez

- Base actual
- Fuente lista
- Analisis
- Integracion pendiente
- Validada por compilacion
- Validada en placa
