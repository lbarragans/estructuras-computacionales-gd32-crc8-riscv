# 2. Compilación y programación

## Configurar CMake

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\configure.ps1 -BuildType Debug
```

Debe terminar con `Configuring done` y `Generating done`.

## Compilar

```powershell
cmake --build --preset build-debug
```

CMake compila `Src/main.c` y `Src/crc8_riscv.S`, enlaza el firmware y genera:

- `GD32VW55x.elf`;
- `GD32VW55x.hex`;
- `GD32VW55x.bin`;
- `GD32VW55x.map`;
- `GD32VW55x.lst`.

Compruebe:

```powershell
Get-ChildItem .\build\debug\GD32VW55x.*
```

## Programar

Conecte el depurador y ejecute:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\flash.ps1 -BuildType Debug
```

El resultado debe incluir:

```text
** Programming Finished **
** Verified OK **
** Resetting Target **
```

## Resultado físico

Después de dos destellos de sincronización, PC13 representa ocho bits. Para
`0xF4` deben verse cuatro pulsos largos, uno corto, uno largo y dos cortos.
La trama vuelve a comenzar después de la pausa.
