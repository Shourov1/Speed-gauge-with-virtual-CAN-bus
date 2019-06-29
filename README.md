# Speed Gauge With Virtual CAN Bus

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

The purpose of this application is to show (simulated) vehicle speed in a gauge developed with [Qt5/ QML](https://doc.qt.io/qt-5/qtqml-index.html).

The application can communicate with a [virtual CAN bus](https://en.wikipedia.org/wiki/SocketCAN), from which the application is receiving simulated vehicle speed data. The application then shows the vehicle speed in a gauge. The UI is developed with [QML](https://en.wikipedia.org/wiki/QML). 


## How to start

### To activate virtual CAN bus interface up:

```bash
modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```
## CAN communications: Linux CAN command-line tools:

In order to test the CAN communication, we are using the can-utils command line CAN tools. These are used similarly on real and simulated CAN buses. For example, one of the tools is candump which allows you to print all data that is being received by a CAN interface.

### In order to test this facility, start it in a terminal window:

```bash
candump vcan0
```

### From another terminal window, send a CAN frame with identifier 0x1A (26 dec) and 8 bytes of data:

```bash
cansend vcan0 01a#11223344AABBCCDD
```
This will appear in the first terminal window (running candump):  vcan0  01A   [8]  11 22 33 44 AA BB CC DD

### To send large amount of random CAN data, use the cangen tool:

```bash
cangen vcan0 -v
```

### In order to record this type of received CAN data to file (including timestamp), use:

```bash
candump -l vcan0
```
The resulting file will be named like: candump-2019-06-23_123001.log


### In order to print logfiles in a user friendly format:

```bash
log2asc -I candump-2015-03-20_123001.log vcan0
```
### Recorded CAN log files can also be re-played back to the same or another CAN interface:

```bash
canplayer -I candump-2015-03-20_123001.log
```


## Built With
* [ubuntu](https://packages.ubuntu.com/bionic-updates/linux-headers-4.15.0-52-generic).
* [Qt5](https://wiki.qt.io/Qt_5.13_Release).

## Author

* **Md Shariful Alam** - [Shariful](https://github.com/Shourov1)

## Acknowledgments

* [Marko Mattila](https://github.com/zchydem) - for providing the Gauge design using QML.

* [Ryan Fabac](https://github.com/rmfabac) - for providing the CAN bus functionality test examples.


