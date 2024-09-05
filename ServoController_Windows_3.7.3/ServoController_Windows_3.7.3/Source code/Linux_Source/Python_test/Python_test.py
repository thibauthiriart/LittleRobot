#!/usr/bin/python3

import serial 

def write_instruction(serialport,instruction):
    serialport.write(instruction.encode("utf-8"))
    while True:
        str=serialport.readall().decode("utf-8")
        if(str=="OK"):
            break
    print("instruction execution successful for:")
    print(instruction)

try:
    serialport=serial.Serial("/dev/ttyACM0",115200,timeout=0.1)
    
    #Execution of preset instructions
    write_instruction(serialport,"#1P2000T1000D100\r\n")
    write_instruction(serialport,"#1P1000#2P666T1000D1000\r\n")

    #Test for action groups,Group 1 run 3 times
    write_instruction(serialport,"G1F3\r\n")

    serialport.close()

except Exception as e:
    print("error：",e)
