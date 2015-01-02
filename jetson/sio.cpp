#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h> // needed for memset

//  sudo i2cset -y 1 0x11 0x00 0xae
 
#define I2CBUS "/dev/i2c-1"
#define ARBOTIXDEV 0x11

int main(int argc,char** argv)
{
    printf( "Opening i2c device /dev/i2c-1" );

    int i2cdev;
    const char *devname = I2CBUS;
    if( (i2cdev = open(devname,O_RDWR)) < 0 ) {
        printf( "failed to open the bus\n" );
        exit( -1 );
    }

    if ( ioctl(i2cdev,I2C_SLAVE,ARBOTIXDEV) < 0 ) {
        printf( "failed to acquire device/set target\n" );
        exit( -1 );
    }
    printf( "setup success\n" );

    struct termios stdio;
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking
    
    unsigned char c=' ';
    while (c!=27) {
        if (read(STDIN_FILENO,&c,1)>0)  write(i2cdev,&c,1);                     // if new data is available on the console, send it to the serial port
    }
    close( i2cdev );
    return 0; 
}

