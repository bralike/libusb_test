/*
 * a program for getting SerialNumber of USB
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <libusb.h>

#define MAX 256

/*切记结构体定义完 尾部加；号*/
struct usb_info_st{
	char sn[MAX];
	uint16_t vid;
	uint16_t pid;
};

void t_init(){
	int r;
	libusb_init(NULL);
	if(r<0){
		perror("init failed");
		exit(-1);
	}	
	return;
}

ssize_t list_usb(struct usb_info_st *st){
	struct libusb_device **devs = NULL;
	struct libusb_device *dev = NULL;
	struct libusb_device_descriptor desc = {0};
	struct libusb_device_handle *husb = NULL;
	char buf[MAX] = {0};
	signed int len = 0;
	int i = 0;

	len  = libusb_get_device_list(NULL, &devs);
	if (len==0){
		perror("can not find list device!");
		exit(-1);
	}
	while ((dev = devs[i++]) != NULL) {
		memset(&desc,sizeof(struct libusb_device_descriptor),0);
		int d = libusb_get_device_descriptor(dev, &desc);
		if (d < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return 0;
		}
		libusb_open(dev,&husb);
//		libusb_open_device_with_vid_pid(husb,desc.idVendor,desc.idProduct);
		libusb_get_string_descriptor_ascii(husb,desc.iSerialNumber,st[i].sn,MAX);
		st[i].pid=desc.idProduct;
		st[i].vid=desc.idVendor;
//		printf("%04x:%04x (bus %d, device %d, serialnumber %c)\n",
//			desc.idVendor, desc.idProduct,
//			libusb_get_bus_number(dev), libusb_get_device_address(dev),desc.iSerialNumber);
//	}
	
		libusb_close(husb);
	}

	libusb_free_device_list(devs, 1);
	return len;
}

int main(void)
{
	struct usb_info_st st[MAX];
	libusb_device **devs;
	int list_len = 0;
	t_init();
	list_len = list_usb(st);
	for(int i=0;i<list_len;i++){
		if(  st[i].sn != 0  &&  strnlen(st[i].sn,MAX) >= 16 )
		printf("pid:%d,vid:%d,sn:%s\n",st[i].pid,st[i].vid,st[i].sn);
	}		

//	print_devs(devs);

	libusb_exit(NULL);
	return 0;
}

