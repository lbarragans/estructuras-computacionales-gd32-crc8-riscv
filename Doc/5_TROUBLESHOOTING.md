# 5. Solución de problemas

| Problema | Causa probable | Solución |
| --- | --- | --- |
| No encuentra el compilador | Ruta local incorrecta o no exportada. | Ejecute `verify_environment.ps1` y configure con el script. |
| No ensambla `crc8_riscv.S` | Archivo ausente de `APP_SOURCES` o sintaxis alterada. | Compruebe CMake y restaure el archivo. |
| `undefined reference to riscv_crc8` | Nombre distinto entre `.h`, C y `.S`, o falta `.global`. | Use exactamente `riscv_crc8`. |
| C y ASM no coinciden | Bucle, máscara o polinomio modificados. | Deténgase por byte y por bit; compare `t0` con `crc`. |
| Ambos dan un valor distinto de `0xF4` | Parámetros o mensaje cambiaron. | Revise `0x07`, init `0x00` y `123456789`. |
| `g_crc_corrupted` también vale `0xF4` | La trama alterada quedó igual o hubo un error de longitud. | Revise último byte y `message_length`. |
| LED parpadea rápido | Falló una de las tres comprobaciones. | Observe las cinco variables globales. |
| No se distinguen bits | Ritmos físicos demasiado rápidos para la placa/observador. | Aumente `BIT_ZERO_MS`, `BIT_ONE_MS` y `BIT_GAP_MS`. |
| Breakpoint ASM no se activa | Fuente no corresponde al ELF o punto sin instrucción. | Recompile Debug y ponga el punto sobre una instrucción. |
| OpenOCD no conecta | Depurador ocupado o JTAG desconectado. | Detenga otras sesiones y revise conexión. |

## Limpiar y reconstruir

Solo si cambió SDK, toolchain o estructura de CMake:

```powershell
Remove-Item .\build -Recurse -Force -ErrorAction SilentlyContinue
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\configure.ps1 -BuildType Debug
cmake --build --preset build-debug
```

## Verificar archivos privados antes de Git

```powershell
git check-ignore .\build
git check-ignore .\tools\local_config.ps1
git check-ignore .\.vscode\launch.json
```

Las tres rutas deben aparecer como ignoradas.
