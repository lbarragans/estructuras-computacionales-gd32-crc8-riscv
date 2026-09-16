# CRC-8 en Ensamblador RISC-V puro

`main.S` contiene toda la logica de aplicacion, sin codigo C: configuracion
MMIO de PC13, recorrido de los mensajes, calculo CRC-8/ATM, validacion y salida
visual.

El vector `123456789` debe producir `0xF4`. El mensaje alterado `123456788`
debe producir un valor diferente. Si ambas condiciones se cumplen, PC13
muestra los ocho bits de `0xF4`; si fallan, parpadea rapidamente.

Conceptos: secciones `.text/.rodata/.bss`, `lbu`, punteros, XOR, shifts,
branches, ABI, subrutinas, MMIO y depuracion de registros.

La seleccion `APP_VARIANT=assembly` conserva startup, linker y SDK. La
validacion fisica final se realiza con `tools/build_variant.ps1`.
