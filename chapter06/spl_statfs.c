/*****************************************************************************
  Title          : spl_statfs.c
  Author         : Stewart Weiss
  Created on     : September 10, 2023
  Description    : Displays filesystem status, similar to "stat -f"
  Purpose        : To interact with the filesystem API
  Usage          : spl_statfs file
  Build with     : gcc -Wall -g -I ../include spl_statfs.c -o spl_statfs \
                   -L../lib -lspl

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/


#define _GNU_SOURCE
#include <sys/stat.h>
#include <errno.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>
#include "common_hdrs.h"

/*--------------------------------------------------------------------------*/

/** fstype2name() returns the human-readable string represented by the
     statfs_buffer's f_type member.                                         */
char* fstype2name( struct statfs statfs_buffer )
{
    switch (statfs_buffer.f_type) {
        case  0x5A3C69F0 : return "aafs";
        case  0x61636673 : return "acfs";
        case  0xADF5     : return "adfs";
        case  0xADFF     : return "affs";
        case  0x5346414F : return "afs";
        case  0x09041934 : return "anon-inode FS";
        case  0x61756673 : return "aufs";
        case  0x0187     : return "autofs";
        case  0x13661366 : return "balloon-kvm-fs";
        case  0x42465331 : return "befs";
        case  0x62646576 : return "bdevfs";
        case  0x1BADFACE : return "bfs";
        case  0x6C6F6F70 : return "binderfs";
        case  0xCAFE4A11 : return "bpf_fs";
        case  0x42494E4D : return "binfmt_misc";
        case  0x9123683E : return "btrfs";
        case  0x73727279 : return "btrfs_test";
        case  0x00C36400 : return "ceph";
        case  0x0027E0EB : return "cgroupfs";
        case  0x63677270 : return "cgroup2fs";
        case  0xFF534D42 : return "cifs";
        case  0x73757245 : return "coda";
        case  0x012FF7B7 : return "coh";
        case  0x62656570 : return "configfs";
        case  0x28CD3D45 : return "cramfs";
        case  0x453DCD28 : return "cramfs-wend";
        case  0x64646178 : return "daxfs";
        case  0x64626720 : return "debugfs";
        case  0x1373     : return "devfs";
        case  0x1CD1     : return "devpts";
        case  0x444D4142 : return "dma-buf-fs";
        case  0xF15F     : return "ecryptfs";
        case  0xDE5E81E4 : return "efivarfs";
        case  0x00414A53 : return "efs";
        case  0xE0F5E1E2 : return "erofs";
        case  0x45584653 : return "exfs";
        case  0x5DF5     : return "exofs";
        case  0x137D     : return "ext";
        case  0xEF53     : return "ext2/ext3";
        case  0xEF51     : return "ext2";
        case  0xF2F52010 : return "f2fs";
        case  0x4006     : return "fat";
        case  0x19830326 : return "fhgfs";
        case  0x65735546 : return "fuseblk";
        case  0x65735543 : return "fusectl";
        case  0x0BAD1DEA : return "futexfs";
        case  0x01161970 : return "gfs/gfs2";
        case  0x47504653 : return "gpfs";
        case  0x4244     : return "hfs";
        case  0x482B     : return "hfs+";
        case  0x4858     : return "hfsx";
        case  0x00C0FFEE : return "hostfs";
        case  0xF995E849 : return "hpfs";
        case  0x958458F6 : return "hugetlbfs";
        case  0x11307854 : return "inodefs";
        case  0x013111A8 : return "ibrix";
        case  0x2BAD1DEA : return "inotifyfs";
        case  0x9660     : return "isofs";
        case  0x4004     : return "isofs";
        case  0x4000     : return "isofs";
        case  0x07C0     : return "jffs";
        case  0x72B6     : return "jffs2";
        case  0x3153464A : return "jfs";
        case  0x6B414653 : return "k-afs";
        case  0xC97E8168 : return "logfs";
        case  0x0BD00BD0 : return "lustre";
        case  0x5346314D : return "m1fs";
        case  0x137F     : return "minix";
        case  0x138F     : return "minix (30 char.)";
        case  0x2468     : return "minix v2";
        case  0x2478     : return "minix v2 (30 char.)";
        case  0x4D5A     : return "minix3";
        case  0x19800202 : return "mqueue";
        case  0x4D44     : return "msdos";
        case  0x564C     : return "novell";
        case  0x6969     : return "nfs";
        case  0x6E667364 : return "nfsd";
        case  0x3434     : return "nilfs";
        case  0x6E736673 : return "nsfs";
        case  0x5346544E : return "ntfs";
        case  0x9FA1     : return "openprom";
        case  0x7461636F : return "ocfs2";
        case  0x794C7630 : return "overlayfs";
        case  0xAAD7AAEA : return "panfs";
        case  0x50495045 : return "pipefs";
        case  0xC7571590 : return "ppc-cmm-fs";
        case  0x7C7C6673 : return "prl_fs";
        case  0x9FA0     : return "proc";
        case  0x6165676C : return "pstorefs";
        case  0x002F     : return "qnx4";
        case  0x68191122 : return "qnx6";
        case  0x858458F6 : return "ramfs";
        case  0x07655821 : return "rdt";
        case  0x52654973 : return "reiserfs";
        case  0x7275     : return "romfs";
        case  0x67596969 : return "rpc_pipefs";
        case  0x5DCA2DF5 : return "sdcardfs";
        case  0x73636673 : return "securityfs";
        case  0xF97CFF8C : return "selinux";
        case  0x43415D53 : return "smackfs";
        case  0x517B     : return "smb";
        case  0xFE534D42 : return "smb2";
        case  0xBEEFDEAD : return "snfs";
        case  0x534F434B : return "sockfs";
        case  0x73717368 : return "squashfs";
        case  0x62656572 : return "sysfs";
        case  0x012FF7B6 : return "sysv2";
        case  0x012FF7B5 : return "sysv4";
        case  0x01021994 : return "tmpfs";
        case  0x74726163 : return "tracefs";
        case  0x24051905 : return "ubifs";
        case  0x15013346 : return "udf";
        case  0x00011954 : return "ufs";
        case  0x54190100 : return "ufs";
        case  0x9FA2     : return "usbdevfs";
        case  0x01021997 : return "v9fs";
        case  0xBACBACBC : return "vmhgfs";
        case  0xA501FCF5 : return "vxfs";
        case  0x565A4653 : return "vzfs";
        case  0x53464846 : return "wslfs";
        case  0xABBA1974 : return "xenfs";
        case  0x012FF7B4 : return "xenix";
        case  0x58465342 : return "xfs";
        case  0x012FD16D : return "xia";
        case  0x0033     : return "z3fold";
        case  0x2FC12FC1 : return "zfs";
        case  0x58295829 : return "zsmallocfs";
        default: return "unknown";
    }
}

/** print_moptions(flag) prints descriptions of the enabled mount options
    bitwise-or-ed into flag.                                                */
void print_moptions( unsigned long flag )
{
    printf("Mount flags:\n");
    if (flag & ST_RDONLY )
        printf(" Mount read-only.  \n");
    if (flag & ST_NOSUID )
        printf(" Ignore suid and sgid bits.  \n");
    if (flag & ST_NODEV  )
        printf(" Disallow access to  device special files.  \n");
    if (flag & ST_NOEXEC  )
        printf(" Disallow program execution.  \n");
    if (flag & ST_SYNCHRONOUS )
        printf(" Writes are synced at once.  \n");
    if (flag & ST_MANDLOCK  )
        printf(" Allow mandatory locks on an FS.  \n");
    if (flag & ST_WRITE )
        printf(" Write on file/directory/symlink.  \n");
    if (flag & ST_APPEND  )
        printf(" Append-only file.  \n");
    if (flag & ST_IMMUTABLE  )
        printf(" Immutable file.  \n");
    if (flag & ST_NOATIME  )
        printf(" Do not update access times.  \n");
    if (flag & ST_NODIRATIME  )
         printf(" Do not update directory access times.  \n");
    if (flag & ST_RELATIME   )
         printf(" Update atime relative to mtime/ctime.  \n");
}

/** print_stat(sb, ftype) prints the available members of the statvfs
    structure, sb, as well as the filesystem type, fstype.                 */
void print_stat(struct statvfs statvfs_buf, char* fstype)
{
    unsigned int low = statvfs_buf.f_fsid & 0xFFFFFFFF;
    unsigned int high = ( statvfs_buf.f_fsid  >> 32)& 0xFFFFFFFF;
    printf("    ID: %08x%08x", low, high);
    printf(" Namelen: %-8lu", statvfs_buf.f_namemax);
    printf("Type: %s\n", fstype);
    printf("Block size: %-11lu", (unsigned long) statvfs_buf.f_bsize);
    printf("Fundamental block size: %lu\n", statvfs_buf.f_frsize);
    printf("Blocks: Total: %-10lu", statvfs_buf.f_blocks);
    printf("Free: %-11lu", statvfs_buf.f_bfree);
    printf(" Available: %lu\n",statvfs_buf.f_bavail);
    printf("Inodes: Total: %-10lu", statvfs_buf.f_files);
    printf("Free: %lu\n", statvfs_buf.f_ffree);
}

int main(int argc, char **argv)
{
    struct statvfs statvfs_buffer; /* statvfs structure filled by statvfs()*/
    struct statfs  statfs_buffer;  /* statfs structure filled by statfs()  */
    char           mssge[128];     /* string to store error messages       */

    /* If no file arguments, print a usage message. */
    if (argc < 2) {
        sprintf(mssge,"usage: %s file \n", basename(argv[0]));
        usage_error(mssge);
    }
    printf("  File: \"%s\"\n", argv[1]);
    errno = 0;
    if ( statvfs(argv[1],  &statvfs_buffer) < 0 ) {
         sprintf(mssge, "Could not statvfs file %s\n", argv[1]);
         fatal_error(errno, mssge);
    }
    errno = 0;
    if ( statfs(argv[1],  &statfs_buffer) < 0 ) {
         sprintf(mssge,"Could not statfs file %s\n", argv[1]);
         fatal_error(errno, mssge);
    }
    print_stat(statvfs_buffer, fstype2name(statfs_buffer));
    return 0;
}
