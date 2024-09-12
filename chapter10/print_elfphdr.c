/*****************************************************************************
  Title          : print_elfphdr.c
  Author         : Stewart Weiss
  Created on     : March 20, 2024
  Description    : Prints a table of ELF program headers
  Purpose        : To show how to parse the ELF header data.
  Usage          : print_elfphdr  <executable-file>
  Build with     : gcc -Wall -g -o print_elfphdr -I../include/ -L../lib  \
                     print_elfphdr.c -lspl




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

char*  permission(int flag)
{
    static char permstr[4];
    memset(permstr,' ',4);
    if ( flag & 4)  permstr[0] = 'R';
    if ( flag & 1 ) permstr[2] = 'E';
    if ( flag & 2)  permstr[1] = 'W';
    permstr[4] = '\0';
    return permstr;
}

char* phdr_type(long long int type)
{
    switch (type ) {
      case PT_NULL: return "NULL";
      case PT_LOAD: return "LOAD";
      case PT_DYNAMIC: return "DYNAMIC";
      case PT_INTERP: return "INTERP";
      case PT_NOTE: return "NOTE";
      case PT_SHLIB: return "SHLIB";
      case PT_PHDR: return "PHDR";
      case PT_TLS: return "TLS";
      case PT_NUM: return "NUM";
      case PT_LOOS: return "LOOS";
      case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
      case PT_GNU_STACK: return "GNU_STACK";
      case PT_GNU_RELRO: return "GNU_RELRO";
      case PT_GNU_PROPERTY: return "GNU_PROPERTY";
      case PT_LOSUNW: return "LOSUNW";
      case PT_SUNWSTACK: return "SUNWSTACK";
      case PT_HISUNW: return "HISUNW";
      case PT_LOPROC: return "LOPROC";
      case PT_HIPROC: return "HIPROC";
      default: return "";
    }
}

char* get_data(int fd, long long offset, long long size)
{
    static char buffer[256];
    lseek(fd, offset, SEEK_SET);
    if ( -1 == read(fd, buffer, size ) )
        fatal_error(errno, "read");
    return buffer;
}

Elf64_Phdr*  read_ph64table( int fd, Elf64_Off offset, Elf64_Half numhdrs )
{
    Elf64_Phdr* hdr = calloc(numhdrs, sizeof(Elf64_Phdr));
    lseek(fd, offset, SEEK_SET);
    read(fd, hdr, numhdrs* sizeof(Elf64_Phdr));
    return hdr;
}


void print_progheader64( int fd, Elf64_Phdr* hdr )
{
    printf("  %-15s", phdr_type(hdr->p_type));
    printf("%#08llx ", (long long) hdr->p_offset);
    printf("%#018lx ", hdr->p_vaddr);
    printf("%#018llx ", (long long) hdr->p_paddr);
    printf("%#08lx ", hdr->p_filesz);
    printf("%#08llx ", (long long) hdr->p_memsz);
    printf("%-4s", permission(hdr->p_flags));
    printf("%#llx \n", (long long) hdr->p_align);
    if ( hdr->p_type == PT_INTERP ) {
        printf("      [Requesting program interpreter:");
        printf("%s]\n", get_data(fd, hdr->p_offset, hdr->p_filesz));
    }
}

int main( int argc, char* argv[])
{
    int  fd;
    ssize_t  nbytes;
    unsigned char ident[EI_NIDENT];
    int class, i;
    Elf64_Ehdr*  elf_header64 = NULL;
    Elf64_Phdr*  phdr64   = NULL;

    if ( argc < 2 )
       usage_error("expecting executable file");
    if ( (fd = open(argv[1], O_RDONLY)) == -1 )
        fatal_error(errno, "while opening input file");
    if ( (nbytes = read(fd, ident, EI_NIDENT)) != EI_NIDENT )
        fatal_error(errno, "reading file");

    lseek(fd, 0, SEEK_SET);
    class = ident[EI_CLASS];
    if ( class != ELFCLASS64 )
        fatal_error(-1, "Expecting a 64-bit ELF file\n");

    if ( NULL == (elf_header64 = calloc(1, sizeof(Elf64_Ehdr)) ) )
        fatal_error(errno, "calloc");
    if ( ( nbytes = read(fd, elf_header64, sizeof(Elf64_Ehdr)) ) == -1  )
        fatal_error(errno, "read");
    printf("There are %d program headers, starting at offset %lu.\n\n",
             elf_header64->e_phnum, elf_header64->e_phoff);
    printf("Program Headers:\n");
    printf("  Type           Offset   VirtAddr           PhysAddr"
           "           FileSiz  MemSiz   Flg Align\n");
    phdr64 = read_ph64table(fd, elf_header64->e_phoff, elf_header64->e_phnum);
    for ( i = 0; i < elf_header64->e_phnum; i++ )
        print_progheader64(fd, &(phdr64[i]));
    return 0;
}







