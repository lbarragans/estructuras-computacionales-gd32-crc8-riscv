    .section .text
    .align 2

    /* Ejercicio 03: CRC-8/ATM completamente en RISC-V Assembly. */
    .equ RCU_AHB1EN,       0x40023830
    .equ GPIOC_CTL,        0x40020800
    .equ GPIOC_OMODE,      0x40020804
    .equ GPIOC_OSPD,       0x40020808
    .equ GPIOC_PUD,        0x4002080C
    .equ GPIOC_BOP,        0x40020818
    .equ GPIOC_BC,         0x40020828
    .equ GPIOC_TG,         0x4002082C

    .equ GPIOC_CLOCK_BIT,  0x00000004
    .equ PC13_BIT,         0x00002000
    .equ PC13_FIELD_MASK,  0x0C000000
    .equ PC13_OUTPUT_MODE, 0x04000000
    .equ PC13_SPEED_10MHZ, 0x04000000
    .equ MESSAGE_LENGTH,   9
    .equ EXPECTED_CRC,     0xF4

    .global main
    .type main, @function

main:
    /* Habilitar GPIOC y configurar PC13 como salida push-pull. */
    li      t0, RCU_AHB1EN
    lw      t1, 0(t0)
    ori     t1, t1, GPIOC_CLOCK_BIT
    sw      t1, 0(t0)
    fence   iorw, iorw

    li      t0, GPIOC_CTL
    lw      t1, 0(t0)
    li      t2, PC13_FIELD_MASK
    not     t2, t2
    and     t1, t1, t2
    li      t2, PC13_OUTPUT_MODE
    or      t1, t1, t2
    sw      t1, 0(t0)

    li      t0, GPIOC_OMODE
    lw      t1, 0(t0)
    li      t2, PC13_BIT
    not     t2, t2
    and     t1, t1, t2
    sw      t1, 0(t0)

    li      t0, GPIOC_OSPD
    lw      t1, 0(t0)
    li      t2, PC13_FIELD_MASK
    not     t2, t2
    and     t1, t1, t2
    li      t2, PC13_SPEED_10MHZ
    or      t1, t1, t2
    sw      t1, 0(t0)

    li      t0, GPIOC_PUD
    lw      t1, 0(t0)
    li      t2, PC13_FIELD_MASK
    not     t2, t2
    and     t1, t1, t2
    sw      t1, 0(t0)

    /* LED activo en bajo: iniciar apagado. */
    li      t0, GPIOC_BOP
    li      t1, PC13_BIT
    sw      t1, 0(t0)

    /* CRC del vector estandar. */
    la      a0, test_message
    li      a1, MESSAGE_LENGTH
    call    crc8_atm
    la      t0, g_crc_ensamblador
    sb      a0, 0(t0)
    mv      s0, a0

    /* CRC del mensaje alterado. */
    la      a0, corrupted_message
    li      a1, MESSAGE_LENGTH
    call    crc8_atm
    la      t0, g_crc_alterado
    sb      a0, 0(t0)

    li      t1, EXPECTED_CRC
    bne     s0, t1, .Lerror
    beq     s0, a0, .Lerror

    la      t0, g_resultados_ok
    li      t1, 1
    sw      t1, 0(t0)

    /* Mostrar 0xF4, MSB primero: largo=1 y corto=0. */
.Lframe:
    li      s1, 0x80
.Lbit_display:
    li      t0, GPIOC_BC
    li      t1, PC13_BIT
    sw      t1, 0(t0)

    and     t2, s0, s1
    beqz    t2, .Lshort_pulse
    li      a0, 600
    j       .Lpulse_delay
.Lshort_pulse:
    li      a0, 150
.Lpulse_delay:
    call    delay_aprox_ms

    li      t0, GPIOC_BOP
    li      t1, PC13_BIT
    sw      t1, 0(t0)
    li      a0, 250
    call    delay_aprox_ms

    srli    s1, s1, 1
    bnez    s1, .Lbit_display
    li      a0, 2000
    call    delay_aprox_ms
    j       .Lframe

.Lerror:
    la      t0, g_resultados_ok
    sw      zero, 0(t0)
.Lerror_loop:
    li      t0, GPIOC_TG
    li      t1, PC13_BIT
    sw      t1, 0(t0)
    li      a0, 100
    call    delay_aprox_ms
    j       .Lerror_loop

    .size main, .-main

    .align 2
    .type crc8_atm, @function
crc8_atm:
    li      t0, 0
    beqz    a1, .Lcrc_done
.Lcrc_byte:
    lbu     t1, 0(a0)
    xor     t0, t0, t1
    li      t2, 8
.Lcrc_bit:
    andi    t3, t0, 0x80
    slli    t0, t0, 1
    andi    t0, t0, 0xFF
    beqz    t3, .Lcrc_next_bit
    xori    t0, t0, 0x07
.Lcrc_next_bit:
    addi    t2, t2, -1
    bnez    t2, .Lcrc_bit
    addi    a0, a0, 1
    addi    a1, a1, -1
    bnez    a1, .Lcrc_byte
.Lcrc_done:
    mv      a0, t0
    ret
    .size crc8_atm, .-crc8_atm

    .align 2
    .type delay_aprox_ms, @function
delay_aprox_ms:
    mv      t0, a0
.Ldelay_ms:
    beqz    t0, .Ldelay_done
    li      t1, 1000
.Ldelay_us:
    li      t2, 16
.Ldelay_inner:
    nop
    addi    t2, t2, -1
    bnez    t2, .Ldelay_inner
    addi    t1, t1, -1
    bnez    t1, .Ldelay_us
    addi    t0, t0, -1
    j       .Ldelay_ms
.Ldelay_done:
    ret
    .size delay_aprox_ms, .-delay_aprox_ms

    .section .rodata
test_message:
    .ascii "123456789"
corrupted_message:
    .ascii "123456788"

    .section .bss
    .align 2
    .global g_crc_ensamblador
    .global g_crc_alterado
    .global g_resultados_ok
g_crc_ensamblador:
    .byte 0
g_crc_alterado:
    .byte 0
    .align 2
g_resultados_ok:
    .word 0
