/* 
 * File:   vdiReader.h
 * Author: Ronish
 *
 * Created on April 5, 2019, 12:16 AM
 */

#ifndef VDIREADER_H
#define	VDIREADER_H

#include "typedefine.h"
#include <string>
#include <sys/types.h>
#include "Header.h"
#include "VdiHead.h"
#include "Mbr.h"
#include "superBlock.h"
#include "group_descriptor.h"
#include "Inode.h"
#include "DirectoryEntry.h"

/*struct __attribute__((packed)) vdiHeader{
            char title[16];
                u32 magic;
                u16 majorVer, minorVer;
                u32 headerSize, imageType, imageFlags;
                char desc[256];
                u32 offsetPages, offsetData, nCylinders,
                    nHeads, nSectors, sectorSize, unused0;
                u64 diskSize;
                u32 pageSize, pageExtra, totalPages, pagesAllocated;
                u8 thisUUID[16], lastSnapUUID[16], linkUUID[16], parentUUID[16],
                    padding1[56]; 
        };
        
*/


//class vdiReader{
    //public:
        int VdiOpen(vdiFile* fl, char* name);
        void VdiClose( vdiFile* fl);
        int VdiRead (vdiFile* fl, void *buffer, ssize_t length);
        int VdiWrite (vdiFile* fl, void *buffer, int length);
        void VdiSeek(vdiFile* fl, off_t offset);
        int ReadMasterBoot(vdiFile* fl, BootSector& bt);
        int ReadMap(vdiFile* fl, unsigned int mp[]);
        unsigned int translate(vdiFile* fl,unsigned int location, BootSector bs, unsigned int map[] );
        int ReadSuperBlock(vdiFile* fl, unsigned int SupLocation, ext2_super_block& SupBlock );
         int ReadGroupDescriptor(vdiFile* fl, int group_count, ext2_group_descriptor grp_desc[], unsigned int location);
         inode ReadInode (vdiFile* fl, inode& tinode , unsigned int inodeLocation);
         void computeIndex(unsigned int blockNum, unsigned int blockSize, int& directNumber, int& indirectIndex, int& double_index,int& triple_index);
          int readBlock(inode inode, unsigned int blockNum, unsigned int blockSize, vdiFile* f, BootSector bootSector, unsigned int vdiMap[], unsigned char* buf);
         
    

   /* private:     
       
        
       struct vdiHeader* head;
        off_t cursor;
        s32 isOpen;
     */            
        
//};
 
        

    




#endif	/* VDIREADER_H */

