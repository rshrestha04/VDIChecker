/* 
 * File:   main.cpp
 * Author: Ronish
 *
 * Created on April 5, 2019, 12:15 AM
 */

#include <cstdlib>
#include "vdiReader.h"
#include "typedefine.h"
#include "VdiHead.h"
#include "Header.h"
#include <iostream>
#include <iomanip>
#include "group_descriptor.h"
#include "Inode.h"
#include "DirectoryEntry.h"
#define width 30



using namespace std;

/*
 * 
 */
int main(int argc, char* argv[]) {
    int fs;
    vdiFile* test = new vdiFile;
    int readInfo;
    
    
     
    fs = VdiOpen(test,"try.vdi");
    cout<< fs<<" ";
    
    test->cursor=0;
    
   // VdiSeek(test,0,SEEK_SET);
    readInfo = VdiRead(test,&(test->header),sizeof(test->header));
   
    cout <<dec << test->header.blockSize<<" " <<readInfo << " ";
    
    
      
 
 

			//cout << setw(width + 20)  << right << vh.text;   cout << endl;
	  		//cout << setw(width)  << right << "SIGNATURE: "        << hex << 
			cout << setw(width)  << right << "VERSION: "          << hex << test->header.version << endl;
			cout << setw(width)  << right << "HEADER SIZE: "      << dec << test->header.headerSize << endl;
			cout << setw(width)  << right << "IMAGE TYPE: "       << dec << test->header.imageType << endl;	
			cout << setw(width)  << right << "IMAGE FLAGS: "      << dec << test->header.imageFlags << endl;
		//	cout << setw(width)  << right << "DESCRIPTION: "      << test- << endl;
			cout << setw(width)  << right << "OFFSET BLOCKS: "    << hex << test->header.offsetBlocks << endl;
			cout << setw(width)  << right << "OFFSET DATA: "	  << hex << test->header.offsetData << endl;
		//	cout << setw(width)  << right << "CYINDERS: "         << dec << vh.cylinders << endl;
		//	cout << setw(width)  << right << "HEADS: "            << dec << vh.heads << endl;
		//	cout << setw(width)  << right << "SECTORS: "          << dec << vh.sectors << endl;
			cout << setw(width)  << right << "SECTOR SIZE: "      << dec << test->header.sectorSize << endl;
		//	cout << setw(width)  << right << "UNUSED1: "          << dec << vh.unused1 << endl;
			cout << setw(width)  << right << "DISK SIZE: "         <<dec<< test->header.diskSize << endl;
			cout << setw(width)  << right << "BLOCK SIZE: "       << dec << test->header.blockSize << endl;
		//	cout << setw(width)  << right << "BLOCK EXTRA: "      << dec << vh.block_extra << endl;
			cout << setw(width)  << right << "BLOCKS IN HDD: "    << dec << test->header.blocksInHdd << endl;
			cout << setw(width)  << right << "BLOCKS ALLOCATED: " << dec << test->header.blocksAllocated << endl;
			cout << endl;
		//	cout << "*******************************************************************************"<< endl;
   
    
    
    
    unsigned map[test->header.blocksInHdd];
    int mapInfo= ReadMap(test, map);
    
    
    BootSector boot;
     if( ReadMasterBoot(test, boot)==-1){
        return 0;
    }
    cout<< hex << boot.magic;
    
    
    cout << "VDI MAP" << endl;
	cout << "============" << endl;
	for (int i = 0; i < 128; i++)
	{
		cout << map[i] << " ";
		if (i == 127) cout << endl;
	}
	cout << "============" << endl;
    
    
        
        //Finding locatiobn of superblock andReading Super Block
    unsigned int superBlockLocation= translate(test,1024, boot,map);
    
    ext2_super_block superBlock;
    
    if(ReadSuperBlock(test,superBlockLocation,superBlock)<0){
        return -1;
    }
    
    cout << "Magic Superblock "<<superBlock.s_magic ;
    
    //	cout << "------------------------------------------------------------------------"<< endl;
	cout << "Partition" << setw(width-5) << right << "Type" << setw(width-4) << right << "Start" << endl;
//	cout << "------------------------------------------------------------------------"<< endl;
	for(int i=0; i<4; i++)
	{
		cout << "Partition["   << i+1   << "]" << setw(width-10) 
		     << right << hex   << "0x"  << (int)boot.partitionTable[i].type 
		     << setw(width-4)  << right << dec
		     << boot.partitionTable[i].sector_1  << endl;
	}
	
	cout << "MBR MAGIC: " << hex << boot.magic <<endl;
	cout << endl;
//	cout << "-----------------------------------------------------------------------"<< endl;
	
        
        
        //Finding group count and reading group descriptor

        int group_count= (superBlock.s_blocks_count - superBlock.s_first_data_block)/superBlock.s_blocks_per_group;
        if (((superBlock.s_blocks_count - superBlock.s_first_data_block)%superBlock.s_blocks_per_group) > 0){
            group_count ++;
        }
       
        cout<< group_count<< " _______________ "<<endl;
        ext2_group_descriptor grp_desc[group_count];
        unsigned int blockSize = 1024 << superBlock.s_log_block_size;
        unsigned int address=0;
        cout<<blockSize<< " **" ;
        if (blockSize==1024){
             address= 2*blockSize;
        }
        else address= 1*blockSize;
        
        cout << address <<" **" ;
          
        
        unsigned int groupDescriptorLocation = translate(test,address,boot,map);
        cout<< groupDescriptorLocation<< " _______________ "<<endl;
        cout<< dec<<superBlock.s_blocks_count<< " _______________ "<<endl;
        cout<< dec<<superBlock.s_blocks_per_group<< " _______________ "<<endl;
        cout<< dec<<superBlock.s_log_block_size<< " _______________ "<<endl;
        cout<< dec<<superBlock.s_first_data_block<< " _______________ "<<endl;
        
        
        
       if(ReadGroupDescriptor(test,group_count, grp_desc, groupDescriptorLocation)<0){
            return -1;
       }
       
        cout << "Group descriptor table" << endl;
	cout << "======================" << endl;
	for (int i = 0; i < group_count; i++)
	{
		cout << "Block bitmap: " << dec   <<grp_desc[i].block_bitmap << endl;
		cout << "Inode bitmap: " << grp_desc[i].inode_bitmap << endl;
		cout << "Inode table: " << grp_desc[i].inode_table << endl;
	}
	cout << "======================" << endl;
	cout << endl;
        
        
        /*****READING INODE*******/
       
        inode testinode;
        
        unsigned int Grp_count= 1/superBlock.s_inodes_per_group;
        cout<< Grp_count <<"***************";
        unsigned int offset1= 1% superBlock.s_inodes_per_group;
        cout<<superBlock.s_inodes_per_group<<"***"<<endl;
        unsigned int inodeinoneblock= blockSize/ sizeof(struct inode);
        unsigned int blockNumber= grp_desc[Grp_count].inode_table+ offset1/inodeinoneblock;
        cout << grp_desc[Grp_count].inode_table <<endl;
        cout<< blockNumber << endl;
        cout << blockSize <<endl;
        unsigned int offset2= 1%inodeinoneblock;
        unsigned int adrs = (blockNumber* blockSize) + (offset2 *sizeof(inode)); 
        unsigned int inodeLocation = translate(test,adrs ,boot,map );
        cout<< dec<<inodeLocation<<endl;
        
        
        testinode = ReadInode(test,testinode,inodeLocation );
        
        cout<<testinode.size;
        
        
        /**Getting directories****/
        
        
        
        
        
   return 0;
   
   
   
}



	
