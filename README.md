# Exercise 03 - CRC-8 en C y Assembly RISC-V: multiples implementaciones

**Curso:** Estructuras Computacionales
**Autora:** Laura Daniela Barragan Silva
**Plataforma:** GD32VW553HMQ6/HMQ7
**Arquitectura:** Nuclei RISC-V RV32
**Entorno:** Visual Studio Code, CMake, Ninja, Nuclei RISC-V GCC y OpenOCD

## 1. Proposito

La implementacion principal del repositorio se conserva intacta:

```text
Src/main.c
Src/crc8_riscv.S
Inc/crc8_riscv.h
```

El ejercicio usa CRC-8 con:

```text
polinomio = 0x07
init      = 0x00
refin     = false
refout    = false
xorout    = 0x00
```

Para el vector estandar:

```text
"123456789" -> 0xF4
```

y para el mensaje alterado:

```text
"123456788" -> 0xF3
```

La pregunta de esta fase es:

> ¿Como cambia el mismo CRC cuando se prioriza claridad, memoria, velocidad,
> generalidad, procesamiento incremental o control de bajo nivel?

## 2. Menu de implementaciones

| # | Variante | Lenguaje / tecnologia | Idea principal | Estado |
|---:|---|---|---|---|
| 01 | C + Assembly actual | C + Assembly | equivalencia y ABI | Base actual |
| 02 | C bit a bit | C | especificacion directa | Fuente lista |
| 03 | C con tabla de 256 entradas | C | velocidad a cambio de ROM | Fuente lista |
| 04 | C con tabla de 16 entradas | C | compromiso memoria/velocidad | Fuente lista |
| 05 | Assembly branchless por bit | Assembly | mascara en vez de branch del polinomio | Fuente lista |
| 06 | CRC incremental/streaming | C | actualizar por fragmentos | Fuente lista |
| 07 | CRC parametrizable | C | polinomio/init/xorout configurables | Fuente lista |
| 08 | Comparador C vs Assembly con inyeccion de errores | C + Assembly | validacion sistematica | Fuente lista |
| 09 | Productor de bytes + CRC streaming | C | arquitectura de flujo | Fuente lista |
| 10 | DMA/IRQ/FreeRTOS: donde calcular el CRC | Documentacion | elegir arquitectura adecuada | Analisis |

Las fuentes educativas estan en [`Variantes/`](Variantes/README.md).

> El `CMakeLists.txt` principal no se modifica en esta fase. La version que ya
> compila sigue siendo la original hasta integrar y validar cada metodo.

## 3. Algoritmo bit a bit

Para cada byte:

```text
crc ^= byte
repetir 8 veces:
    si MSB == 1:
        crc = (crc << 1) XOR 0x07
    si no:
        crc = crc << 1
```

La variante C es una referencia muy legible y la variante Assembly hace
explicitas las operaciones:

- `lbu`;
- `xor`;
- `andi`;
- `slli`;
- `xori`;
- branches;
- avance de puntero.

## 4. Por que `lbu` es importante

Los datos son bytes sin signo.

Assembly usa:

```asm
lbu t1, 0(a0)
```

y no `lb`.

`lbu` extiende con ceros a XLEN; `lb` extenderia el bit de signo.

## 5. Tabla de 256 entradas

El CRC de un byte puede precomputarse.

Entonces el bucle principal se reduce conceptualmente a:

```c
crc = table[crc ^ byte];
```

Ventaja:

- menos trabajo por byte.

Costo:

- 256 bytes de tabla para CRC-8;
- mas accesos a memoria;
- menor transparencia del algoritmo.

## 6. Tabla de 16 entradas

La variante nibble procesa:

```text
nibble alto
nibble bajo
```

y usa solo 16 entradas.

Es un compromiso util para microcontroladores con memoria limitada.

## 7. Assembly branchless

La implementacion base pregunta:

```text
¿MSB era 1?
```

y usa un salto condicional.

La variante 05 convierte ese bit en una mascara:

```text
0x00000000
o
0xFFFFFFFF
```

para aplicar `0x07` sin branch dentro de la decision del polinomio.

No se declara mas rapida sin medicion real.

## 8. CRC incremental

En sistemas reales los datos no siempre llegan en un bloque completo.

Pueden llegar:

```text
UART -> fragmento
red  -> paquete
DMA  -> mitad de buffer
flash -> bloque
```

Un CRC bien disenado puede mantener estado:

```text
init
update(fragmento 1)
update(fragmento 2)
...
finalize
```

El resultado debe ser igual al CRC del bloque completo si el orden de los bytes
es el mismo.

## 9. CRC parametrizable

"CRC-8" no identifica una unica variante.

Cambiar:

- polinomio;
- init;
- reflexion;
- xorout;

cambia el resultado.

La variante 07 mantiene el modelo no reflejado del ejercicio pero permite variar
polinomio, init y xorout para estudiar esta dependencia.

## 10. Deteccion de errores

CRC no "corrige" automaticamente el mensaje.

Su funcion es producir una firma que permita detectar muchas alteraciones.

La variante 08 compara C y Assembly y prueba cambios de bits controlados.

## 11. Flujo de datos

La variante 09 modela el procesamiento como:

```text
fuente de bytes
      ↓
crc8_update()
      ↓
estado CRC
      ↓
resultado final
```

Es el paso conceptual previo a UART, DMA, red o tareas.

## 12. Interrupciones, DMA y FreeRTOS

No conviene calcular todo el CRC dentro de una ISR larga.

Una arquitectura mas razonable es:

```text
IRQ/DMA -> datos listos
              ↓
main/tarea -> procesa bloque -> actualiza CRC
```

FreeRTOS puede ser util si varios productores/consumidores trabajan de forma
concurrente, pero no es necesario para calcular el CRC de una constante.

## 13. Lenguajes

Este repositorio utiliza de verdad:

```text
C
Assembly RISC-V
```

Por eso ambos deben contar.

No deben alterar la barra:

```text
PowerShell
CMake
JSON
VS Code
Markdown
```

## 14. Validacion

Cada implementacion debe verificarse contra, como minimo:

```text
"123456789" -> 0xF4
```

y compararse con la implementacion de referencia.

Ver [`Doc/7_PLAN_DE_VALIDACION.md`](Doc/7_PLAN_DE_VALIDACION.md).
