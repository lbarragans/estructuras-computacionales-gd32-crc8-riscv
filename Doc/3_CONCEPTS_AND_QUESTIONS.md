# 3. Conceptos y preguntas

## Qué es un CRC

Un código de redundancia cíclica produce un residuo a partir de una secuencia de
bits. El emisor adjunta ese valor y el receptor repite el cálculo. Si ambos
valores difieren, la información fue modificada durante almacenamiento o
transmisión.

Un CRC detecta errores; no cifra información, no demuestra identidad y no
repara por sí solo el dato alterado.

## Parámetros de esta variante

| Parámetro | Valor |
| --- | --- |
| Ancho | 8 bits |
| Polinomio | `0x07` |
| Inicial | `0x00` |
| Entrada reflejada | No |
| Salida reflejada | No |
| XOR final | `0x00` |
| Vector de control | `123456789 → 0xF4` |

El nombre “CRC-8” no basta para reproducir un resultado: deben especificarse
todos los parámetros.

## Interpretación polinómica

El polinomio `0x07` representa los términos inferiores de:

```text
x^8 + x^2 + x + 1
```

El término `x^8` es implícito por el ancho. En aritmética binaria, suma y resta
se realizan con XOR, sin acarreo.

## Operaciones de la implementación

| Operación | Significado |
| --- | --- |
| `crc XOR byte` | Incorpora el byte actual al residuo. |
| `crc & 0x80` | Consulta el bit más significativo. |
| `crc << 1` | Avanza una posición en la división. |
| `XOR 0x07` | Aplica el polinomio cuando el MSB era uno. |
| `& 0xFF` | Mantiene el estado dentro de ocho bits en RV32. |

## C frente a ensamblador

En C, tipos, bucles y punteros expresan la intención. En ensamblador se hacen
explícitos la carga de memoria, contadores, saltos y registros.

### ABI de `riscv_crc8`

```c
uint8_t riscv_crc8(const uint8_t *data, uint32_t length);
```

| Registro | Uso al entrar o durante la función |
| --- | --- |
| `a0` | puntero `data`; al terminar contiene el CRC retornado. |
| `a1` | número de bytes pendientes. |
| `t0` | CRC acumulado. |
| `t1` | byte leído con `lbu`. |
| `t2` | contador de los ocho bits. |
| `t3` | copia del MSB antes del desplazamiento. |
| `ra` | dirección de retorno utilizada por `ret`. |

Los registros `a*` y `t*` son *caller-saved*: la función puede modificarlos.
No se utilizan registros `s*`, por lo que no es necesario guardarlos en la
pila. La rutina tampoco llama otras funciones y es una función hoja.

## Instrucciones RISC-V relevantes

| Instrucción | Efecto |
| --- | --- |
| `lbu t1, 0(a0)` | carga un byte y lo extiende con ceros. |
| `xor` / `xori` | XOR entre registros o con inmediato. |
| `andi` | aplica una máscara. |
| `slli` | desplazamiento lógico a la izquierda. |
| `addi` | incrementa puntero o decrementa contador. |
| `beqz` / `bnez` | salto según si un registro es cero. |
| `mv a0, t0` | coloca el resultado en el registro de retorno. |
| `ret` | vuelve al llamador. |

## Detección de la alteración

Los mensajes solo difieren en el último carácter:

```text
123456789 → 0xF4
123456788 → 0xF3
```

El resultado distinto permite detectar el cambio. Esto no significa que un CRC
pueda detectar absolutamente cualquier patrón de error; su capacidad depende
del polinomio, el ancho y la longitud de la trama.

## CRC, checksum, cifrado y autenticación

| Técnica | Objetivo principal |
| --- | --- |
| Suma de comprobación | detectar algunos errores con bajo costo. |
| CRC | detectar patrones de error en bloques o comunicaciones. |
| Cifrado | ocultar el contenido sin la clave. |
| MAC/firma | comprobar integridad con autenticidad. |
| ECC | detectar y, según el código, corregir errores. |

## Preguntas de análisis

1. ¿Por qué `lbu` es más apropiada que `lw` para recorrer el mensaje?
2. ¿Por qué se conserva únicamente `0xFF` después del desplazamiento?
3. ¿Qué registros contienen los argumentos de la función?
4. ¿Por qué la función no necesita crear un marco de pila?
5. ¿Qué cambiaría si el CRC fuera reflejado?
6. ¿Por qué CRC no debe confundirse con cifrado?
7. ¿Qué evidencia aporta comparar C, ASM y el vector conocido `0xF4`?
8. ¿Cómo se comprobaría que ambos algoritmos recorren exactamente nueve bytes?
9. ¿Qué sucede si `length` vale cero?
10. ¿Por qué el mensaje alterado produce un CRC diferente pero del mismo ancho?

## Actividades propuestas

1. Cambie un solo carácter y registre el nuevo CRC.
2. Coloque un breakpoint dentro de `.Lbit_loop` y observe `t0`.
3. Compare en `GD32VW55x.lst` las instrucciones generadas para `crc8_c` con la
   rutina escrita manualmente.
4. Añada un contador global de bytes y compruebe que vale nueve.
5. Explique la trama luminosa `11110100` sin consultar el valor hexadecimal.
