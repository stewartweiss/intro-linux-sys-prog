/*****************************************************************************
  Title          : printelf.c
  Author         : Stewart Weiss
  Created on     : March 20, 2024
  Description    : Equivalent to readelf -h; it prints the ELF header
  Purpose        : To show how to parse the ELF header data.
  Usage          : printelf  <executable-file>
  Build with     : gcc -Wall -g -o printelf -I../include/ -L../lib  \
                     printelf.c -lspl

  Note:
  There will be some differences in Type and Machine fields since these are
  implementation-dependent names.


******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"
#include <elf.h>
#include "elf_utils.c"


char * f_type (unsigned e_type)
{
  switch (e_type)
    {
    case ET_NONE: return "NONE (None)";
    case ET_REL:  return "REL (Relocatable file)";
    case ET_EXEC: return "EXEC (Executable file)";
    case ET_DYN:  return "DYN ";
    case ET_CORE: return "CORE (Core file)";
    default:     return "<unknown>";
    }
}

void print_elfhdr(Elf64_Ehdr *hdr)
{
    printf("%-38s%s\n",elfhdr_label[E_TYPE],f_type(hdr->e_type));
    printf("%-38s%s\n",elfhdr_label[E_MACHINE],em_label[hdr->e_machine]);
    printf("%-38s%s\n",elfhdr_label[E_VERSION],em_label[hdr->e_version]);
    printf("%-38s0x%lx\n",elfhdr_label[E_ENTRY],hdr->e_entry);
    printf("%-38s%ld\n",elfhdr_label[E_PHOFF],hdr->e_phoff);
    printf("%-38s%ld\n",elfhdr_label[E_SHOFF],hdr->e_shoff);
    printf("%-38s0x%x\n",elfhdr_label[E_FLAGS],hdr->e_flags);
    printf("%-38s%d\n",elfhdr_label[E_EHSIZE],hdr->e_ehsize);
    printf("%-38s%d (bytes)\n",elfhdr_label[E_PHENTSIZE],hdr->e_phentsize);
    printf("%-38s%d\n",elfhdr_label[E_PHNUM],hdr->e_phnum);
    printf("%-38s%d (bytes)\n",elfhdr_label[E_SHENTSIZE],hdr->e_shentsize);
    printf("%-38s%d\n",elfhdr_label[E_SHNUM],hdr->e_shnum);
    printf("%-38s%d\n",elfhdr_label[E_SHSTRNDX],hdr->e_shstrndx);
}

/* Parse the first 4 bytes of the file, which contain the magic number. */
BOOL elf_check_ident( unsigned char ident[])
{
    if ( ident[EI_MAG0] != ELFMAG0) {
        printf( "ELF Header EI_MAG0 incorrect.\n");
        return FALSE;
    }
    if ( ident[EI_MAG1] != ELFMAG1) {
        printf( "ELF Header EI_MAG1 incorrect.\n");
        return FALSE;
    }
    if ( ident[EI_MAG2] != ELFMAG2) {
        printf( "ELF Header EI_MAG2 incorrect.\n");
        return FALSE;
    }
    if ( ident[EI_MAG3] != ELFMAG3) {
        printf( "ELF Header EI_MAG3 incorrect.\n");
        return FALSE;
    }
    return TRUE;
}

Elf32_Phdr*  get_proghdr32( int fd, uint32_t offset )
{
    Elf32_Phdr* hdr = calloc(1, sizeof(Elf32_Phdr));

    lseek(fd, offset, SEEK_SET);
    read(fd, hdr, sizeof(Elf32_Phdr));
    return hdr;
}


Elf64_Phdr*  get_proghdr64( int fd, uint64_t offset )
{
    Elf64_Phdr* hdr = calloc(1, sizeof(Elf64_Phdr));
    lseek(fd, offset, SEEK_SET);
    read(fd, hdr, sizeof(Elf64_Phdr));
    return hdr;
}

void print_progheader64(Elf64_Phdr* hdr )
{

    printf("%d  ", hdr->p_type);
    printf("%#015llx  ", (long long) hdr->p_offset);
    printf("%#015lx ", hdr->p_vaddr);
    printf("%#015llx ", (long long) hdr->p_paddr);
    printf("%#015lx ", hdr->p_filesz);
    printf("%#015llx ", (long long) hdr->p_memsz);
    printf("%4d ", hdr->p_flags);
    printf("%#llx \n", (long long) hdr->p_align);
}


int main( int argc, char *argv[])
{
    int  fd;
    ssize_t  nbytes;
    unsigned char ident[EI_NIDENT];
    int class;
    int data_encoding;

    Elf32_Ehdr*  header32 = NULL;
    Elf64_Ehdr*  header64 = NULL;


    if ( argc < 2 )
       usage_error("expecting executable file");

    if ( (fd = open(argv[1], O_RDONLY)) == -1 )
        fatal_error(errno, "while opening input file");

    if ( (nbytes = read(fd, ident, EI_NIDENT)) != EI_NIDENT )
        fatal_error(errno, "reading file");

    if  ( ! elf_check_ident(ident) )
        printf("Not an ELF file\n");

    printf("ELF Header:\n  Magic:  ");
    for ( int i = 0; i < EI_NIDENT; i++ )
        printf(" %02x", ident[i]);
    printf("\n");

    class = ident[EI_CLASS];
    switch( class ) {
        case ELFCLASS32:
            printf("%-38s%s\n","  Class:","ELF32");
            if ( NULL == (header32 = calloc(1, sizeof(Elf32_Ehdr)) ) )
                fatal_error(errno, "malloc");
            break;
        case ELFCLASS64:
            printf("%-38s%s\n","  Class:","ELF64");
            if ( NULL == (header64 = calloc(1, sizeof(Elf64_Ehdr)) ) )
                fatal_error(errno, "malloc");
            break;
        case ELFCLASSNONE:
            printf("%-38s%s\n","  Class:","None");  break;
    }

    data_encoding = ident[EI_DATA];
    switch (data_encoding ) {
        case ELFDATA2LSB:
            printf("%-38s%s\n","  Data:","2's complement, little-endian (LSB)");
            break;
        case ELFDATA2MSB:
            printf("%-38s%s\n","  Data:","2's complement, big-endian (MSB)");
            break;
        case ELFDATANONE:
            printf("%-38s%s\n","  Data:","bad format");
            fatal_error(-1, "Corrupted file\n");
    }

    if ( ident[EI_VERSION] != EV_CURRENT )
        fatal_error(-1, "Invalid ELF version\n");
    printf("%-38s%s\n","  Version:",   "Current version");

    switch ( ident[EI_OSABI] ) {
    case ELFOSABI_NONE: printf("%-38s%s\n","  OS/ABI:","UNIX System V ABI"); break;
    case ELFOSABI_HPUX: printf("%-38s%s\n","  OS/ABI:","HP-UX "); break;
    case ELFOSABI_NETBSD: printf("%-38s%s\n","  OS/ABI:","NetBSD.  "); break;
    case ELFOSABI_GNU: printf("%-38s%s\n","  OS/ABI:","Object uses GNU ELF extensions.  "); break;
    case ELFOSABI_SOLARIS: printf("%-38s%s\n","  OS/ABI:","Sun Solaris.  "); break;
    case ELFOSABI_AIX: printf("%-38s%s\n","  OS/ABI:","IBM AIX."); break;
    case ELFOSABI_IRIX: printf("%-38s%s\n","  OS/ABI:","SGI Irix."); break;
    case ELFOSABI_FREEBSD: printf("%-38s%s\n","  OS/ABI:","  FreeBSD.  "); break;
    case ELFOSABI_TRU64: printf("%-38s%s\n","  OS/ABI:","  Compaq TRU64 UNIX.  "); break;
    case ELFOSABI_MODESTO: printf("%-38s%s\n","  OS/ABI:","  Novell Modesto.  "); break;
    case ELFOSABI_OPENBSD: printf("%-38s%s\n","  OS/ABI:","  OpenBSD.  "); break;
    case ELFOSABI_ARM_AEABI: printf("%-38s%s\n","  OS/ABI:","  ARM EABI "); break;
    case ELFOSABI_ARM: printf("%-38s%s\n","  OS/ABI:","  ARM "); break;
    case ELFOSABI_STANDALONE: printf("%-38s%s\n","  OS/ABI:","  Standalone (embedded) application "); break;
    }

    printf("%-38s%d\n","  ABI Version:",ident[EI_ABIVERSION]);

    /* Now load the ELF header */

    lseek(fd, 0, SEEK_SET);
    if ( class == ELFCLASS64 ) {
        if ( ( nbytes = read(fd, header64, sizeof(Elf64_Ehdr)) ) == -1  )
            fatal_error(errno, "read");

    }
    else if ( class == ELFCLASS32 ) {
        if ( ( nbytes = read(fd, header32, sizeof(Elf32_Ehdr)) ) == -1  )
            fatal_error(errno, "read");

    }
    em_machines_init();
    init_elf_header_labels();
    print_elfhdr(header64);

    return 0;
}

