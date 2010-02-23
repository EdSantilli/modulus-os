#!/bin/bash
ld -M -T link.ld boot.o irq.o kernel.o basicvid.o page.o string.o idt.o handlers.o timer.o kmalloc.o task.o
