# 6. Variantes del CRC-8

## Comparacion

| Variante | Memoria de tabla | Trabajo por byte | Generalidad | Assembly |
|---|---:|---|---|---:|
| Base C/ASM | 0 | 8 iteraciones de bit | fija | Si |
| C bitwise | 0 | 8 iteraciones de bit | fija | No |
| Tabla 256 | 256 B | 1 lookup | fija | No |
| Tabla 16 | 16 B | 2 lookups | fija | No |
| ASM branchless | 0 | 8 pasos de bit | fija | Si |
| Streaming | 0 | depende del backend | fragmentos | No |
| Parametrizable | 0 | bitwise | alta | No |
| Error injection | 0 | varias ejecuciones | prueba | Si |
| Pipeline | 0 | incremental | flujo | No |

## Preguntas

1. ¿Por que el vector `"123456789"` es tan util?
2. ¿Que hace `crc ^= byte`?
3. ¿Por que se procesan exactamente ocho bits por byte?
4. ¿Que representa el polinomio `0x07`?
5. ¿Por que `lbu` es preferible a `lb`?
6. ¿Que costo de memoria tiene una tabla de 256 entradas?
7. ¿Por que una tabla de 16 entradas necesita dos pasos por byte?
8. ¿Que significa que un CRC sea incremental?
9. ¿Por que no debe reinicializarse el CRC entre fragmentos de un mismo mensaje?
10. ¿Por que dos algoritmos llamados CRC-8 pueden dar resultados diferentes?
11. ¿Que ventaja y que riesgo tiene eliminar un branch?
12. ¿Por que una ISR deberia ser corta?
