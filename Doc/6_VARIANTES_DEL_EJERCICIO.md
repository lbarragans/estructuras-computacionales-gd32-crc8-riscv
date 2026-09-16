# 6. Implementaciones definitivas

## Referencia original

Se conserva:

```text
Src/main.c
Src/crc8_riscv.S
Inc/crc8_riscv.h
```

Permite comparar el algoritmo C con una funcion RISC-V llamada mediante ABI.

## Ensamblador RISC-V puro

`Ensamblador_RISCV_Puro/main.S` contiene toda la logica de aplicacion:

- `.text`, `.rodata` y `.bss`;
- CRC-8/ATM;
- `lbu`, XOR, shifts y branches;
- punteros y contadores;
- subrutinas y `call/ret`;
- variables visibles para depuracion;
- MMIO de GPIOC;
- validacion y salida LED.

No existe codigo C de aplicacion dentro de este apartado.

## FreeRTOS puro

`FreeRTOS_Puro/main.c` presenta:

- tres tareas con responsabilidades separadas;
- dos queues;
- transferencia de estructuras;
- bloqueo mediante `xQueueReceive`;
- temporizacion con `vTaskDelay`;
- validacion del vector y deteccion del mensaje alterado.

No llama las rutinas Assembly de la referencia.
