.section .text
    .align 2

    .global riscv_crc8_branchless
    .type riscv_crc8_branchless, @function

/*
 * uint8_t riscv_crc8_branchless(const uint8_t *data, uint32_t length)
 *
 * a0 pointer / return
 * a1 length
 */
riscv_crc8_branchless:
    li      t0, 0
    beqz    a1, .Ldone

.Lbyte:
    lbu     t1, 0(a0)
    xor     t0, t0, t1
    li      t2, 8

.Lbit:
    srli    t3, t0, 7
    andi    t3, t3, 1
    sub     t3, zero, t3       /* 0 or 0xFFFFFFFF */

    slli    t0, t0, 1
    andi    t0, t0, 0xFF

    li      t4, 0x07
    and     t4, t4, t3
    xor     t0, t0, t4

    addi    t2, t2, -1
    bnez    t2, .Lbit

    addi    a0, a0, 1
    addi    a1, a1, -1
    bnez    a1, .Lbyte

.Ldone:
    mv      a0, t0
    ret

    .size riscv_crc8_branchless, .-riscv_crc8_branchless
