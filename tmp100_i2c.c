//Welcome to Intel
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <getopt.h>

/* Add a sample of how to use the application
   $ ./tmp100_i2c -A 73 -C 124 -L 0000 -H 65500
   The selected chip-address is: 73
   The CONFIG_REGISTER value is: 124
   The TEMP_LOW_REGISTER value is: 0000
   The TEMP_HIGH_REGISTER value is: 65500
   The value stored in the parameters: 0x49 0x7c 0000 0xffdc !
*/

int main (int argc, char ** argv)
{
    int c, conf_reg, address, file, temp_high, temp_low;
    float t;
    __u8 reg = 0x00;
    __s32 res, b1, b0;

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
            conf_reg = atoi(optarg);
            printf ("The CONFIG_REGISTER value is: %s\n", optarg);
            break;
        case 'L':
            temp_low = atoi(optarg);
            printf ("The TEMP_LOW_REGISTER value is: %s\n", optarg);
            break;
        case 'H':
            temp_high = atoi(optarg);
            printf ("The TEMP_HIGH_REGISTER value is: %s\n", optarg);
            break;
        default:
            printf("Usage: %s -A [chip-address] -C [config-reg] -L [t_low-reg] -H [t_high-reg] \n", argv[0]);
            exit(1);
        }
    }

    //Check the values that you have entered in a hex format
    printf ("The value stored in the parameters: %#04x %#04x %#04x %#04x !\n", address, conf_reg, temp_low, temp_high);

    //address = 0x48;

    // Open the i2c device
    file = open("/dev/i2c-1", O_RDWR);
    if (file < 0){
        printf("ERROR open(%d)failed\n", file);
        exit(1);
    }

    // Test if chip address is in range
    if (address < 0  || address > 119){
        printf ("Chip address is out of 0x00 - 0x77 range!\n");
        exit(-1);
    }

    //Specify address of the i2c chip to communicate with
    if (ioctl(file, I2C_SLAVE, address) < 0){
        printf("IOCTL failed\n");
        exit(1);
    }

    //Test out of range values
    if ( conf_reg > 255 || conf_reg < 0 ){
        printf ("The CONFIG_REGISTER value is out of range!\n");
        exit (-1);
    } else {
       i2c_smbus_write_word_data(file, 0x01, conf_reg);
    }

    if ( temp_low > 65535 || temp_low < 0 ){
        printf ("The TEMP_LOW_REGISTER register value is out of range!\n");
        exit (-1);
    } else {
        i2c_smbus_write_word_data(file, 0x02, temp_low);
    }

    if ( temp_high > 65535 || temp_high < 0 ){
        printf ("The TEMP_HIGH_REGISTER value is out of range!\n");
        exit (-1);
    } else {
        i2c_smbus_write_word_data(file, 0x03, temp_high);
    }

    //Read the Temperature register
    while(1){
        res = i2c_smbus_read_word_data(file, reg);
        if (res < 0){
            printf("SMBus read failed\n");
            exit(1);
        } else {
            b1 = (res & 0X000000ff) << 4;
            b0 = (res & 0X0000f000) >> 12;
            t = (b0 + b1) * 0.0625;
            printf ("The temperature is: %.4f °C\n", t);
        }
        sleep(1);
    }

    return 0;
}
