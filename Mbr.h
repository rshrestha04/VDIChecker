/* 
 * File:   Mbr.h
 * Author: Ronish
 *
 * Created on April 15, 2019, 11:25 PM
 */

#ifndef MBR_H
#define	MBR_H

const unsigned short BOOT_SECTOR_MAGIC = 0xaa55;

struct __attribute__ ((packed)) PartitionEntry{
   unsigned char unused_0[4];
   unsigned char type;
   unsigned char unused_1[3];
   unsigned int sector_1;
   unsigned int nSectors;

	};

struct __attribute__ ((packed)) BootSector{
  unsigned char unused_0[0x1be];
  PartitionEntry partitionTable[4];
  unsigned short magic;

	};


#endif	/* MBR_H */

