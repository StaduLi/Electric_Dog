#ifndef _WIFI_H
#define _WIFI_H
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "inc/basic.h"

// 需要注意在更改运动状态前要进行stop操作

const char *AP_NAME = "Electric Dog"; // wifi名字

const byte DNS_PORT = 53;       // DNS端口号
IPAddress apIP(192, 168, 5, 1); // esp8266-AP-IP地址

DNSServer dnsServer;         // 创建dnsServer实例
ESP8266WebServer server(80); // 创建WebServer

class Web
{
public:
  int getConnectUserNum()
  {
    return WiFi.softAPgetStationNum();
  }
  void dnsServerInit()
  {
    if (!Serial)
      Serial.begin(115200);

    initSoftAP();
    initWebServer();
    initDNS();
  }

  void handleRequest()
  {
    server.handleClient();
    dnsServer.processNextRequest();
  }

private:
  static void handleGetDogState()
  { // 设置请求头以便客户端进行异源请求
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    String sendMessage;
    if (isCharging())
      sendMessage = "CHARGING";
    else if (isLowBattery())
      sendMessage = "LOW_BATTERY";
    else
      sendMessage = "NULL";
    Serial.println(getBatteryVoltage());
    server.send(200, "text/plain", sendMessage);
  }

  static void handleAction()
  {
    run_times = 0;
    String action_code = server.arg("action_code");
    run_mode = action_code.toInt() + 1;
    Serial.println("run mode= " + String(run_mode));

    // 设置请求头以便客户端进行异源请求
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    String sendMessage = "run_mode = " + String(run_mode) + " have been upload";
    server.send(200, "text/plain", sendMessage);
  }

  static void handleRun()
  {
    run_mode = 0;
    String rec = server.arg("run_code");
    run_code = rec.toInt();
    Serial.println("run code= " + String(run_code));

    // 设置请求头以便客户端进行异源请求
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    String sendMessage = "run_code=" + String(run_code) + "have been upload";
    server.send(200, "text/plain", sendMessage);
  }

  // 获取文件类型
  static String getContentType(String filename)
  {
    if (filename.endsWith(".htm"))
      return "text/html";
    else if (filename.endsWith(".html"))
      return "text/html";
    else if (filename.endsWith(".css"))
      return "text/css";
    else if (filename.endsWith(".js"))
      return "application/javascript";
    else if (filename.endsWith(".png"))
      return "image/png";
    else if (filename.endsWith(".gif"))
      return "image/gif";
    else if (filename.endsWith(".jpg"))
      return "image/jpeg";
    else if (filename.endsWith(".ico"))
      return "image/x-icon";
    else if (filename.endsWith(".xml"))
      return "text/xml";
    else if (filename.endsWith(".pdf"))
      return "application/x-pdf";
    else if (filename.endsWith(".zip"))
      return "application/x-zip";
    else if (filename.endsWith(".gz"))
      return "application/x-gzip";
    return "text/plain";
  }

  static bool handleFileRead(String path)
  { // 处理浏览器HTTP访问

    if (path.endsWith("/"))
    {                         // 如果访问地址以"/"为结尾
      path = "/control.html"; // 则将访问地址修改为/index.html便于SPIFFS访问
    }

    String contentType = getContentType(path); // 获取文件类型

    // FOR TEST
    Serial.println("doc type : " + contentType);

    if (SPIFFS.exists(path))
    {
      Serial.println("find " + path);       // 如果访问的文件可以在SPIFFS中找到
      File file = SPIFFS.open(path, "r");   // 则尝试打开该文件
      server.streamFile(file, contentType); // 并且将该文件返回给浏览器
      file.close();                         // 并且关闭文件
      return true;                          // 返回true
    }
    else
    {
      Serial.println("NOT FIND " + path);
      Serial.println("WILL OPEN control.HTML");
      File file = SPIFFS.open("/control.html", "r"); // 则尝试打开该文件
      server.streamFile(file, "text/html");          // 并且将该文件返回给浏览器
      file.close();                                  // 并且关闭文件
      return true;                                   // 返回true
    }
  }

  static void handleUserRequet()
  {
    // FOR TEST
    Serial.println("User visit");

    // 获取用户请求网址信息
    String webAddress = server.uri();

    // FOR TEST
    Serial.println("User request : " + webAddress);

    // 通过handleFileRead函数处处理用户访问
    bool fileReadOK = handleFileRead(webAddress);

    // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
    if (!fileReadOK)
    {
      server.send(404, "text/plain", "404 Not Found");
    }
  }

  // 服务器回调函数配置及初始化

  static void initWebServer(void)
  { // 初始化WebServer
    // server.on("/",handleRoot);
    // 上面那行必须以下面这种格式去写否则无法强制门户
    server.on("/", HTTP_GET, handleUserRequet); // 设置主页回调函数
    server.onNotFound(handleUserRequet);

    server.on("/run", handleRun);
    server.on("/action", handleAction);
    server.on("/getDogState", handleGetDogState);

    server.begin(); // 启动WebServer
    Serial.println("WebServer started!");
  }

  static void initSoftAP(void)
  { // 初始化AP模式
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    if (WiFi.softAP(AP_NAME))
    {
      Serial.println("ESP8266 SoftAP is right");
    }
  }
  static void initDNS(void)
  { // 初始化DNS服务器
    if (dnsServer.start(DNS_PORT, "*", apIP))
    { // 判断将所有地址映射到esp8266的ip上是否成功
      Serial.println("start dnsserver success.");
    }
    else
      Serial.println("start dnsserver failed.");
  }
};

#endif // WIFI_H
