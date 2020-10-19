/* 作者:MWHLS,主页MWHLS.TOP
 * 链接：http://mwhls.top/?p=659
 * 通过主机连接上ESP8266热点，进入通信网址，点击CLICK按钮后，
 * 一个消息会使用I2C通信传输给Arduino，这个消息会触发舵机转动。
 * 参考文章：
 * I2C通信：https://blog.csdn.net/qq_44506730/article/details/90578507
 * 网页控制：https://blog.csdn.net/qq_46292418/article/details/106605366
 */
#include <ESP8266WiFi.h>            // 本程序使用 ESP8266WiFi库
#include <ESP8266WebServer.h>       // ESP8266WebServer库
#include <Wire.h>                   // 使用Wire.h进行I2C通信

ESP8266WebServer esp8266_server(80);// 建立ESP8266WebServer对象，对象名称为esp8266_server
                                    // 括号中的数字是网路服务器响应http请求的端口号
                                    // 网络服务器标准http端口号为80，因此这里使用80为端口号
#define WIFISSID "ESP8266Web"       // 设定ESP8266 wifi名称
#define WIFIPSSD "123456789"        // 设定wifi密码
 
void setup(void){
  Serial.begin(9600);               // 启动串口通讯
  Wire.begin(D1,D2);                // I2C通信端口
  setAP();                          // 设定AP模式，并建立热点。
  
//--------"启动网络服务功能"程序部分开始--------           // 此部分为程序为本示例程序重点1
  esp8266_server.begin();                             // 详细讲解请参见太极创客网站《零基础入门学用物联网》
  esp8266_server.on("/", HTTP_GET, handleRoot);       // 第3章-第2节 ESP8266-NodeMCU网络服务器-1
  esp8266_server.on("/CLICK", HTTP_POST, handleClick);// 处理用户点击消息。
  esp8266_server.onNotFound(handleNotFound);          // 404处理。
//--------"启动网络服务功能"程序部分结束--------
  Serial.println("HTTP esp8266_server started");      // 告知用户ESP8266网络服务功能已经启动
}
 
void loop(void){
  esp8266_server.handleClient();                      // 处理http服务器访问
}
                                                                          
void setAP(){
  WiFi.mode(WIFI_AP);                                 // 设定ESP8266的AP模式
  WiFi.softAP(WIFISSID,WIFIPSSD);                     // 设定ESP8266热点
  Serial.printf("Success!\nWIFISSID:  %s \nWIFIPSSD:  %s \nControlWeb:  ", WIFISSID, WIFIPSSD);
  Serial.println(WiFi.softAPIP());                    // 接上行，输出WIFI信息与通信网址。
}


void handleRoot() {                                   // 处理网站根目录“/”的访问请求 
  esp8266_server.send(200, "text/html", "<form action=\"/CLICK\" method=\"POST\"><input type=\"submit\" value=\"CLICK\"></form>");
}

void handleClick(){                                   // 点击消息处理函数
  Serial.println("Click.");                           
  clickTransmission();                                // 传输点击消息到Arduino板。
  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转  
}

void clickTransmission(){                             // 点击消息传输函数
  Wire.beginTransmission(8);                          // 开始传输
  Wire.write('1');                                    // 传 1 至Arduino版
  Wire.endTransmission();                             // 结束传输
  Serial.println("Click transmiss success.");         // 输出成功信息
}
 
// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
