 #include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>

const char *ssid     = "Vignesh galaxy A71 5G";
const char *password = "vignesh m d";

String date, Time;
int day, hour, minute;

int period = 1;
String student1 = "";
String student2 = "";
String student3 = "";
String student4 = "";
String student5 = "";
String student6 = "";
String student7 = "";
String student8 = "";
String student9 = "";
String student10 = "";

const char* host = "script.google.com";
const int httpsPort = 443;
String GAS_ID = "AKfycbwQVq9nLzKM6_MC5108jrS5qGWvXJA1huntwP3KtlqIH2hBknMgd5cy12FRV_UGYuuDvQ";


const int lateMinutes = 10;
const long utcOffsetInSeconds = 19800;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const int days = 7;
const int classes = 7;

String timeTable[days][classes] =
                         {
                           {"-1", "-1", "-1", "-1", "-1", "-1", "-1"},                    // Sunday
                           {"10:00", "08:05", "12:00", "14:00", "15:00", "16:00", "-1"},  // Monday
                           {"10:00", "11:00", "12:00", "14:00", "15:00", "21:10", "-1"},  // Tuesday
                           {"10:00", "11:00", "12:00", "14:00", "15:00", "16:00", "-1"},  // Wednesday
                           {"10:00", "11:00", "12:00", "14:00", "15:00", "16:00", "-1"},  // Thursday
                           {"10:00", "11:48", "12:40", "14:00", "15:00", "20:54", "-1"},  // Friday
                           {"11:16", "10:52", "10:57", "-1", "-1", "-1", "-1"}            // Saturday
                         };

WiFiClientSecure client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);
SoftwareSerial MCU(13, 12); //D7-RX, D6-TX
bool flag = 0;

void setup()
{
  Serial.begin(9600);
  MCU.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay (500);
    Serial.print (".");
  }
  Serial.println("");
  timeClient.begin();
  updateTime();
}

void loop()
{
  updateTime();
  routine(day);
  if(MCU.available()) readMCUdata();
  delay(1000);
}

void routine(int d)
{
  String t, th, tm;
  for(int j=0; j<classes; j++)
  {
    t = timeTable[d][j];
    if(t != "-1")
    {
      th = t;
      tm = t;
      th.remove(2,3);
      tm.remove(0,3);
      if((hour == th.toInt()) && (minute >= (tm.toInt())) && (minute < (tm.toInt() + lateMinutes)))
      {
        period = j+1;
        while(minute < (tm.toInt() + lateMinutes))
        {
          take_attendance(period);
          delay(10000);
          updateTime();
        }
        submit_attendance();
      }
    }
  }
}

void take_attendance(int p)
{
  Serial.println(p);
  MCU.println(p);
}

void submit_attendance()
{
  Serial.println(0);
  for(int i=0; i<500; i++)
  {
    MCU.println(0);
    delay(1);
    if(MCU.available()) break;
  } 
}

void updateTime()
{
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int month = ptm->tm_mon+1;
  //String currentMonthName = months[currentMonth-1];
  int year = ptm->tm_year+1900;  
  day = timeClient.getDay();
  hour = timeClient.getHours();
  minute = timeClient.getMinutes();
  
  date = String(monthDay) + "/" + String(month) + "/" + String(year);
  Time = String(hour) + ":" + String(minute);
  
  Serial.println(String(daysOfTheWeek[day]) + ", " + date + ", " + Time);
  //Serial.println(timeClient.getFormattedTime());
}

void sendData()
{
  client.setInsecure();
  Serial.print("Connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }

  String url = "/macros/s/" + GAS_ID + "/exec?Date=" + date + "&Time="+ Time + "&Period=" + String(period) + "&Student1=" + student1 + "&Student2=" + student2 + "&Student3=" + student3 + "&Student4=" + student4 + "&Student5=" + student5 + "&Student6=" + student6 + "&Student7=" + student7 + "&Student8=" + student8 + "&Student9=" + student9 + "&Student10=" + student10;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("Request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      //Serial.println("Headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  /*if (line.startsWith("{\"state\":\"success\""))Serial.println("esp8266/Arduino CI successfull!");
  else Serial.println("esp8266/Arduino CI has failed");
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");*/
}

void readMCUdata()
{
  String att_data;
  if(MCU.available()) while(MCU.available()) att_data = MCU.readStringUntil('\n');
  Serial.println(att_data);
  int i1 = att_data.indexOf(",");
  int i2 = att_data.indexOf(",", i1+1);
  int i3 = att_data.indexOf(",", i2+1);
  int i4 = att_data.indexOf(",", i3+1);
  
  student1 = att_data.substring(0, i1);
  student2 = att_data.substring(i1+1, i2);
  student3 = att_data.substring(i2+1, i3);
  student4 = att_data.substring(i3+1, i4);
  student5 = att_data.substring(i4+1);
  int len = student5.length();
  student5.remove(len-1);
    
  if((student1 == "0") || (student1 == "1")) sendData();
}
