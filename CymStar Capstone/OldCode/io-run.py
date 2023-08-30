#!/usr/bin/python3
from uio.ti.icss import Icss
from ctypes import c_uint32
from ctypes import c_uint32 as u32, c_uint16 as u16, c_uint8 as u8
import ctypes
import struct
import time
import socket
import sys
def read_SA_from_file():
    rx = [None] * 30
    tx = [None] * 30
    
    f = open("config.txt", "r")
    if (not f.closed):
        configs = f.readlines()
        configs = [config.strip() for config in configs]
        terminalAddress = configs[0];   # First line is always RT address
        # Loop over remaining lines in configs
        read_rx = 0;
        read_tx_header = 1;
        for line in configs[1::]: # Don't inclue first line, it is already read
            if (line == "T"):           # if a line is T, read T address
                read_rx = 0
            elif (line == "R"):          # if a line is R, read R address
                read_rx = 1
            elif (read_rx):             # Read Rx Header
                (add, count) = line.split()
                rx[int(add)] = int(count)
            elif (read_tx_header):  # Read Tx Header
                read_tx_header = not read_tx_header
                (add, count) = line.split()
                tx[int(add)] = int(count)
            elif (not read_tx_header):  # Read Tx words
                read_tx_header = not read_tx_header
                words = line.replace(" ", "")
                tx[int(add)] = (int(count), int(words, 16))
    count = 0;
    
    
    f.close();		# close the file object.
    return (terminalAddress,rx,tx)
#--- Pru status definitions ---
#define TEST_STATUS 		999
#define IDLE_STATUS 		1000
#define CW_RECEIVE_STATUS	1010
#define SW1_TRANSMIT_STATUS 1020
#define DW_TRANSMIT_STATUS	1030
#define DW_RECEIVE_STATUS	1040
#define SW2_TRANSMIT_STATUS 1050
TEST_STATUS = 999
IDLE_STATUS = 1000
CW_RECEIVE_STATUS = 1010
SW1_TRANSMIT_STATUS = 1020
DW_TRANSMIT_STATUS = 1030
DW_RECEIVE_STATUS = 1040
SW2_TRANSMIT_STATUS = 1050#!/usr/bin/python3
from uio.ti.icss import Icss
from ctypes import c_uint32
from ctypes import c_uint32 as u32, c_uint16 as u16, c_uint8 as u8
import ctypes
import struct
import time
def read_SA_from_file():
    rx = [None] * 30
    tx = [None] * 30
    
    f = open("config.txt", "r")
    if (not f.closed):
        configs = f.readlines()
        configs = [config.strip() for config in configs]
        terminalAddress = configs[0];   # First line is always RT address
        # Loop over remaining lines in configs
        read_rx = 0;
        read_tx_header = 1;
        for line in configs[1::]: # Don't inclue first line, it is already read
            if (line == "T"):           # if a line is T, read T address
                read_rx = 0
            elif (line == "R"):          # if a line is R, read R address
                read_rx = 1
            elif (read_rx):             # Read Rx Header
                (add, count) = line.split()
                rx[int(add)] = int(count)
            elif (read_tx_header):  # Read Tx Header
                read_tx_header = not read_tx_header
                (add, count) = line.split()
                tx[int(add)] = int(count)
            elif (not read_tx_header):  # Read Tx words
                read_tx_header = not read_tx_header
                words = line.replace(" ", "")
                tx[int(add)] = (int(count), int(words, 16))
    count = 0;
    
    
    f.close();		# close the file object.
    return (terminalAddress,rx,tx)
#--- Pru status definitions ---
#define TEST_STATUS 		999
#define IDLE_STATUS 		1000
#define CW_RECEIVE_STATUS	1010
#define SW1_TRANSMIT_STATUS 1020
#define DW_TRANSMIT_STATUS	1030
#define DW_RECEIVE_STATUS	1040
#define SW2_TRANSMIT_STATUS 1050
TEST_STATUS = 999
IDLE_STATUS = 1000
CW_RECEIVE_STATUS = 1010
SW1_TRANSMIT_STATUS = 1020
DW_TRANSMIT_STATUS = 1030
DW_RECEIVE_STATUS = 1040
SW2_TRANSMIT_STATUS = 1050
pruss = Icss( "/dev/uio/pruss/module" )
pruss.initialize()
core = pruss.core0
core.load( 'fw/am335x-pru0-fw' )
#--- Map variables in pru memory ---
class SubAdr( ctypes.Structure ):
    _fields_ = [
            ( 'adr',      u8),
            ( 'wordCnt',  u8),
            ( 'data', u16 * 30),
        ]
        
class RemoteTerminal (ctypes.Structure ):
    _fields_ = [
            ('address', u8),
	        ('rxaddresses', SubAdr * 30),
	        ('txaddresses', SubAdr * 30),
        ]
gpio_0 = core.map( c_uint32, 0x10000 ) # gpio_0 - Data from gpio_0 register pins, for direct troubleshooting
gpio_1 = core.map( c_uint32, 0x10004 ) # gpio_1 - Data from gpio_1 register pins, for direct troubleshooting
gpio_2 = core.map( c_uint32, 0x10008 ) # gpio_2 - Data from gpio_2 register pins, for direct troubleshooting
status = core.map( c_uint32, 0x1000C ) # status - Current status of PRU (IDLE/INIT/TRANSMIT/RECEIVE/ERR)
rxdata = core.map( c_uint32, 0x10010 ) # rxdata - Received of SYNC and 16 bit word data
txdata = core.map( c_uint32, 0x10014 ) # txdata - Transmit of SYNC and 16 bit word data 
errmsg = core.map( c_uint32, 0x10018 ) # errmsg - Error code reporting if error detected
cmdmsg = core.map( c_uint32, 0x1001C ) # cmdmsg - Command message from client to PRU (IDLE/INIT/TRANSMIT/RECEIVE/ERR)
rxtick = core.map( c_uint32, 0x10020 ) # rxtick - how many ticks of words received
rcvahi = core.map( c_uint32, 0x10024 ) # rcvahi - if RCVA is high
samout = core.map( c_uint32, 0x10028 ) # samout - Status and Mode Register Output
subadrx = core.map( RemoteTerminal,   0x11000 ) # subadrx - Structure of rx subaddresses
subadrx.address = 20
print(f"RT ADDRESS {subadrx.address}")
(terminalAddress, rx, tx) = read_SA_from_file()
print("terminal address:")
print(terminalAddress)
print("rx addresses:")
print(rx)
print("tx addresses:")
print(tx)
    
#--- print initial gpio_0 values on startup, wait for input ---
print( "gpio_0 = %d,  gpio_1 = %d, gpio_2 = %d" % ( gpio_0.value, gpio_1.value, gpio_2.value ) )
input("Press Enter to run core...")
core.run()
start_time = time.time()
wait_time = time.time()
old_0 = gpio_0.value
old_1 = gpio_1.value
old_2 = gpio_2.value
old_status = status.value
old_rxdata = rxdata.value
old_errmsg = errmsg.value
old_rxtick = rxtick.value
old_rcvahi = rcvahi.value
txdata.value = 0
cmdmsg.value = IDLE_STATUS
print('Cmdmsg Value', cmdmsg.value)
print( "waiting for core to change memory value" )
f = open("commands.txt", "r")
commands = f.readlines()
TEST = 1000;
print(f"Signaling {cmdmsg.value} command. Waiting for core status to change")
while status == 0:
    pass
print(f"core status is {status.value}")
# Create a TCP/IP
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Connect the socket to the port where the server is listening
server_address= ('192.168.0.30', 8081)
print('connecting to %s port %s' % server_address)
sock.connect(server_address)
word = 0xfa10
data = [0]*100
tick = [0]*100
oldtick = rxtick.value;
for sends in range(0,10):
    print('waiting for words to receive')
    for index in range(0,100):
        while rxtick.value == oldtick:
            print(f'status {status.value}, rxdata { rxdata.value }, rcvahi { rcvahi.value }, samout {samout.value}, errmsg {errmsg.value}')
            oldtick = rxtick.value
            data[index] = rxdata.value
            tick[index] = rxtick.value
    try:
        for word in data:             # Send all data
            message = str(word).encode()
            print(data)
            print('sending "%s"' % message)
            sock.sendall(message)
            # Look for the response
            amount_received = 0
            amount_expected = len(message)
            while amount_received< amount_expected:
                data = sock.recv(16)
                amount_received+= len(data)
                print('received "%i"' % int(data))
    finally:
        pass
    
time.sleep(0.25)
sock.close()
		
"""
while True:
    if time.time() - wait_time > 3:
        wait_time = time.time()
        print(subadrx.rxaddresses[0].data[0]);
        print(f"\r\nstatus: {status.value}, rxdata: {hex(rxdata.value)}, rxtick: {rxtick.value}, txdata: {txdata.value}, errmsg: {errmsg.value}, cmdmsg: {cmdmsg.value}, rcvahi: {rcvahi.value}. time : {time.time() - start_time}")
    if (rxdata.value != old_rxdata):
        print(f"\r\nrxdata changed from {hex(old_rxdata)} to {hex(rxdata.value)}. Status {status.value} | time : {time.time() - start_time}\n")
        old_rxdata = rxdata.value
    if (rcvahi.value != old_rcvahi):
        print(f"\r\nrcvahi changed from {old_rcvahi} to {rcvahi.value} | time : {time.time() - start_time}\n")
        old_rcvahi = rcvahi.value
    if (status.value != old_status):
        print(f"\r\nstatus changed from {old_status} to {status.value} | time : {time.time() - start_time}\n")
        old_status = status.value
    if (errmsg.value != 0):
        pass
        #print(f"\errmsg returned {errmsg.value}.\n")
        #errmsg.value = 0; # After acknowledging error, clear it
"""
