# 05 - Assembly branchless para aplicar el polinomio

## Lenguaje

Assembly RISC-V.

## Idea

Se elimina el branch que decide si aplicar `0x07`.

El MSB se convierte en mascara:

```text
0 -> 0x00000000
1 -> 0xFFFFFFFF
```

y luego:

```text
poly & mask
```

## Advertencia

Todavia existen branches del bucle. "Branchless" se refiere solo a la decision
del polinomio.

No se afirma mejor rendimiento sin medicion.

## Estado

**Fuente lista.**
