aarch64-linux-gnu-gcc -c boot.s -o boot.o
aarch64-linux-gnu-gcc -c lib.s -o liba.o
aarch64-linux-gnu-gcc -c handler.s -o handlera.o
aarch64-linux-gnu-gcc -c mmu.s -o mmu.o
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only  -c main.c
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only  -c uart.c
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only  -c print.c
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only  -c debug.c
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only  -c handler.c
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only  -c memory.c
aarch64-linux-gnu-ld -nostdlib -T link.lds -o kernel boot.o main.o liba.o uart.o print.o debug.o handlera.o handler.o mmu.o memory.o
aarch64-linux-gnu-objcopy -O binary kernel kernel8.img