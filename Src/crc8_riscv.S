    .section .text
    .align  2
    .global riscv_crc8
    .type   riscv_crc8, @function

/*
 * uint8_t riscv_crc8(const uint8_t *data, uint32_t length)
 *
 * ABI RISC-V:
 *   a0 = puntero al primer byte; tambien contiene el valor de retorno
 *   a1 = numero de bytes
 *   t0 = CRC acumulado
 *   t1 = byte actual
 *   t2 = contador de bits
 *   t3 = bit mas significativo antes del desplazamiento
 */
riscv_crc8:
    li      t0, 0                  /* CRC inicial = 0x00 */
    beqz    a1, .Ldone

.Lbyte_loop:
    lbu     t1, 0(a0)              /* Leer un byte sin signo */
    xor     t0, t0, t1             /* Mezclarlo con el CRC */
    li      t2, 8                  /* Procesar ocho bits */

.Lbit_loop:
    andi    t3, t0, 0x80           /* Guardar el MSB */
    slli    t0, t0, 1              /* crc <<= 1 */
    andi    t0, t0, 0xff           /* Mantener solo ocho bits */
    beqz    t3, .Lwithout_poly
    xori    t0, t0, 0x07           /* Aplicar polinomio */

.Lwithout_poly:
    addi    t2, t2, -1
    bnez    t2, .Lbit_loop

    addi    a0, a0, 1              /* Siguiente byte */
    addi    a1, a1, -1
    bnez    a1, .Lbyte_loop

.Ldone:
    mv      a0, t0                  /* Retorno en a0 */
    ret

    .size riscv_crc8, .-riscv_crc8
