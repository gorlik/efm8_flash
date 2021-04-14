/******************************************************************************
 *  EFM8 Flash - Flash tool for Silicon labs EFM8 MCUs                        *
 *  Copyright 2020 Gabriele Gorla                                             *
 *                                                                            *
 *  This program is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  EFM8 Flash is distributed in the hope that it will be useful,             *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with EFM8 Flash.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                            *
 ******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "hidapi/hidapi.h"

#define FLASH_SIZE 16*1024

static unsigned char buf[40];
static unsigned char ret[8];

static int send_buf(hid_device *h, unsigned char *b)
{
  int res;
  int size=b[2]+3;
  b[0]=0;
  
  res = hid_send_feature_report(h, b, size);  
  if(res<0) return res;
  
  res = hid_get_feature_report(h, ret, 8);
  if(res<0) return res;
  
  return ret[1];
}

static int send_id(hid_device *h, unsigned int id)
{
  buf[1]=0x24; // prefix
  buf[2]=0x03; // size
  buf[3]=0x30; // ID
  buf[4]=(id>>8)&0xff;
  buf[5]=(id)&0xff;
  
  return send_buf(h,buf);
}

static int send_setup(hid_device *h)
{
  buf[1]=0x24; // prefix
  buf[2]=0x04; // size
  buf[3]=0x31; // ID
  buf[4]=0xa5;
  buf[5]=0xf1;
  buf[6]=0x00;
  
  return send_buf(h,buf);
}

static int send_erase(hid_device *h, unsigned int addr)
{
  buf[1]=0x24; // prefix
  buf[2]=0x03;
  buf[3]=0x32; // ID
  buf[4]=(addr>>8)&0xff;
  buf[5]=(addr)&0xff;
  
  return send_buf(h,buf);
}

static int send_write(hid_device *h, unsigned int addr, unsigned char *data, unsigned int len)
{
  int i;
  
  if(len>32) len=32;
  
  buf[1]=0x24; // prefix
  buf[2]=len+3; // size
  buf[3]=0x33; // ID
  buf[4]=(addr>>8)&0xff;
  buf[5]=(addr)&0xff;
  for(i=0;i<len;i++)
    buf[6+i]=data[i];
  
  return send_buf(h,buf);
}


static int send_runapp(hid_device *h)
{
  buf[1]=0x24; // prefix
  buf[2]=0x03; // size
  buf[3]=0x36; // ID
  buf[4]=0x00;
  buf[5]=0x00;
  
  return send_buf(h,buf);
}


int efm8_find()
{
  struct hid_device_info *devs;
  
  devs=hid_enumerate(0x10c4, 0xeac9);
  
  if(devs) {
    hid_free_enumeration(devs);
    return 1;
  }
  return 0;
}

int efm8_flash(unsigned char *fw_buf, unsigned int fw_size)
{
  hid_device *handle;
  int i;
  
  // EFM8UB1 bootloader starts at 0x3A00
  // max size without erasing the bootloader
  if((fw_size==0) || (fw_size>=(FLASH_SIZE-1536))) {
    //    printf("file exceeds 14.5k\n");
    return -2;
  }
  
  // Open the device using the VID, PID
  handle = hid_open(0x10c4, 0xeac9, NULL);
  
  if(handle==NULL) {
    return -1;
  }
  
  hid_set_nonblocking(handle,0);
  
  send_id(handle,0x3245);
  // send_id(handle,0x3246);
  send_setup(handle);
  
  for(i=0;i<fw_size;i+=512) {
    send_erase(handle,i);
  }
  
  for(i=0;i<fw_size;i+=32) {
    send_write(handle,i,fw_buf+i,fw_size-i);
  }
  
  send_runapp(handle);
  
  hid_close(handle);
  return 0;
}

#ifdef CLI_APP

static unsigned char prg_buf[FLASH_SIZE];

int main(int argc, char *argv[])
{
  FILE *file;
  unsigned int prg_size;
  int i,res;
  
  
  if(argc==2) {
    file=fopen(argv[1],"rb");
    if(file==NULL) {
      printf("can't open %s\n",argv[1]);
      return -2;
    }
    
    for(i=0;i<FLASH_SIZE;i++)
      prg_buf[i]=0xff;
    
    prg_size=fread(prg_buf,1,FLASH_SIZE,file);
    
    if(efm8_find()) {
       printf("Successfully found EFM8UB1\n");
    } else {
      printf("Can't find EFM8UB1\n");
      return -1;
    }
    
    res=efm8_flash(prg_buf,prg_size);
    if(res) {
      printf("flashing failed. Error code: %d\n",res);
    }
    
  } else {
    printf("usage:\n");
    printf("efm8_flash fw.bin     flash fw.bin into the device\n");
  }
  
  return res;
}

#endif
