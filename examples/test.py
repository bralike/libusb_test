import os
#import os.path
import sys


def client():
    
	out=os.popen('/root/get_UsbSerialNumber')
	output=out.read()
	print output


if __name__=="__main__":
	client()
