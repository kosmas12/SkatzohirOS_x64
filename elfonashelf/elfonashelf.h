//
// Created by kosmas on 28/11/2022.
//

#ifndef SKATZOHIROS_X64_ELFONASHELF_H
#define SKATZOHIROS_X64_ELFONASHELF_H

#include <stdint.h>

// Magic number = 0x7F and then "ELF"
#define ELFMAG "\177ELF"
#define ELFMAG0 0x7F
#define ELFMAG1 0x45
#define ELFMAG2 0x4C
#define ELFMAG3 0x46
// Size of ELF magic number (4 bytes)
#define SELFMAG 4

#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_ABIVERSION 8
#define EI_PAD 9

#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define EV_NONE 0
#define EV_CURRENT 1

#define ELFOSABI_SYSV 0
#define ELFOSABI_NONE ELFOSABI_SYSV
#define ELFOSABI_HPUX 1
#define ELFOSABI_NETBSD 2
#define ELFOSABI_LINUX 3
#define ELFOSABI_GNUHURD 4
#define ELFOSABI_SOLARIS 6
#define ELFOSABI_AIX 7
#define ELFOSABI_IRIX 8
#define ELFOSABI_FREEBSD 9
#define ELFOSABI_TRU64 0xA
#define ELFOSABI_NOVELLMODESTO 0xB
#define ELFOSABI_OPENBSD 0xC
#define ELFOSABI_OPENVMS 0xD
#define ELFOSABI_NONSTOPKERNEL 0xE
#define ELFOSABI_AROS 0xF
#define ELFOSABI_FENIXOS 0x10
#define ELFOSABI_NUXICLOUDABI 0x11
#define ELFOSABI_OPENVOS 0x12

#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define ET_LOOS 0xFE00
#define ET_HIOS 0xFEFF
#define ET_LOPROC 0xFF00
#define ET_HIPROC 0xFFFF

#define EM_X86_64 0x3E

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_TLS 7
#define PT_LOOS 0x60000000
#define PT_HIOS 0x6FFFFFFF
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7FFFFFFF

// e_ident is 16 bytes large
#define NIDENT 16

typedef struct {
    uint8_t e_ident[NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
}Elf64_Ehdr;

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
}Elf64_Phdr;

#endif //SKATZOHIROS_X64_ELFONASHELF_H
