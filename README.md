# OSDriverDemonstration
A repository for tracking and storing driver demonstrations written for Operating Systems semester project.

### Includes:
  - Character Device Driver - Interfaces with /dev/charDevice to read and write byte data
  - Block Device Driver - Interfaces with an allocated memory block to read and write block data
  - Network Device Driver - Creates a virtual network interface to display connection information
    
### Installation
  1. ```git clone https://github.com/malottno/OSDriverDemonstration```
  2. ```cd OSDriverDemonstration```
  3. ```./buildAll.sh```
    
### Build Options (may require root privileges)
  - `<make|./buildAll.sh> clean` to remove build files and any installed modules
  - `<make|./buildAll.sh> install` to build and insert modules
  - `<make|./buildAll.sh> test` to build, install, and run test scripts
  
## Character Device Driver
#### Overview
The character device driver implements a readable and writable null device driver that tracks buffered data that has been sent to the device. A parameter for the character buffer size, not to exceed 1MB, is used to create an internal buffer for the driver. As data is written into the buffer from devices the buffer will overflow and begin to write over itself. Applications may read from the driver, and access is not strictly limited to a single process/thread at a time. A mutex lock for the buffer is implemented to protect the buffer during changes in case of multiple accessing threads.

#### Design
The driver implements 3 major functions:
* c_read - called when a program is requesting to read from the driver's buffer
* c_write - called when a program is requesting to write to the driver's buffer
* c_open - called when a program first opens the driver as a system file

An initialization and cleanup module were also implemented to facilitate loading and unloading of the kernel module.

#### Test Suite
Requirement | Satisfied in | Status
--- | --- | ---
Driver must be able to read in a stream of data | charTest.c | TODO
Driver must be able to write out a stream of data | charTest.c | TODO
Driver must be able to connect to a serial port | charTest.c | TODO
Driver must track statistics, including: bytes tx, bytes rx, average processing time | make test (shell) | TODO
Driver must be implemented as a kernel module | make test (shell) | TODO
Driver must allow for multiple-process access | make test (shell) | TODO


  
  
## Block Device Driver
#### Overview

#### Design

#### Test Suite
Requirement | Satisfied in | Status
--- | --- | ---
Driver must store labels for allocated memory | blockTest.c | TODO
Driver must allocate memory when directed allocation size is less than 1024 Bytes | blockTest.c | TODO
Driver must implement a file system with allocation size of 1MB and blocks of 1024 Bytes | blockTest.c | TODO
Driver must free memory when directed | blockTest.c | TODO
Driver must track statistics, including: chunks free, bytes free, largest block available, average allocation time | blockTest.c | TODO
Driver must be implemented as a kernel module | make test (shell) | TODO
Driver must allow for multiple-process access | make test (shell) | TODO


## Network Device Driver
#### Overview


#### Design
The driver implements 4 major functions:
* net_open - called when a program is requesting to open the network interface
* net_stop - called when a program is requesting to close the network interface
* net_device_stats - called when a program requests statistics on the network interface
* net_start_xmit - called when a network interface is enabled

An initialization and cleanup module were also implemented to facilitate loading and unloading of the kernel module.

#### Test Suite
Requirement | Satisfied in | Status
--- | --- | ---
Driver must create a network interface | make test (shell) | TODO
Driver must capture packet data on network interface | netTest.c | TODO
Driver must track statistics, including: recent communications, total packets analyzed, analysis time | netTest.c | TODO
Driver must be implemented as a kernel module | make test (shell) | TODO
Driver must allow for multiple-process access | make test (shell) | TODO





