/* 
 * File:   group_descriptor.h
 * Author: Ronish
 *
 * Created on April 17, 2019, 9:06 PM
 */

#ifndef GROUP_DESCRIPTOR_H
#define	GROUP_DESCRIPTOR_H

struct __attribute__ ((packed)) ext2_group_descriptor{
  unsigned int block_bitmap;
  unsigned int inode_bitmap;
  unsigned int inode_table;
  unsigned short free_blocks_count;
  unsigned short free_inodes_count;
  unsigned short used_dirs_count;
  unsigned short pad;
  unsigned int reserved[3];

	};



#endif	/* GROUP_DESCRIPTOR_H */

