/*
 * Arduino_Software_Serial.ino
 * Copyright (c) 2014-2020 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 */
 
 #include <SoftwareSerial.h>
 
 SoftwareSerial SoftSerial(2, 3); // RX, TX

void wait_serial_return_ok()
{
  int num=0;
  char c[16];
  while(1)
  {
    while(SoftSerial.available() > 0)
    {
      c[num]=SoftSerial.read();
      num++;
      if(num>=15)
        num=0;
    }
    if(c[num-2]=='O'&&c[num-1]=='K')
      break; 
  }
}

void setup() {
  SoftSerial.begin(9600);
  delay(1000);
}

void loop() {
  SoftSerial.print("#3P500T2000\r\n");
  //Waiting to return OK
  wait_serial_return_ok();
  
  SoftSerial.print("#3P1500#4P1100T2000\r\n");  
  wait_serial_return_ok();
  
  SoftSerial.print("#3P2500T2000D1555\r\n");  
  wait_serial_return_ok();
  
  SoftSerial.print("#3P1500T2000\r\n");
  wait_serial_return_ok();
  
  //Test for send wrong instruction
  SoftSerial.print("#3P2500T9000D9000\r\n");
  
  //Stop wrong instruction
  SoftSerial.print("~ST");
  
  //Change right instruction
  SoftSerial.print("#2P2500T9000D9000\r\n");
  wait_serial_return_ok();
  
  //Test for action groups,Group 1 run 3 times
  SoftSerial.print("G1F3\r\n");
  wait_serial_return_ok();
}
