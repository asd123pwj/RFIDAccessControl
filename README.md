# RFIDAccessControl
An access control project base on RC522, Arduino Nano, ESP8266 and SG90.

# Equipment
Arduino Nano V3.0 * 1  
MFRC-522 * 1  
NodeMCU Lua V3 * 1 (ESP8266)  
SG90 * 1  
Dupont line  

# Burning Platform
Arduino 1.8.13

Arduino Nano V3.0:  
Board: Arduino Nano    Processor: ATmega328P(Old Bootloader)

NodeMCU Lua V3.0:  
Board: NodeMCU V1.0

# How can them do
When RC522 read an accessful card, Nano will make SG90 turn, then, SG90 turn 180 degrees, and wait 1 second it will turn back.
In the meanwhile, you can turn SG90 like that by connecting wifi ESP8266Web and open 192.168.4.1(local address) to click the click button.

# CHINESE
中文介绍与使用方法请看我的博客：http://mwhls.top/%e5%b8%a6%e8%81%94%e7%bd%91%e5%8a%9f%e8%83%bd%e7%9a%84rfid%e5%ae%bf%e8%88%8d%e9%97%a8%e7%a6%81
更加详细，移动网打不开请看CSDN这里：https://blog.csdn.net/asd123pwj/category_10484364.html
