/*
 * C_test.c:
 * Copyright (c) 2014-2020 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
*  1. Connect to board via usb
*  2.build:
*       gcc -o C_test C_test.c
*  3.run to C_test:
*       ./C_test
 ***********************************************************************
 */

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>

int open_serial(int fd, char *serialport, int baudrate)
{
	fd = open(serialport,O_RDWR|O_NOCTTY|O_NDELAY);
	if (-1 == fd)
	{ 
		printf("Can't Open Serial Port\r\n"); 
		return(-1); 
	} 
	printf("Open %s successfully\r\n", serialport); 

		//Set serial port parameters(baudrate, 8, N, 1)
	struct termios rtrobot_termios;
	bzero(&rtrobot_termios, sizeof(rtrobot_termios));
	rtrobot_termios.c_cflag |= CLOCAL | CREAD;
	rtrobot_termios.c_cflag &= ~CSIZE;

	switch( baudrate)
	{
		case 2400:
			cfsetispeed(&rtrobot_termios, B2400);
			cfsetospeed(&rtrobot_termios, B2400);
			break;
		case 4800:
			cfsetispeed(&rtrobot_termios, B4800);
			cfsetospeed(&rtrobot_termios, B4800);
			break;
		case 9600:
			cfsetispeed(&rtrobot_termios, B9600);
			cfsetospeed(&rtrobot_termios, B9600);
			break;
		case 115200:
			cfsetispeed(&rtrobot_termios, B115200);
			cfsetospeed(&rtrobot_termios, B115200);
			break;
		default:
			cfsetispeed(&rtrobot_termios, B9600);
			cfsetospeed(&rtrobot_termios, B9600);
			break;
	}

	rtrobot_termios.c_cflag |= CS8;
	rtrobot_termios.c_cflag &= ~PARENB;
	rtrobot_termios.c_cflag &=  ~CSTOPB;

	if((tcsetattr(fd,TCSANOW,&rtrobot_termios))!=0)
	{
		printf("Failed to set serial port parameters\r\n");
		return -1;
	}
	
	return fd;
}

int write_serial(int fd, char *instruction)
{
	char buffer[512];

	write(fd,instruction, strlen(instruction));

	while (read(fd, buffer, 512)<=0);

	if(buffer[0]!='O'&&buffer[1]!='K')
	{
		printf("error\r\n");
		return -1;
	}
	printf("instruction execution successful for:\r\n%s\r\n", instruction);
	return 0;
}

int main(void)
{
	int fd;
	
	//open the serial  port
	fd=open_serial(fd, "/dev/ttyACM0" ,115200);
	
	//Execution of preset instructions
	write_serial(fd, "#1P2000T1000D100\r\n");
	write_serial(fd, "#1P1000#2P666T1000D2333\r\n");

	//Test for action groups,Group 1 run 3 times
	write_serial(fd, "G1F3\r\n");

	//close the serial port
	close(fd);

	return 0;
}
