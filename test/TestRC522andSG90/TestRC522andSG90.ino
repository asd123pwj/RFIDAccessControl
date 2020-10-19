#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 8
#define USER_NUM 3

byte servoRun = 0;              // 舵机是否运行。
MFRC522 rfid(SS_PIN, RST_PIN);  // 实例化类
byte userCard[USER_NUM][4] = {  // 通行卡存储数组。
  {28,   184,  119,  33},
  {249,  231,  71,   179},
  {109,  242,  234,  235}
};
 
void setup() { 
  Serial.begin(9600);           // 波特率设置
  SPI.begin();                  // 初始化SPI总线
  rfid.PCD_Init();              // 初始化 MFRC522 
  pinMode(SERVO_PIN, OUTPUT);   // 舵机控制端口
}
 
void loop() {
  if(servoRun == 1){            // 舵机运行变量若为1，则运行。
    servoControl();             // 舵机控制函数。
  }
  
  if ( ! rfid.PICC_IsNewCardPresent()) return;       // 找卡
  if ( ! rfid.PICC_ReadCardSerial())   return;       // 验证NUID是否可读
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  // 检查是否MIFARE卡类型
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Can identify this card!");
    return;
  }
  
  byte i;
  for (i=0; i<USER_NUM;  i++){    // 判断卡是否为通行卡。
    byte i2;
    for(i2=0;  i2<4; i2++){       // 遍历userCard中所有卡。
      if(rfid.uid.uidByte[i2] != userCard[i][i2]) break;  // break时，表示此卡不是通行卡。
    }
    if(i2 == 4){                  // i2为4，表示此卡的四位值都验证通过。
      Serial.println("Find an accessful card.");  // 输出成功信息。
      servoRun = 1;               // 将开门变量置1。
      break;                      // 已找到通行卡，跳出循环。
    }
  }
  if(i==USER_NUM){                // 若i等于通行用户数量，则上层循环未找到通行卡。
    Serial.print("Find a unknown card, its uid:");
    for(i=0;  i<4;  i++){         // 输出此卡UID。
      Serial.print(rfid.uid.uidByte[i], DEC);
      Serial.print(" ");
    }
    Serial.println();
  }
  
  rfid.PICC_HaltA();              // 使放置在读卡区的IC卡进入休眠状态，不再重复读卡
  rfid.PCD_StopCrypto1();         // 停止读卡模块编码

}


void servoControl(){              // 舵机控制函数。
  Serial.println("Servo run!");   // 函数运行输出标识。
  servoPulse(0);                  // 舵机转至0度。
  delay(1000);                    // 等待舵机运转。
  servoPulse(180);                // 舵机转至180度。
  servoRun = 0;                   // 舵机运行变量置零。
}

void servoPulse(int myangle)      // 定义一个脉冲函数，作者：https://blog.csdn.net/sss_369/article/details/52894347
{
  int pulseWidth=(myangle*11)+500;// 将角度转化为500-2480 的脉宽值
  digitalWrite(SERVO_PIN,HIGH);   // 将舵机接口电平至高
  delayMicroseconds(pulseWidth);  // 延时脉宽值的微秒数weimiao
  digitalWrite(SERVO_PIN,LOW);    // 将舵机接口电平至低
  delay(20-pulseWidth/1000);
}
