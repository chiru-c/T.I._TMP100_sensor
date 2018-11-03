# T.I._TMP100_sensor
A user-space application that will display the temperature read from the Texas Instruments TMP100 temperature sensor using I2C bus to communicate. The application takes 3 input arguments to configure the sensor before utilizing it.

An example on how to call the application from command line:

$ ./tmp100_i2c -A 73 -C 124 -L 0000 -H 65500

Output of application call:

The selected chip-address is: 73
The CONFIG_REGISTER value is: 124
The TEMP_LOW_REGISTER value is: 0000
The TEMP_HIGH_REGISTER value is: 65500
The value stored in the parameters: 0x49 0x7c 0000 0xffdc !

