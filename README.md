# Ejercicio 03 - CRC-8 en GD32VW553

**Curso:** Estructuras Computacionales

**Autora:** Laura Daniela Barragan Silva

**Plataforma:** GD32VW553HMQ6/HMQ7
**Arquitectura:** Nuclei RISC-V RV32

## 1. Objetivo

Calcular y validar CRC-8/ATM con:

```text
polinomio = 0x07
init      = 0x00
refin     = false
refout    = false
xorout    = 0x00
```

Vector conocido:

```text
"123456789" -> 0xF4
```

Mensaje alterado:

```text
"123456788" -> valor diferente de 0xF4
```

## 2. Tres caminos de estudio

| Camino | Archivos | Concepto principal |
|---|---|---|
| Referencia original | `Src/main.c`, `Src/crc8_riscv.S`, `Inc/crc8_riscv.h` | equivalencia C/Assembly y ABI |
| Ensamblador RISC-V puro | `Ensamblador_RISCV_Puro/main.S` | algoritmo, MMIO y aplicacion completa sin C |
| FreeRTOS puro | `FreeRTOS_Puro/main.c` | tareas, queues y procesamiento concurrente |

Las diez variantes comparativas anteriores fueron retiradas. La estructura
queda alineada con los ejercicios 00, 01 y 02.

## 3. Estructura

```text
03_CRC8_C_vs_RISCV_Assembly/
├── Src/
│   ├── main.c
│   └── crc8_riscv.S
├── Inc/
│   └── crc8_riscv.h
├── Ensamblador_RISCV_Puro/
│   ├── main.S
│   ├── README.md
│   └── DEPURACION.md
├── FreeRTOS_Puro/
│   ├── main.c
│   ├── README.md
│   └── INTEGRACION.md
├── Doc/
├── CMakeLists.txt
└── README.md
```

## 4. Referencia original

La compilacion principal permanece intacta. Calcula el CRC en C y mediante la
funcion `riscv_crc8`, compara ambos resultados y representa `0xF4` en PC13.

Esta ruta permite estudiar paso de argumentos, valor de retorno, registros
temporales y equivalencia entre C y RISC-V Assembly.

## 5. Ensamblador RISC-V puro

`Ensamblador_RISCV_Puro/main.S` realiza directamente:

- configuracion MMIO de GPIOC/PC13;
- lectura byte a byte con `lbu`;
- calculo CRC-8/ATM;
- validacion del vector conocido y del mensaje alterado;
- presentacion binaria del CRC mediante el LED.

No contiene ni llama codigo C de aplicacion.

## 6. FreeRTOS puro

`FreeRTOS_Puro/main.c` separa responsabilidades:

```text
ProducerTask
    ↓ job_queue
ProcessorTask
    ↓ result_queue
ValidatorTask
    ↓ PC13
```

El procesamiento usa primitivas nativas de FreeRTOS y no llama la rutina
Assembly de la referencia.

## 7. Estado

| Implementacion | Estado |
|---|---|
| Referencia C + Assembly | funcional y conservada como compilacion principal |
| Ensamblador RISC-V puro | fuente lista; integracion y prueba en placa pendientes |
| FreeRTOS puro | fuente lista; kernel/port e integracion pendientes |

## 8. Lenguajes

C y Assembly son lenguajes reales de las implementaciones. FreeRTOS es una
tecnologia/RTOS. PowerShell, CMake, JSON y Markdown permanecen excluidos de los
porcentajes mediante `.gitattributes`.
