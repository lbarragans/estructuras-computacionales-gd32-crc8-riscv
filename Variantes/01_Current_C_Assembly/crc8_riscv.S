.section .text
    .align  2
    .global riscv_crc8
    .type   riscv_crc8, @function

riscv_crc8:
    li      t0, 0
    beqz    a1, .Ldone

.Lbyte_loop:
    lbu     t1, 0(a0)
    xor     t0, t0, t1
    li      t2, 8

.Lbit_loop:
    andi    t3, t0, 0x80
    slli    t0, t0, 1
    andi    t0, t0, 0xff
    beqz    t3, .Lwithout_poly
    xori    t0, t0, 0x07

.Lwithout_poly:
    addi    t2, t2, -1
    bnez    t2, .Lbit_loop

    addi    a0, a0, 1
    addi    a1, a1, -1
    bnez    a1, .Lbyte_loop

.Ldone:
    mv      a0, t0
    ret

    .size riscv_crc8, .-riscv_crc8
