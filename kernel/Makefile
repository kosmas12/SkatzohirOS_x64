SRCDIR := src
OBJDIR := objs
BUILDDIR := bin

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC = $(call rwildcard,$(SRCDIR),*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)

LDSCRIPT = kernel.ld
CC = clang
LD = ld.lld
CFLAGS = -ffreestanding -fshort-wchar -target x86_64-pc-none -Wall -Wextra -pedantic
LDFLAGS = -T $(LDSCRIPT) -static -Bsymbolic -nostdlib

kernel: $(OBJS) setup link

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@ echo Compiling $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

link: setup
	@ echo Linking final executable
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

setup:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(OBJDIR)
