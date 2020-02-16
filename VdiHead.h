/* 
 * File:   VdiHead.h
 * Author: Ronish
 *
 * Created on April 14, 2019, 3:44 PM
 */

#ifndef VDIHEAD_H
#define	VDIHEAD_H
#include "vdiReader.h"
#include "Header.h"

using namespace std;

struct vdiFile {
    int file;
    vdiHeader header;
    int filesize;
    off_t cursor;
    
    
};



#endif	/* VDIHEAD_H */

