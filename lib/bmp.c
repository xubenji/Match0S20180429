/*************************
*bmp.c文件
*************************/

#include "lib/bmp.h"
#include "driver/video.h"
#include "fs/fs.h"
#include "gui/graphic.h"
#include "gui/layer.h"
#include "gui/desktop.h"
extern struct layer *layer_desktop;

int32_t layer_image_bmp(struct layer *layer, uint32_t x,uint32_t y, const char *bmpname)
{
	if(NULL == bmpname)return -1;
	
	uint32_t off_x = x, off_y = y;
	int line_x = 0, line_y = 0;
	uint16_t tmp = 0;

	BitMapFileHeader FileHead;
	BitMapInfoHeader InfoHead;
	RgbQuad rgb;

	unsigned short location = 0;

	//打开.bmp文件
	int fd = sys_open(bmpname, O_RDWR);
	if (fd == -1){
		gprintk("fopen bmp error#");
		return -1;
	}

	//读文件信息
	if (0 == sys_read(fd, &FileHead, sizeof(BitMapFileHeader))){
		gprintk("read BitMapFileHeader error!#");
		close(fd);
		return -1;
	}
	if (memcmp(FileHead.bfType, "BM", 2) != 0){
		gprintk("it's not a BMP file\n");
		close(fd);
		return -1;
	}
	
	gprintk("image head info: size:%d Off:%d#",FileHead.bfSize,FileHead.bfOffBits);
 
	//读位图信息
	if (0 == sys_read(fd, (char *)&InfoHead, sizeof(BitMapInfoHeader))){
		gprintk("read BitMapInfoHeader error!#");
		close(fd);
		return -1;
	}
 //跳转至数据区
	sys_lseek(fd, FileHead.bfOffBits, SEEK_SET);
	gprintk("image info: size:%d width:%d height:%d image size:%d bit:%d#",InfoHead.biSize,InfoHead.biWidth, InfoHead.biHeight, InfoHead.biSizeImage, InfoHead.biBitCount);

	int len = InfoHead.biBitCount / 8;
 
	uint8_t *buf_image = (uint8_t *)sys_malloc(InfoHead.biSizeImage);
	uint8_t *buf = buf_image;
	//gprintk("alloced ");
 
	memset(buf, 0x00, InfoHead.biSizeImage);
	if (0 == sys_read(fd,(uint8_t *)buf, InfoHead.biSizeImage)){
		sys_mfree(buf);
		return -1;
	}

	int image_size = InfoHead.biSizeImage;

	/*int i;
	for(i = 0; i < 100; i++){
		gprintk("%x ", buf[400*300+i]);
	}*/
	rgb.Reserved = 0xFF;
	line_x = 0;
	line_y = InfoHead.biHeight-1;
	//gprintk("b2 ");
	/*buf += image_size;
	buf -=3;
	*/
	while(image_size > 0){
		tmp = 0;
		memcpy(&rgb, buf, len);
		tmp = RGB16(rgb.Red, rgb.Green, rgb.Bluse);
		draw_pix16(layer->buf,layer->wide, off_x+line_x, off_y+line_y, tmp);
		image_size-=len;
		buf += len;
		
		//倒着显示回来（文件结构问题）
		line_x++;
		if (line_x == InfoHead.biWidth){
			line_x = 0;
			line_y--;
			if(line_y == 0)break;
		}    
	}
	
	layer_refresh(layer,off_x,off_y,off_x+InfoHead.biWidth+1, off_y+InfoHead.biHeight+1);
	sys_mfree(buf_image);
	close(fd);
	return 0;
}

/*************************
*fdp，映射内存起始地址
*scrinfo，屏幕信息结构体
*bmpname，.bmp位图文件名
*************************/
int show_photo(const char *bmpname)
{
 if(NULL == bmpname)return -1;

 int line_x = 0, line_y = 0;
uint16_t tmp = 0;

 BitMapFileHeader FileHead;
 BitMapInfoHeader InfoHead;
 RgbQuad rgb;

 unsigned short location = 0;

 //打开.bmp文件
 int fd = sys_open(bmpname, O_RDWR);
 if (fd == -1)
 {
 gprintk("fopen bmp error#");
 return -1;
 }

 //读文件信息
 if (0 == sys_read(fd, &FileHead, sizeof(BitMapFileHeader)))
 {
 gprintk("read BitMapFileHeader error!#");
 close(fd);
 return -1;
 }
 if (memcmp(FileHead.bfType, "BM", 2) != 0)
 {
 gprintk("it's not a BMP file\n");
 close(fd);
 return -1;
 }
 
 //读位图信息
 if (0 == sys_read(fd, (char *)&InfoHead, sizeof(BitMapInfoHeader)))
 {
 gprintk("read BitMapInfoHeader error!#");
 close(fd);
 return -1;
 }
 //跳转至数据区
 sys_lseek(fd, FileHead.bfOffBits, SEEK_SET);
 gprintk("image info: size:%d width:%d height:%d image size:%d bit:%d#",InfoHead.biSize,InfoHead.biWidth, InfoHead.biHeight, InfoHead.biSizeImage, InfoHead.biBitCount);
 
 
 
 
 
 int len = InfoHead.biBitCount / 8;
 //gprintk("biBitCount %d#",InfoHead.biBitCount);
 
// gprintk("ready draw#");
  //gprintk("ready draw%d %d#", file_table[fd].fd_pos, file_table[fd].fd_block->size);

 //gprintk("biSize %d#",InfoHead.biSize);
 
 //int size = InfoHead.biWidth*InfoHead.biHeight*len;
 
 //gprintk("size %d %d#",size, InfoHead.biSizeImage);
 
 uint8_t *buf = (uint8_t *)sys_malloc(InfoHead.biSizeImage);
 
  gprintk("alloced ");
 
 memset(buf, 0, InfoHead.biSizeImage);
 if (0 == sys_read(fd,(uint8_t *)buf, InfoHead.biSizeImage))
 {
	sys_mfree(buf);
	return -1;
 }
 gprintk("readed ");
 
 int image_size = InfoHead.biSizeImage;
// int size2 = InfoHead.biWidth*InfoHead.biHeight*3;
 //uint16_t *buf16 = (uint16_t *)sys_malloc(size2);
	rgb.Reserved = 0xFF;
	
	line_x = 0;
	line_y = InfoHead.biHeight;
	gprintk("b2 ");
	while(image_size > 0){
		tmp = 0;
		
		memcpy(&rgb, buf, len);
		tmp = RGB16(rgb.Red, rgb.Green, rgb.Bluse);
		draw_pix16(layer_desktop->buf,layer_desktop->wide, line_x, line_y, tmp);
		image_size-=len;
		buf += len;
		
		//倒着显示回来（文件结构问题）
		line_x++;
		if (line_x == InfoHead.biWidth){
			line_x = 0;
			line_y--;
			if(line_y == 0)break;
		}    
	}
 
	layer_refresh(layer_desktop,0,0,InfoHead.biWidth+1, InfoHead.biHeight+1);
 
 
 close(fd);
 return 0;
}
