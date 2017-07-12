#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <getopt.h>

int cr, th, tl, address;

int main (int argc, char ** argv)
{
    char c;
    while (1){
        c = getopt (argc, argv, "A:C:L:H:");
        if (c == -1){
            /* We have finished processing all the arguments. */
            break;
        }
        switch (c){
        case 'A':
            address = atoi(optarg);
            printf ("The selected chip-address is: %s\n", optarg);
            break;
        case 'C':
            cr = atoi(optarg);
            printf ("The argument cr is %s !\n", optarg);
            break;
        case 'L':
            tl = atoi(optarg);
            printf ("The argument TL is set to %s .\n", optarg);
            break;
        case 'H':
            th = atoi(optarg);
            printf ("The argument TH is set to %s .\n", optarg);
            break;
        case '?':
        default:
            printf("Usage: %s -A [chip-address] -C [config-reg] -L [Tlow-reg] -H [Thigh-reg] \n",argv[0]);
            break;
        }
    }

    printf ("The value stored in the parameters: %#04x %#04x %#04x %#04x\n", address, cr, tl, th);

    int file;
    file = open("/dev/i2c-1", O_RDWR);
    if (file < 0){
        printf("ERROR open(%d)failed\n", file);
        exit(1);
    }

    //int addr = 0x48;

    if (ioctl(file, I2C_SLAVE, address) < 0){
        printf("IOCTL failed\n");
        exit(1);
    }
    __u8 reg = 0x00;
    __s32 res, b1, b0;

    if ( cr > 255 || cr < 0 ){
        printf ("The Config register value is out of range!\n");
        exit (-1);
    } else {
       i2c_smbus_write_word_data(file, 0x01, cr);
    }

    if ( tl > 65535 || tl < 0){
        printf ("The TempLow register value is out of range!\n");
        exit (-1);
    } else {
        i2c_smbus_write_word_data(file, 0x02, tl);
    }

    if ( th > 65535 || th < 0){
        printf ("The TempHigh register value is out of range!\n");
        exit (-1);
    } else {
        i2c_smbus_write_word_data(file, 0x03, th);
    }

    res = i2c_smbus_read_word_data(file, reg);

    float t;

    if (res < 0){
        printf("SMBus read failed\n");
        exit(1);
    } else {
        b1 = (res & 0X000000ff) << 4;
        b0 = (res & 0X0000f000) >> 12;
        t = (b0 + b1) * 0.0625;
        printf ("The temperature is: %.4f °C\n", t);
    }

    return 0;
}
