

char* em_label[EM_NUM];

#define  E_TYPE       5
#define  E_MACHINE    6
#define  E_VERSION    7
#define  E_ENTRY    8
#define  E_PHOFF    9
#define  E_SHOFF    10
#define  E_FLAGS    11
#define  E_EHSIZE    12
#define  E_PHENTSIZE    13
#define  E_PHNUM    14
#define  E_SHENTSIZE    15
#define  E_SHNUM    16
#define  E_SHSTRNDX    17
#define  E_NUMMEMBERS  18

char* elfhdr_label[E_NUMMEMBERS];

void init_elf_header_labels()
{
    int i;
    for ( i = 0; i < E_NUMMEMBERS ; i++)
        elfhdr_label[i] = malloc(64);

    strcpy( elfhdr_label[E_TYPE],  "Type:");
    strcpy( elfhdr_label[E_MACHINE], "Machine:");
    strcpy( elfhdr_label[E_VERSION], "Version:");
    strcpy( elfhdr_label[E_ENTRY], "Entry point address:");
    strcpy( elfhdr_label[E_PHOFF], "Start of program headers:");
    strcpy( elfhdr_label[E_SHOFF], "Start of section headers:");
    strcpy( elfhdr_label[E_FLAGS], "Flags:");
    strcpy( elfhdr_label[E_EHSIZE], "Size of this header:");
    strcpy( elfhdr_label[E_PHENTSIZE], "Size of program headers:");
    strcpy( elfhdr_label[E_PHNUM],     "Number of program headers:");
    strcpy( elfhdr_label[E_SHENTSIZE],     "Size of section headers:");
    strcpy( elfhdr_label[E_SHNUM],     "Number of section headers:");
    strcpy( elfhdr_label[E_SHSTRNDX],     "Section header string table index:");
}




void em_machines_init()
{
    int i;
    for ( i = 0; i < EM_NUM ; i++)
        em_label[i] = malloc(64);

    strcpy(em_label[ EM_NONE],"No machine");
    strcpy(em_label[ EM_M32],"AT&T WE 32100");
    strcpy(em_label[EM_SPARC ],"SUN SPARC");
    strcpy(em_label[ EM_386],"Intel 80386");
    strcpy(em_label[ EM_68K],"Motorola m68k family");
    strcpy(em_label[EM_88K],"Motorola m88k family");
    strcpy(em_label[EM_IAMCU ],"Intel MCU");
    strcpy(em_label[EM_860],"Intel 80860");
    strcpy(em_label[ EM_MIPS],"MIPS R3000 big-endian ");
    strcpy(em_label[EM_S370],"IBM System/370");
    strcpy(em_label[EM_MIPS_RS3_LE],"MIPS R3000 little-endian ");
    strcpy(em_label[EM_PARISC],"HPPA ");
    strcpy(em_label[EM_VPP500],"Fujitsu VPP500 ");
    strcpy(em_label[EM_SPARC32PLUS],"Sun's v8plus");
    strcpy(em_label[EM_960],"Intel 80960 ");
    strcpy(em_label[EM_PPC],"PowerPC ");
    strcpy(em_label[EM_PPC64],"PowerPC 64-bit ");
    strcpy(em_label[EM_S390],"IBM S390 ");
    strcpy(em_label[EM_SPU],"IBM SPU/SPC ");
    strcpy(em_label[EM_V800],"NEC V800 series ");
    strcpy(em_label[EM_FR20],"Fujitsu FR20 ");
    strcpy(em_label[EM_RH32],"TRW RH-32 ");
    strcpy(em_label[EM_RCE],"Motorola RCE ");
    strcpy(em_label[EM_ARM],"ARM ");
    strcpy(em_label[EM_FAKE_ALPHA],"Digital Alpha ");
    strcpy(em_label[EM_SH],"Hitachi SH ");
    strcpy(em_label[EM_SPARCV9],"SPARC v9 64-bit ");
    strcpy(em_label[EM_TRICORE],"Siemens Tricore ");
    strcpy(em_label[EM_ARC],"Argonaut RISC Core ");
    strcpy(em_label[EM_H8_300],"Hitachi H8/300 ");
    strcpy(em_label[EM_H8_300H],"Hitachi H8/300H ");
    strcpy(em_label[EM_H8S],"Hitachi H8S ");
    strcpy(em_label[EM_H8_500],"Hitachi H8/500 ");
    strcpy(em_label[EM_IA_64],"Intel Merced ");
    strcpy(em_label[EM_MIPS_X],"Stanford MIPS-X ");
    strcpy(em_label[EM_COLDFIRE],"Motorola Coldfire ");
    strcpy(em_label[EM_68HC12],"Motorola M68HC12 ");
    strcpy(em_label[EM_MMA],"Fujitsu MMA Multimedia Accelerator ");
    strcpy(em_label[EM_PCP],"Siemens PCP ");
    strcpy(em_label[EM_NCPU],"Sony nCPU embeeded RISC ");
    strcpy(em_label[EM_NDR1],"Denso NDR1 microprocessor ");
    strcpy(em_label[EM_STARCORE],"Motorola Start*Core processor ");
    strcpy(em_label[EM_ME16],"Toyota ME16 processor ");
    strcpy(em_label[EM_ST100],"STMicroelectronic ST100 processor ");
    strcpy(em_label[EM_TINYJ],"Advanced Logic Corp. Tinyj emb.fam ");
    strcpy(em_label[EM_X86_64],"AMD x86-64 architecture ");
    strcpy(em_label[EM_PDSP],"Sony DSP Processor ");
    strcpy(em_label[EM_PDP10],"Digital PDP-10 ");
    strcpy(em_label[EM_PDP11],"Digital PDP-11 ");
    strcpy(em_label[EM_FX66],"Siemens FX66 microcontroller ");
    strcpy(em_label[EM_ST9PLUS],"STMicroelectronics ST9+ 8/16 mc ");
    strcpy(em_label[EM_ST7],"STmicroelectronics ST7 8 bit mc ");
    strcpy(em_label[EM_68HC16],"Motorola MC68HC16 microcontroller ");
    strcpy(em_label[EM_68HC11],"Motorola MC68HC11 microcontroller ");
    strcpy(em_label[EM_68HC08],"Motorola MC68HC08 microcontroller ");
    strcpy(em_label[EM_68HC05],"Motorola MC68HC05 microcontroller ");
    strcpy(em_label[EM_SVX],"Silicon Graphics SVx ");
    strcpy(em_label[EM_ST19],"STMicroelectronics ST19 8 bit mc ");
    strcpy(em_label[EM_VAX],"Digital VAX ");
    strcpy(em_label[EM_CRIS],"Axis Communications 32-bit emb.proc ");
    strcpy(em_label[EM_JAVELIN],"Infineon Technologies 32-bit emb.proc ");
    strcpy(em_label[EM_FIREPATH],"Element 14 64-bit DSP Processor ");
    strcpy(em_label[EM_ZSP],"LSI Logic 16-bit DSP Processor ");
    strcpy(em_label[EM_MMIX],"Donald Knuth's educational 64-bit proc ");
    strcpy(em_label[EM_HUANY],"Harvard University machine-independent object files ");
    strcpy(em_label[EM_PRISM],"SiTera Prism ");
    strcpy(em_label[EM_AVR],"Atmel AVR 8-bit microcontroller ");
    strcpy(em_label[EM_FR30],"Fujitsu FR30 ");
    strcpy(em_label[EM_D10V],"Mitsubishi D10V ");
    strcpy(em_label[EM_D30V],"Mitsubishi D30V ");
    strcpy(em_label[EM_V850],"NEC v850 ");
    strcpy(em_label[EM_M32R],"Mitsubishi M32R ");
    strcpy(em_label[EM_MN10300],"Matsushita MN10300 ");
    strcpy(em_label[EM_MN10200],"Matsushita MN10200 ");
    strcpy(em_label[EM_PJ],"picoJava ");
    strcpy(em_label[EM_OPENRISC],"OpenRISC 32-bit embedded processor ");
    strcpy(em_label[EM_ARC_COMPACT],"ARC International ARCompact ");
    strcpy(em_label[EM_XTENSA],"Tensilica Xtensa Architecture ");
    strcpy(em_label[EM_VIDEOCORE],"Alphamosaic VideoCore ");
    strcpy(em_label[EM_TMM_GPP],"Thompson Multimedia General Purpose Proc ");
    strcpy(em_label[EM_NS32K],"National Semi. 32000 ");
    strcpy(em_label[EM_TPC],"Tenor Network TPC ");
    strcpy(em_label[EM_SNP1K],"Trebia SNP 1000 ");
    strcpy(em_label[EM_ST200],"STMicroelectronics ST200 ");
    strcpy(em_label[EM_IP2K],"Ubicom IP2xxx ");
    strcpy(em_label[EM_MAX],"MAX processor ");
    strcpy(em_label[EM_CR],"National Semi. CompactRISC ");
    strcpy(em_label[EM_F2MC16],"Fujitsu F2MC16 ");
    strcpy(em_label[EM_MSP430],"Texas Instruments msp430 ");
    strcpy(em_label[EM_BLACKFIN],"Analog Devices Blackfin DSP ");
    strcpy(em_label[EM_SE_C33],"Seiko Epson S1C33 family ");
    strcpy(em_label[EM_SEP],"Sharp embedded microprocessor ");
    strcpy(em_label[EM_ARCA],"Arca RISC ");
    strcpy(em_label[EM_UNICORE],"PKU-Unity & MPRC Peking Uni. mc series ");
    strcpy(em_label[EM_EXCESS],"eXcess configurable cpu ");
    strcpy(em_label[EM_DXP],"Icera Semi. Deep Execution Processor ");
    strcpy(em_label[EM_ALTERA_NIOS2]," Altera Nios II ");
    strcpy(em_label[EM_CRX],"National Semi. CompactRISC CRX ");
    strcpy(em_label[EM_XGATE],"Motorola XGATE ");
    strcpy(em_label[EM_C166],"Infineon C16x/XC16x ");
    strcpy(em_label[EM_M16C],"Renesas M16C ");
    strcpy(em_label[EM_DSPIC30F],"Microchip Technology dsPIC30F ");
    strcpy(em_label[EM_CE],"Freescale Communication Engine RISC ");
    strcpy(em_label[EM_M32C],"Renesas M32C ");
    strcpy(em_label[EM_TSK3000],"Altium TSK3000 ");
    strcpy(em_label[EM_RS08],"Freescale RS08 ");
    strcpy(em_label[EM_SHARC],"Analog Devices SHARC family ");
    strcpy(em_label[EM_ECOG2],"Cyan Technology eCOG2 ");
    strcpy(em_label[EM_SCORE7],"Sunplus S+core7 RISC ");
    strcpy(em_label[EM_DSP24],"New Japan Radio (NJR) 24-bit DSP ");
    strcpy(em_label[EM_VIDEOCORE3],"Broadcom VideoCore III ");
    strcpy(em_label[EM_LATTICEMICO32],"RISC for Lattice FPGA");
    strcpy(em_label[EM_SE_C17],"Seiko Epson C17 ");
    strcpy(em_label[EM_TI_C6000],"Texas Instruments TMS320C6000 DSP ");
    strcpy(em_label[EM_TI_C2000],"Texas Instruments TMS320C2000 DSP ");
    strcpy(em_label[EM_TI_C5500],"Texas Instruments TMS320C55x DSP ");
    strcpy(em_label[EM_TI_ARP32],"Texas Instruments App. Specific RISC ");
    strcpy(em_label[EM_TI_PRU],"Texas Instruments Prog. Realtime Unit ");
    strcpy(em_label[EM_MMDSP_PLUS],"STMicroelectronics 64bit VLIW DSP ");
    strcpy(em_label[EM_CYPRESS_M8C],"Cypress M8C ");
    strcpy(em_label[EM_R32C],"Renesas R32C ");
    strcpy(em_label[EM_TRIMEDIA],"NXP Semi. TriMedia ");
    strcpy(em_label[EM_QDSP6],"QUALCOMM DSP6 ");
    strcpy(em_label[EM_8051],"Intel 8051 and variants ");
    strcpy(em_label[EM_STXP7X],"STMicroelectronics STxP7x ");
    strcpy(em_label[EM_NDS32],"Andes Tech. compact code emb. RISC ");
    strcpy(em_label[EM_ECOG1X],"Cyan Technology eCOG1X ");
    strcpy(em_label[EM_MAXQ30],"Dallas Semi. MAXQ30 mc ");
    strcpy(em_label[EM_XIMO16],"New Japan Radio (NJR) 16-bit DSP ");
    strcpy(em_label[EM_MANIK],"M2000 Reconfigurable RISC ");
    strcpy(em_label[EM_CRAYNV2],"Cray NV2 vector architecture ");
    strcpy(em_label[EM_RX],"Renesas RX ");
    strcpy(em_label[EM_METAG],"Imagination Tech. META ");
    strcpy(em_label[EM_MCST_ELBRUS],"MCST Elbrus ");
    strcpy(em_label[EM_ECOG16],"Cyan Technology eCOG16 ");
    strcpy(em_label[EM_CR16],"National Semi. CompactRISC CR16 ");
    strcpy(em_label[EM_ETPU],"Freescale Extended Time Processing Unit ");
    strcpy(em_label[EM_SLE9X],"Infineon Tech. SLE9X ");
    strcpy(em_label[EM_L10M],"Intel L10M ");
    strcpy(em_label[EM_K10M],"Intel K10M ");
    strcpy(em_label[EM_AARCH64],"ARM AARCH64 ");
    strcpy(em_label[EM_AVR32],"Amtel 32-bit microprocessor ");
    strcpy(em_label[EM_STM8],"STMicroelectronics STM8 ");
    strcpy(em_label[EM_TILE64],"Tilera TILE64 ");
    strcpy(em_label[EM_TILEPRO],"Tilera TILEPro ");
    strcpy(em_label[EM_MICROBLAZE],"Xilinx MicroBlaze ");
    strcpy(em_label[EM_CUDA],"NVIDIA CUDA ");
    strcpy(em_label[EM_TILEGX],"Tilera TILE-Gx ");
    strcpy(em_label[EM_CLOUDSHIELD],"CloudShield ");
    strcpy(em_label[EM_COREA_1ST],"KIPO-KAIST Core-A 1st gen. ");
    strcpy(em_label[EM_COREA_2ND],"KIPO-KAIST Core-A 2nd gen. ");
    strcpy(em_label[EM_ARCV2],"Synopsys ARCv2 ISA.  ");
    strcpy(em_label[EM_OPEN8],"Open8 RISC ");
    strcpy(em_label[EM_RL78],"Renesas RL78 ");
    strcpy(em_label[EM_VIDEOCORE5],"Broadcom VideoCore V ");
    strcpy(em_label[EM_78KOR],"Renesas 78KOR ");
    strcpy(em_label[EM_56800EX],"Freescale 56800EX DSC ");
    strcpy(em_label[EM_BA1],"Beyond BA1 ");
    strcpy(em_label[EM_BA2],"Beyond BA2 ");
    strcpy(em_label[EM_XCORE],"XMOS xCORE ");
    strcpy(em_label[EM_MCHP_PIC],"Microchip 8-bit PIC(r) ");
    strcpy(em_label[EM_INTELGT],"Intel Graphics Technology ");
    strcpy(em_label[EM_KM32],"KM211 KM32 ");
    strcpy(em_label[EM_KMX32],"KM211 KMX32 ");
    strcpy(em_label[EM_EMX16],"KM211 KMX16 ");
    strcpy(em_label[EM_EMX8],"KM211 KMX8 ");
    strcpy(em_label[EM_KVARC],"KM211 KVARC ");
    strcpy(em_label[EM_CDP],"Paneve CDP ");
    strcpy(em_label[EM_COGE],"Cognitive Smart Memory Processor ");
    strcpy(em_label[EM_COOL],"Bluechip CoolEngine ");
    strcpy(em_label[EM_NORC],"Nanoradio Optimized RISC ");
    strcpy(em_label[EM_CSR_KALIMBA],"CSR Kalimba ");
    strcpy(em_label[EM_Z80],"Zilog Z80 ");
    strcpy(em_label[EM_VISIUM],"Controls and Data Services VISIUMcore ");
    strcpy(em_label[EM_FT32],"FTDI Chip FT32 ");
    strcpy(em_label[EM_MOXIE],"Moxie processor ");
    strcpy(em_label[EM_AMDGPU],"AMD GPU ");
    strcpy(em_label[EM_RISCV],"RISC-V ");
    strcpy(em_label[EM_BPF],"Linux BPF -- in-kernel virtual machine ");
    strcpy(em_label[EM_CSKY], " C-SKY");
}



