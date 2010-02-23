##############################################################################
#           makefile
#
#	Mon 04 Feb 2008 16:01:13 EST
#   Copyright  2008  Ed Santilli
#   EdSantilli@gmail.com
##############################################################################

# The kernel's makefile

# Source files
ASMSRCS := boot.S isr.S irq.S
CSRCS   := basicvid.c handlers.c idt.c kernel.c memmap.c page.c string.c timer.c

# Object files
ASMOBJS = $(addprefix $(BUILDDIR)/,$(ASMSRCS:.S=.o))
COBJS = $(addprefix $(BUILDDIR)/,$(CSRCS:.c=.o))

#Linker script
PPLDSCRIPT = $(SRCDIR)/link.ld.pp
LDSCRIPT = $(BUILDDIR)/$(basename $(notdir $(PPLDSCRIPT)))

# The final product - the kernel binary file
PROG = $(BINDIR)/kernel0


# Important directories
ROOTDIR   = $(CURDIR)
SRCDIR	  = $(ROOTDIR)/src
CSRCDIR   = $(SRCDIR)
ASMSRCDIR = $(SRCDIR)/sys
INCDIR    = $(SRCDIR)
BUILDDIR  = $(ROOTDIR)/build
BINDIR    = $(ROOTDIR)


# Compilers and preprocessors
ASM = gcc
CC = gcc
PP = gcc
LD = ld

# Compiler and preprocessor options
FLAGS = -m32 -std=gnu99 -Wall -Wextra -O -fstrength-reduce -fomit-frame-pointer\
 -finline-functions -nostdinc -fno-builtin -fno-stack-protector -I $(INCDIR) -c
ASMFLAGS = $(FLAGS)
CFLAGS = $(FLAGS)
PPFLAGS = $(FLAGS) -E -P -x c
LDFLAGS  = -m elf_i386 -T $(LDSCRIPT)


.PHONY: all clean report include

all: $(PROG)
$(PROG): $(LDSCRIPT) $(ASMOBJS) $(COBJS)
	@echo 
	@echo --- Linking ---
	$(LD) $(LDFLAGS) -o $(PROG) $(ASMOBJS) $(COBJS)
	chmod a-x $(PROG)
	@echo 
	@echo --- Finished making $(PROG) ---
	@echo 
$(LDSCRIPT): $(PPLDSCRIPT)
	@echo 
	@echo --- Preprocessing linker script ---
	$(PP) $(PPFLAGS) -o $@ $<
$(ASMOBJS): $(BUILDDIR)/%.o: $(ASMSRCDIR)/%.S
	@echo 
	@echo --- Compiling ASM source ---
	$(ASM) $(ASMFLAGS) -o $@ $<
$(COBJS): $(BUILDDIR)/%.o: $(CSRCDIR)/%.c
	@echo 
	@echo --- Compiling C source ---
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@echo 
	@echo --- Cleaning $(BUILDDIR) ---
	rm -rf $(BUILDDIR)/*
	rm -f $(PROG)
	@echo 

report:
	@echo 
	@echo --- Source makefile report ---
	@echo 
	@echo Source directory = $(SRCDIR)
	@echo ASM sources = $(ASMSRCS)
	@echo C sources = $(CSRCS)
	@echo 
	@echo Build directory = $(BUILDDIR)
	@echo Object files = $(notdir $(ASMOBJS)) $(notdir $(COBJS))
	@echo 
	@echo Preprocessed linker script = $(PPLDSCRIPT)
	@echo Processed linker script = $(LDSCRIPT)
	@echo 
	@echo Kernel binary = $(PROG)
	@echo 
	@echo --- End of source makefile report ---
	@echo 

include:
	@echo 
	@echo --- Preprocessing sources ---
	$(foreach NAME,$(ASMSRCS),$(ASM) $(ASMFLAGS) -E -o $(BUILDDIR)/$(NAME:.S=.i) $(ASMSRCDIR)/$(NAME);)
	$(foreach NAME,$(CSRCS),$(CC) $(CFLAGS) -E -o $(BUILDDIR)/$(NAME:.c=.i) $(CSRCDIR)/$(NAME);)
	$(PP) $(PPFLAGS) -o $(LDSCRIPT) $(PPLDSCRIPT)
	@echo 
	@echo --- Finished preprocessing sources ---
	@echo 

depend: $(addprefix $(ASMSRCDIR)/,$(ASMSRCS)) $(addprefix $(CSRCDIR)/,$(CSRCS))
	$(CC) -M $(CFLAGS) $^ > $@
include depend

