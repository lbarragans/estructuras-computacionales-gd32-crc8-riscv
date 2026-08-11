# 4. Depuración

## Crear launch.json

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\create_debug_config.ps1
```

## Breakpoints recomendados

En `Src/main.c`:

1. llamada a `crc8_c(test_message, message_length)`;
2. llamada a `riscv_crc8(test_message, message_length)`;
3. asignación de `g_results_match`;
4. entrada de `led_show_byte(g_crc_asm)`.

En `Src/crc8_riscv.S`:

1. `lbu t1, 0(a0)` para observar cada byte;
2. `andi t3, t0, 0x80` para observar cada bit;
3. `mv a0, t0` para comprobar el retorno.

## Watch

Agregue estas expresiones:

```text
g_crc_c
g_crc_asm
g_crc_corrupted
g_error_detected
g_results_match
g_crc_c,x
g_crc_asm,x
```

La presentación hexadecimal también puede seleccionarse desde el menú de cada
variable.

## Registros al entrar en ensamblador

- `a0`: dirección de `test_message`;
- `a1`: `9`;
- `t0`: todavía no contiene el CRC inicial hasta ejecutar `li t0, 0`.

Use Step Into para entrar a `riscv_crc8`. Al finalizar, `a0` debe contener
`0xF4`. Como `a0` fue usado inicialmente como puntero, la rutina lo avanza y
finalmente lo reemplaza por el retorno.

## Prueba de error controlada

Cambie temporalmente `CRC8_EXPECTED` de `0xF4U` a `0xF5U`, compile y programe.
Debe aparecer parpadeo rápido. Restaure `0xF4U` después de la demostración.

## Optimización

La configuración Debug usa `-Og -g3`. En Release, algunas variables y líneas
pueden optimizarse, por lo que la observación paso a paso es menos directa.
