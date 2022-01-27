# iow_sht7x - Node.js module for Sensirion SHT7X with IOW158A-S 

This Node module is intended for use with a [Sensiron SHT7x](https://www.farnell.com/datasheets/1563784.pdf) sensor IC that is coupled with a Code Mercenaries IOW158A-S.

## Prerequisites
* This module requires the installation of the [Code Mercenaries IO-Warrior SDK (Linux)](https://www.codemercs.com/downloads/iowarrior/IO-Warrior_SDK_linux.zip) version 1.7.

## Installation

After downloading the IO-Warrior SDK, upzip into any folder and run the following:

```
./configure
make
sudo make install
```
It is expected that the binaries and includes go into standard locations (`/usr/lib`, `/usr/include`)

### Installing module from npm registry
```
npm install --save @devonho/iow_sht7x
```

### Installing module from source

Installing within Node.js project

```
git clone https://github.com/devonho/iow_sht7x.git
npm install ./iow_sht7x
```


## Limitations
* This only runs on Linux currently.

## Credits

* The key I2C bytes sequences that made this work were from [this post](https://forum.codemercs.com/viewtopic.php?t=1694). Thanks to user "Christof" for sharing his VBA code.
* A large part of the the code was referenced from samples from [Code Mercenaries IO-Warrier SDK (Windows)](https://www.codemercs.com/downloads/iowarrior/IO-Warrior_SDK_win.zip)


