#include "driver/hd.h"
#include "gui/gui.h"
#include "gui/layer.h"
#include "gui/graphic.h"
#include "kernel/memory.h"
#include "kernel/io.h"
#include "driver/clock.h"
#include "lib/bmp.h"
#include "fs/fs.h"

#define BMP_SECTORS 2820
#define WIN_SECTORS 550

extern struct layer *layer_pic;
//117kb文件显示没有问题


void task_hd_entry()
{
	hd_identify(0);
	fs_main();
	uint8_t *buf_image = sys_malloc(BMP_SECTORS*SECTOR_SIZE+50*SECTOR_SIZE);
	uint8_t *buf = buf_image;
	uint8_t *buf2_image = sys_malloc(WIN_SECTORS*SECTOR_SIZE+10*SECTOR_SIZE);
	uint8_t *buf2 = buf_image;
	//sys_list_folder("/");
	//sys_list_folder("/bin/");
  /* int i;
   int count = BMP_SECTORS/200;
   int left = BMP_SECTORS-(BMP_SECTORS/200);
   
   for(i = 0; i < count; i++){
	   hd_read(0,2000+i*200,buf + i*200*512, 200);
   }
   hd_read(0,2000+(i+1)*200,buf+(i+1)*200*512, left);
*/
	/*int i, j;
	for(i = 0, j = 0;i < BMP_SECTORS/200-1; i++){
		hd_read(0,2000+j,(void *)(buf +j*512), 200);
		j += 200;
	}
	hd_read(0,2000+j,(void *)(buf +j*512), BMP_SECTORS-(BMP_SECTORS/200-1)*512);*/
  
/*   int j = 0;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
	hd_read(0,4000+j,buf +j*512, 200);
	
	j += 200;
	hd_read(0,4000+j,buf +j*512, 13);
*/
/*	int j=0;
	for(int q=0;q<14;q++){
	hd_read(0,4000+j,buf +j*512, 200);
	j += 200;
}
	hd_read(0,4000+j,buf +j*512, 13);
*/




	WR_TEMP(buf_image);


	
/*
	int j = 0;
	hd_read(0,7000+j,buf2 +j*512, 200);
	j += 200;
	hd_read(0,7000+j,buf2 +j*512, 200);
	j += 200;
	hd_read(0,7000+j,buf2 +j*512, 151);
	
   gprintk("@@@@data%x#", buf2[200*200]);
   
   sys_unlink("/win");
sys_open("/win", 4);
	int fd = sys_open("/win", 3);
	int bytes = sys_write(fd, buf2,230454);
	gprintk("write data%d#", bytes);
	
	
	close(fd);
   
*/
/*
	sys_unlink("/ico");
	sys_open("/ico", 4);
	int fd = sys_open("/ico", 3);
	int bytes = sys_write(fd, buf,BMP_SECTORS*SECTOR_SIZE);
	gprintk("write data%d#", bytes);
	
	close(fd);
	*/

    //sys_lseek(fd, 0, 1);
	
	/*
    memset(buf, 0, 4096); bytes = read(fd, buf, 1024); gprintk("read%d#",
     bytes); gprintk("data%x#", *buf);
	*/
	//
	//show_photo("/ico");
	sys_mfree(buf_image);
	sys_mfree(buf2_image);
	
	layer_image_bmp(layer_desktop, 0,0, "/ico");
	layer_image_bmp(layer_pic, 0,0, "/win");
	
	for(;;){
		
	}
}


struct hd hd;

//uint8_t hdbuf[SECTOR_SIZE * 2];

void init_hd()
{

	uint8_t * harddisk_nr = (uint8_t*)HARDDISK_NR_ADDR;
	uint8_t *hdbuf = sys_malloc(SECTOR_SIZE*2);
	hd.devices = *harddisk_nr;
	
	
	hd.buf = hdbuf;
	/*
	draw_int16(layer_desktop->buf,layer_desktop->wide, 0, 0, COLOR_WHITE, *harddisk_nr);
	layer_refresh(layer_desktop, 0, 0, 100, 16);
	*/
	//打开中断
	put_irq_handler(AT_WINI_IRQ, hd_handler);
	enable_irq(CASCADE_IRQ);
	enable_irq(AT_WINI_IRQ);
	
	hd.interrupt_ready = 1;
}

void hd_handler(int32_t irq)
{
	hd.interrupt_ready = 1;
	io_in8(REG_STATUS);
}

/*
获取硬盘基本信息
硬盘中断必须先做好，不然有错误
*/
void hd_identify(int drive)
{
	struct hd_cmd cmd;
	memset(&cmd,0,sizeof(struct hd_cmd));
	cmd.device  = MAKE_DEVICE_REG(0, drive, 0);
	cmd.command = ATA_IDENTIFY;	//获取信息命令
	hd_cmd_out(&cmd);	//写cmd
	
	interrupt_wait();	//等待硬盘中断
	
	port_read(REG_DATA, hd.buf, SECTOR_SIZE);	//读取数据
	
	print_identify_info((uint16_t*)hd.buf);		//分析并显示出来
}
void hd_read(int drive, int lba, void *buf, int counts)
{
	struct hd_cmd cmd;
	memset(&cmd,0,sizeof(struct hd_cmd));
	cmd.device  = MAKE_DEVICE_REG(1, drive, (lba >> 24) & 0xF);
	cmd.count = counts;
	cmd.lba_low = lba;
	cmd.lba_mid = lba>>8;
	cmd.lba_high = lba>>16;
	cmd.command = ATA_READ;	//获取信息命令
	hd.interrupt_ready = 0;
	
	hd_cmd_out(&cmd);	//写cmd
	
	interrupt_wait();	//等待硬盘中断
	port_read(REG_DATA, buf, counts*512);	//读取数据
}

void hd_write(int drive, int lba, void *buf, int counts)
{
	//layer_string(layer_desktop, 200,300,"a", COLOR_WHITE);
	
	struct hd_cmd cmd;
	memset(&cmd,0,sizeof(struct hd_cmd));
	cmd.device  = MAKE_DEVICE_REG(1, drive, (lba >> 24) & 0xF);
	cmd.count = counts;
	cmd.lba_low = lba;
	cmd.lba_mid = lba>>8;
	cmd.lba_high = lba>>16;
	cmd.command = ATA_WRITE;	//获取信息命令
	hd.interrupt_ready = 0;
	
	hd_cmd_out(&cmd);	//写cmd
	layer_square(layer_desktop, 200,300, 8,16, COLOR_BLACK);
	layer_string(layer_desktop, 200,300,"1", COLOR_WHITE);
	
	waitfor(STATUS_DRQ, STATUS_DRQ, HD_TIMEOUT);
	
	port_write(REG_DATA, buf, counts*512);	//读取数据
	
	interrupt_wait();	//等待硬盘中断
	layer_square(layer_desktop, 200,300, 8,16, COLOR_BLACK);
	layer_string(layer_desktop, 200,300,"2", COLOR_WHITE);
	
	
	layer_square(layer_desktop, 200,300, 8,16, COLOR_BLACK);
	layer_string(layer_desktop, 200,300,"3", COLOR_WHITE);
	
	
	layer_square(layer_desktop, 200,300, 8,16, COLOR_BLACK);
	layer_string(layer_desktop, 200,300,"4", COLOR_WHITE);
	
}

int hd_cmd_out(struct hd_cmd* cmd)
{
	if (!waitfor(STATUS_BSY, 0, HD_TIMEOUT))
		//PANIC("hd error.");

	/* Activate the Interrupt Enable (nIEN) bit */
	io_out8(REG_DEV_CTRL, 0);
	/* Load required parameters in the Command Block Registers */
	io_out8(REG_FEATURES, cmd->features);
	io_out8(REG_NSECTOR,  cmd->count);
	io_out8(REG_LBA_LOW,  cmd->lba_low);
	io_out8(REG_LBA_MID,  cmd->lba_mid);
	io_out8(REG_LBA_HIGH, cmd->lba_high);
	io_out8(REG_DEVICE,   cmd->device);
	/* Write the command code to the Command Register */
	io_out8(REG_CMD,     cmd->command);
}

void print_identify_info(uint16_t* hdinfo)
{
	int i, k;

	//显示获取的信息
	int capabilities = hdinfo[49];
	/*draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 32, COLOR_WHITE, ("LBA supported:"));
	draw_string16(layer_desktop->buf,layer_desktop->wide, 150, 32, COLOR_WHITE, (capabilities & 0x0200) ? "Yes" : "No");*/
	int cmd_set_supported = hdinfo[83];
	/*draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 48, COLOR_WHITE, ("LBA48 supported:"));
	draw_string16(layer_desktop->buf,layer_desktop->wide, 150, 48, COLOR_WHITE, ((cmd_set_supported & 0x0400) ? "Yes" : "No"));*/
	int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
	/*draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 64, COLOR_WHITE, ("HD size: "));
	draw_int16(layer_desktop->buf,layer_desktop->wide, 150, 64, COLOR_WHITE, sectors * 512 / (1024*1024));
	*/
	//layer_refresh(layer_desktop, 0, 0, 200, 100);
	//保存
	hd.sectors = sectors;
}

int waitfor(int mask, int val, int timeout)
{
	int t = sys_getTicks();
	while(((sys_getTicks() - t) * 1000 / HZ) < timeout)
		if ((io_in8(REG_STATUS) & mask) == val)
			return 1;

	return 0;
}

int interrupt_wait()
{
	//循环等待中断发生
	while(1) {
		
		if (hd.interrupt_ready==1) {
			//hd.interrupt_ready=0;
			return 1;
		}
	}
	return 0;
}
//temp for read
void WR_TEMP(uint8_t *buf){
		int j=0;
      for(int i=0;i<14;i++){
		hd_read(0,4000+j,buf +j*512, 200);
	    j += 200;
      	}
		hd_read(0,4000+j,buf +j*512, 13);


		sys_unlink("/ico");
	sys_open("/ico", 4);
	int fd = sys_open("/ico", 3);
	int bytes = sys_write(fd, buf,BMP_SECTORS*SECTOR_SIZE);
	gprintk("write data%d#", bytes);
	
	close(fd);
	  
}


