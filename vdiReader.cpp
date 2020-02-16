

#include "vdiReader.h"
#include "typedefine.h"
#include <fcntl.h>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include "Header.h"
#include "VdiHead.h"
#include <iomanip>
#include "superBlock.h"
#include "group_descriptor.h"
#include "DirectoryEntry.h"





 int VdiOpen(vdiFile* fl, char* name){
     
     fl->file = open(name, O_RDWR);
     if (fl->file <0 ){
        std::cout <<"file didn't open"; 
        return 0;
         
     }
     
     fl->cursor = 0;
     
     return 1;
     
    /* std::cout<< sizeof(head)<< " ";
     ssize_t readBytes = read(isOpen,&head, sizeof(head));
     if (readBytes != sizeof(head)){
         std::cout<< "error in reading"; 
     }
     std::cout<< sizeof(head)<<" " <<readBytes;*/  
    }
 
 void VdiClose(vdiFile* fl){
     
     fl->file= close(fl->file);
     if (fl->file <0 ){
         std::cout << "Error in closing";
         return;
     }
     return;
 }
 
 int VdiRead (vdiFile* fl, void *buffer, ssize_t length){
     ssize_t nread;
     lseek(fl->file,fl->cursor,SEEK_SET);
     nread= read(fl->file,buffer,length);
     
     std::cout << nread <<" ";
    if(nread!=length){
        
        std::cout <<"error reading" << " " << nread<< " "<<length;
        return -1;
    };   
    std::cout <<nread<<" ";
     return length;
     
 }
 
 int VdiWrite (vdiFile* fl, void *buffer, int length){
     if(write (fl->file, buffer,length)!=length){
         return -1;
     };
      
     return length;
 }
 
 
 void VdiSeek(vdiFile* fl, off_t offset){
     if(lseek(fl->file, offset, SEEK_SET)<0){
        std::cout<<"error seeking";
         return; 
     };
     
 }
 
 int ReadMasterBoot(vdiFile* fl, BootSector& bt){
     fl->cursor = lseek(fl->file, fl->header.offsetData, SEEK_SET);
     if (fl->cursor<0){
        std::cout<<"error seeking mbr";
         return -1; 
    };
    int nread=read(fl->file,&bt,sizeof(bt));
    if (nread!= sizeof(bt)){
        std::cout<<"error reading mbr"<<" "<< nread<<" "<<sizeof(bt);
         return -1;
        
    }
    
    return 1;
    
}
 
 int ReadMap(vdiFile* fl, unsigned int mp[]){
     if( lseek(fl->file, fl->header.offsetBlocks, SEEK_SET)<0){
         std::cout<<"failed to seek map";
         return -1;
     }
     
     if(read(fl->file,mp , 4*(fl->header.blocksInHdd) )<0){
         std::cout << "failed to read map";
         return -1;
     }
     
     return 1;
     
     
 }
 
 unsigned int translate(vdiFile* fl,unsigned int ad, BootSector bs, unsigned int map[] ){
     
     unsigned int ad1= bs.partitionTable[0].sector_1*512 +ad;
     unsigned int offset = ad1%fl->header.blockSize;
     unsigned int ad2 = map[ad1/fl->header.blockSize];
     return (fl->header.offsetData+ ad2*fl->header.blockSize +offset);
 }
 
 int ReadSuperBlock(vdiFile* fl, unsigned int SupLocation, ext2_super_block& SupBlock ){
     
     if (lseek(fl->file,SupLocation ,SEEK_SET)<0){
         std:: cout << " Error seeking Super block";
         return -1;
         
     }
     
     int nread = read(fl->file, &SupBlock, sizeof(SupBlock));
     if (nread!=sizeof(SupBlock)){
         std::cout<< "Error Reading Super block";
         return -1;
     }
     return 1 ;
     
 }
 
 int ReadGroupDescriptor(vdiFile* fl, int group_count, ext2_group_descriptor desc[], unsigned int location){
     
     if(lseek(fl->file, location, SEEK_SET )<0){
         std::cout << "Error Seeking Group Discriptor";
         return -1;
     }
     
     int nread =read(fl->file, desc, sizeof(ext2_group_descriptor)*group_count);
     
          if(nread!= sizeof(ext2_group_descriptor)*group_count){
              std::cout<<dec<< nread<<" "<<"**";
             std::cout<<dec<< sizeof(ext2_group_descriptor)*group_count;
         std::cout << "Error Reading Group Discriptor";
         return -1;
     }
 }
 
 inode ReadInode (vdiFile* fl, inode& tinode , unsigned int inodeLocation){
     if (lseek(fl->file,inodeLocation, SEEK_SET)<0){
         std::cout << "Error reading Inode";
       
     }
     
     if (read(fl->file, &tinode, sizeof(inode))<0){
         std::cout <<"Error Reading Inode";
         
     }
 }
 
 
 bool getDirEntry(ext2_dir_entry_2& found, unsigned char* dataBlock, unsigned int sizeDifference, string filename, bool displayfiles)
{
	ext2_dir_entry_2 * dir_entry = (ext2_dir_entry_2 *)malloc(sizeof(ext2_dir_entry_2));
	memcpy(dir_entry, dataBlock, sizeof(*dir_entry));
	unsigned int size = 0;
	while (size < sizeDifference)
	{
		char file_name[256];
		memcpy(file_name, dir_entry->name, dir_entry->name_len);
		file_name[dir_entry->name_len] = '\0'; 
		if (dir_entry->inode != 0)
		{
			if (displayfiles) cout << file_name << endl;
			else if ((string)file_name == filename)
			{
				found = *dir_entry;
				free(dir_entry);
				return true;
			}
		}
		else
		{
			size += sizeof(*dir_entry);
			memcpy(dir_entry, dataBlock + size, sizeof(*dir_entry));
			continue;
		}
		size += dir_entry->rec_len;
		memcpy(dir_entry, dataBlock + size, sizeof(*dir_entry));
	}
	free(dir_entry);
	return false;
}
 
 
 
 int readBlock(inode inode, unsigned int blockNum, unsigned int blockSize, vdiFile* f, BootSector bootSector,
              unsigned int vdiMap[], unsigned char* buf)
{
	if (blockNum * blockSize >= inode.size) return -1;
	int direct, single_index, double_index, triple_index;
	computeIndex(blockNum, blockSize, direct, single_index, double_index, triple_index);
	unsigned int direct_block_num = 0;
	unsigned int single_block_num = 0;
	unsigned int double_block_num = 0;
	unsigned int triple_block_num = inode.i_block[14];
	bool hole = false; //Tracks any holes
	if (triple_index != -1)
	{
		if (triple_block_num == 0) hole = true;
		else
		{
			if (single_index != -1 && double_index != -1 && direct == -1)
			{
				if (lseek(f->file, translate( f,triple_block_num * blockSize, bootSector, vdiMap), SEEK_SET) < 0) return -1;
				if (read(f->file, buf, blockSize) < 0) return -1;
				if (triple_index >= (blockSize / 4)) return -1;
				double_block_num = *(((unsigned int *)buf) + triple_index);
			}
			else return -1;
		}
	}
	if (!hole && double_index != -1)
	{
		if (single_index != -1 && direct == -1)
		{
			if (triple_index == -1) double_block_num = inode.i_block[13];

			if (double_block_num == 0) hole = true;

			else
			{
				if (lseek(f->file, translate( f,double_block_num * blockSize, bootSector, vdiMap), SEEK_SET) < 0) return -1;
				if (read(f->file, buf, blockSize) < 0) return -1;
				if (double_index >= (blockSize / 4)) return -1;
				single_block_num = *(((unsigned int *)buf) + double_index);
			}
		}
		else return -1;
	}
	if (!hole && single_index != -1)
	{
		if (direct == -1)
		{
			if (double_index == -1) single_block_num = inode.i_block[12];
			if (single_block_num == 0) hole = true;
			else
			{
				if (lseek(f->file, translate( f,single_block_num * blockSize, bootSector, vdiMap), SEEK_SET) < 0) return -1;
				if (read(f->file, buf, blockSize) < 0) return -1;
				if (single_index >= (blockSize / 4))return -1;
				direct_block_num = *(((unsigned int *)buf) + single_index);
			}
		}
		else return -1;
	}
	if (!hole && direct != -1)
	{
		if (direct < 12) direct_block_num = inode.i_block[direct];
		else return -1;
	}
	if (hole) memset(buf, 0, blockSize);
	else
	{
		if (lseek(f->file, translate( f,direct_block_num * blockSize, bootSector, vdiMap), SEEK_SET) < 0) return -1;
		if (read(f->file, buf, blockSize) < 0) return -1;
	}
	unsigned int difference = inode.size - (blockNum * blockSize);
	if (difference >= blockSize) return blockSize;
	return difference;
}

/**
 * Helper function to compute the indices.
 */
void computeIndex(unsigned int blockNum, unsigned int blockSize, int& directNumber, int& indirectIndex, int& double_index,
                  int& triple_index)
{
	if (blockNum <= 11)
	{
		directNumber = blockNum;
		indirectIndex = -1;
		double_index = -1;
		triple_index = -1;
		return;
	}
	unsigned int blocks_remaining = blockNum - 12;
	if (blocks_remaining < blockSize / 4)
	{
		directNumber = -1;
		double_index = -1;
		triple_index = -1;
		indirectIndex = blocks_remaining;
		return;
	}
	blocks_remaining -= blockSize / 4;
	if (blocks_remaining < (blockSize / 4) * (blockSize / 4))
	{
		directNumber = -1;
		triple_index = -1;
		double_index = blocks_remaining / (blockSize / 4);
		indirectIndex = blocks_remaining - (double_index) * (blockSize / 4);
		return;
	}
	blocks_remaining -= (blockSize / 4) * (blockSize / 4);
	if (blocks_remaining < (blockSize / 4) * (blockSize / 4) * (blockSize / 4))
	{
		directNumber = -1;
		triple_index = blocks_remaining / ((blockSize / 4) * (blockSize / 4));
		double_index = (blocks_remaining - (triple_index) * (blockSize / 4) * (blockSize / 4)) / (blockSize / 4);
		indirectIndex = (blocks_remaining - (triple_index) * (blockSize / 4) * (blockSize / 4)) - (double_index) * (blockSize / 4);
		return;
	}
}
 
 
 

 
 
 
 
  
     
 
 
 
 


