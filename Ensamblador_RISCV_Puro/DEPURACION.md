# Depuracion

Observe `g_crc_ensamblador`, `g_crc_alterado` y `g_resultados_ok`.

Puntos recomendados:

- `.Lcrc_byte`: byte cargado con `lbu`;
- `.Lcrc_bit`: residuo parcial en `t0`;
- `.Lcrc_done`: resultado en `a0`;
- `.Lbit_display`: mascara del bit mostrado en `s1`.

Valores esperados: CRC principal `0xF4`, CRC alterado distinto de `0xF4` y
bandera de resultados igual a uno.
