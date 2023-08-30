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
txdata = core.map( c_uint32, 0x10014 ) # txdata - Transmit of 16 bit word data for RT test
errmsg = core.map( c_uint32, 0x10018 ) # errmsg - Error code reporting if error detected
cmdmsg = core.map( c_uint32, 0x1001C ) # cmdmsg - Command message from client to PRU (IDLE/INIT/TRANSMIT/RECEIVE/ERR)
rxtick = core.map( c_uint32, 0x10020 ) # rxtick - how many ticks of words received
rcvahi = core.map( c_uint32, 0x10024 ) # rcvahi - if RCVA is high
samout = core.map( c_uint32, 0x10028 ) # samout - Status and Mode Register Output
syncin = core.map( c_uint32, 0x1002C ) # syncin - SYNC information input for RT test 
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
#192.168.0.30
server_address= ('localhost', 10000)
print('connecting to %s port %s' % server_address)
sock.connect(server_address)
data = [0]*100
tick = [0]*100
oldtick = rxtick.value;
rtadd = 20
transmit = 1
receive = 0
subadd = 1
wordcnt = 15
rxwordcnt = 13
subadrx.txaddresses[subadd].word_count = wordcnt
subadrx.txaddresses[1].word_count = 15;
subadrx.rxaddresses[1].word_count = 13;
commandword   = (rtadd << 11) | (transmit << 10)  | (subadd << 5) | wordcnt
rxcommandword = (rtadd << 11) | (receive << 10) | (subadd << 5) | rxwordcnt
# T/R = 1
# sub address = 1
# word count = 13
print(rxcommandword)
# 0x1111 is the status word in this case
test_words = [commandword,0x0000, 0x1111,0x2222,0x3333,0x4444,0x5555,0x6666,0x7777,0x8888,0x9999,0xAAAA,0xBBBB,0xCCCC,0xDDDD,0xEEEE, 0xFFFF]
test_syncs = [          1,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,      0,      0]
rxtest_words = [rxcommandword, 0x0000, 0x1243, 0x5678, 0xabcd, 0xdeed, 0xfeef, 0x1783, 0x1fff, 0x9381, 0x1293, 0x439e, 0x23e4, 0x1283, 0x1111]
print(commandword)
wait_time = time.time()
for sends in range(0,100):
    print('waiting for command to transmit')
    # Test TX transmit RT control loop
    for index in range(0,len(test_words)): # +2 to include command word and status word
        while rxtick.value == oldtick:
            if time.time() - wait_time > 0.66:
                #print(f'RT {subadrx.address} | RTWordCount {gpio_0.value}, WordCountReq {gpio_1.value}, GPIO2 {gpio_2.value} | txdata {txdata.value}, status {status.value}, rxtick { rxtick.value}, rxdata { rxdata.value }, rcvahi { rcvahi.value }, samout {samout.value}, errmsg {errmsg.value}')
                wait_time = time.time();
        print(f'RT {subadrx.address} | txdata {hex(txdata.value)}, status {status.value}, rxtick { rxtick.value}, rcvahi { rcvahi.value }, samout {samout.value}, errmsg {errmsg.value}')
        print(f'sync {syncin.value} | GPIO0 {gpio_0.value}, GPIO1 {gpio_1.value}, GPIO2 {gpio_2.value}')
        syncin.value = test_syncs[index]
        txdata.value = test_words[index]
        oldtick = rxtick.value
        data[index] = rxdata.value
        tick[index] = rxtick.value
    time.sleep(1)
    print('waiting for command to receive')
    # Test RX transmit RT control loop
    for index in range(0,len(rxtest_words)):
        while rxtick.value == oldtick:
             if time.time() - wait_time > 0.66:
                #print(f'RT {subadrx.address} | RTWordCount {gpio_0.value}, WordCountReq {gpio_1.value}, GPIO2 {gpio_2.value} | txdata {txdata.value}, status {status.value}, rxtick { rxtick.value}, rxdata { rxdata.value }, rcvahi { rcvahi.value }, samout {samout.value}, errmsg {errmsg.value}')
                wait_time = time.time();
        print(f'RT {subadrx.address} | rxdata {hex(rxdata.value)}, status {status.value}, rxtick { rxtick.value}, rcvahi { rcvahi.value }, samout {samout.value}, errmsg {errmsg.value}')
        print(f' sync {syncin.value} | GPIO0 {gpio_0.value}, GPIO1 {gpio_1.value}, GPIO2 {gpio_2.value}')
        syncin.value = test_syncs[index]
        txdata.value = rxtest_words[0] # transmitted word on bus = command word
        rxdata.value = rxtest_words[index] # word received by PRU = data word
        oldtick = rxtick.value
        data[index] = rxdata.value
        tick[index] = rxtick.value
    print(data)
    print(tick)
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
