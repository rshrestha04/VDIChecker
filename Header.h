/* 
 * File:   Header.h
 * Author: Ronish
 *
 * Created on April 14, 2019, 4:40 PM
 */

#ifndef HEADER_H
#define	HEADER_H

struct __attribute__((packed)) vdiHeader{
   unsigned int diskImage[16];
  unsigned int imageSignature;
  unsigned int version;
  unsigned int headerSize;
  unsigned int imageType;
  unsigned int imageFlags;
  unsigned int imageDescription[64];
  unsigned int offsetBlocks;
  unsigned int offsetData;
  unsigned int cylinders;
  unsigned int heads;
  unsigned int sectors;
  unsigned int sectorSize;
  unsigned int unused;
  unsigned int diskSize[2];
  unsigned int blockSize;
  unsigned int blockExtraData;
  unsigned int blocksInHdd;
  unsigned int blocksAllocated;
  unsigned int UuidVdi[4];
  unsigned int UuidSnap[4];
  unsigned int UuidLink[4];
  unsigned int UuidParent[4];
  unsigned int garbage[14]; 
};




#endif	/* HEADER_H */

