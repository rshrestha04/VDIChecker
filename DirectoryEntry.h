/* 
 * File:   DirectoryEntry.h
 * Author: Ronish
 *
 * Created on April 23, 2019, 1:00 AM
 */

#ifndef DIRECTORYENTRY_H
#define	DIRECTORYENTRY_H


struct __attribute__ ((packed)) ext2_dir_entry_2{
  unsigned int inode; //Inode number
  unsigned short rec_len; //Directory entry length
  unsigned char name_len; //Name length
  unsigned char file_type; //Type of file [1 - Regular Name, 2 - Directory etc.]
  unsigned char name[225]; //File name

	};



#endif	/* DIRECTORYENTRY_H */

