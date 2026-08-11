# 1. Preparación del entorno

## Requisitos

- GD32VW553HMQ6/HMQ7;
- WCH-Link o interfaz CMSIS-DAP conectada por JTAG;
- Visual Studio Code;
- CMake 3.20 o posterior;
- Ninja;
- Nuclei RISC-V GCC/GDB;
- OpenOCD con `target/gd32vw55x.cfg`;
- `GD32VW55x_Firmware_Library_V1.6.0`.

La instalación general está documentada en el repositorio
`gd32vw553-vscode-cmake-guide`.

## Configuración local

Desde PowerShell en la raíz del ejercicio:

```powershell
Copy-Item .\tools\local_config.example.ps1 .\tools\local_config.ps1
notepad .\tools\local_config.ps1
```

Configure:

```powershell
$GD32_SDK_ROOT = "C:/ruta/GD32VW55x_Firmware_Library_V1.6.0"
$NUCLEI_TOOLCHAIN_DIR = "C:/ruta/NucleiRISCVGCC/bin"
$OPENOCD_ROOT = "C:/ruta/OpenOCD/xpack-openocd-0.11.0-3"
```

No publique `tools/local_config.ps1`.

## Verificación

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\verify_environment.ps1
```

Todos los elementos deben aparecer como `[OK]`.

## Extensiones de VS Code

- C/C++ de Microsoft;
- CMake Tools;
- Cortex-Debug.

Abra únicamente la carpeta `03_CRC8_C_vs_RISCV_Assembly` como raíz del espacio
de trabajo.
