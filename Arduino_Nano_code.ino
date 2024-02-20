/*
 * Vighnesh - ID 1
 * Prithvi - ID 2
 * Shanmukha - ID 3
 * Prajwal - ID 4
 * Tejas - ID 5
 */

#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>

const int blue_led = A4;
const int green_led = A0;
const int red_led = 13;
const int buzz = 2;
const int rs = 5, en = 6, d4 = 7, d5 = 8, d6 = 9, d7 = 10;

const int total_students = 5;
const int id_confidence = 80;
bool punchin_flag = false;
int attendance[10] = {0,0,0,0,0,0,0,0,0,0};
String names[total_students] = {"Vighnesh", "Prithvi", "Shanmukha", "Prajwal", "Tejas"};
String numbers[total_students] = {"+917899381170", "+919886540408", "+918904859446", "+919632549633", "+917204908049"};

int period;

SoftwareSerial softserial1(11, 12); // RX, TX for fingerprint sensor
SoftwareSerial gsm(3, 4); // RX, TX for GSM MODEM
SoftwareSerial nodemcu(A2, A3); // RX, TX for NodeMCU
Adafruit_Fingerprint scanner = Adafruit_Fingerprint(&softserial1);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  io_init();
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Attendance   ");
  lcd.setCursor(0,1);
  lcd.print("   Monitoring   ");
  led(1,0,0);
  delay(1000);
  led(0,1,0);
  delay(1000);
  led(0,0,1);
  delay(1000);
  led(0,0,0);
  scanner.begin(57600);
  if (scanner.verifyPassword()) Serial.println("Found fingerprint sensor!");
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.print("Sensor not found");
    while (1) { delay(1); }
  }

  scanner.getTemplateCount();

  if (scanner.templateCount == 0) Serial.print("Sensor doesn't contain any fingerprint data. Please enroll first.");
  else
  {
    Serial.print("Sensor contains "); Serial.print(scanner.templateCount); Serial.println(" templates");
  }
  nodemcu.begin(9600);
}

void loop()
{
  for(int i=0; i<250; i++)
  {
    if(nodemcu.available())
    {
      int p = nodemcu.parseInt();
      if(p > 0)
      {
        if(punchin_flag == false)
        {
          Serial.println(p);
          tone(buzz, 1200,500);
          punchin_flag = true;
          period = p;
        }
      }
      else
      {
        if(punchin_flag == true)
        {
          Serial.println(p);
          submit_attendance();
          clearData();
          punchin_flag = false;
        }
      }
      break;
    }
    delay(1);
  }
  if(punchin_flag) take_attendance();
  delay(1);
}

void take_attendance()
{
  led(0,0,1);
  int id = getFingerprintIDez();
  if(id >= 0) storeData(id);
  nodemcu.begin(9600);
  led(0,0,0);
}

int getFingerprintIDez()
{
  scanner.begin(57600);
  uint8_t p = scanner.getImage();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.print("   Period - " + String(period));
    lcd.setCursor(0,1);
    lcd.print("    Punch in    ");
    return -1;
  }

  p = scanner.image2Tz();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.print("Please try again");
    tone(buzz, 1400,150);
    led(1,0,0);
    delay(500);
    return -1;
  }

  p = scanner.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.print("No match found");
    tone(buzz, 1400,150);
    led(1,0,0);
    delay(500);
    return -1;
  }

  Serial.print("Found ID #"); Serial.print(scanner.fingerID);
  Serial.print(" with confidence of "); Serial.println(scanner.confidence);
  if(scanner.confidence > id_confidence)
  {
    lcd.clear();
    lcd.print("  Successfull!  ");
    tone(buzz, 1200,200);
    led(0,1,0);
    delay(1000);
    return scanner.fingerID;
  }
  else
  {
    lcd.clear();
    lcd.print("Please try again");
    tone(buzz, 1400,150);
    led(1,0,0);
    delay(500);
    return -1;
  }
}

void storeData(int Id)
{
  attendance[Id-1] = 1;
}

void submit_attendance()
{
  led(0,1,0);
  String att;
  for(int i=0; i<total_students; i++)
  {
    att += attendance[i];
    if(i != (total_students-1)) att += ",";
  }
  nodemcu.println(att);
  Serial.println(att);
  Serial.println("");
  lcd.clear();
  lcd.print("P-" + String(period) + "  Attendance");
  lcd.setCursor(0,1);
  lcd.print("     submitted");
  tone(buzz, 1200,100);
  absentees_check();
  delay(4000);
  led(0,0,0);
}

void absentees_check()
{
  gsm.begin(9600);
  for(int k=0; k<total_students; k++)
  {
    if(attendance[k] != 1) sendSMS(k);
    delay(250);
  }
  nodemcu.begin(9600);
}

void sendSMS(int n)
{
  String msg = names[n] + " is absent for the period: " + String(period) + ".\n\nSent from JSS Polytechnic Mysuru.";
  gsm.println("AT+CMGF=1"); delay(500); //Sets the GSM Module in Text Mode
  Serial.println("AT+CMGF=1");  
  String phnumber = "AT+CMGS=\"" + numbers[n] + "\"\r";
  gsm.println(phnumber); delay(500);
  Serial.println(phnumber);
  gsm.println(msg); delay(500);
  Serial.println(msg);
  gsm.println((char)26); delay(500); // ASCII code of CTRL+Z
  Serial.println((char)26);
  Serial.println("SMS sent");
}

void clearData()
{
  for(int i=0; i<total_students; i++)
  {
    attendance[i] = 0;
    //Serial.print(attendance[i]);
  }
  Serial.println("");
  lcd.clear();
  lcd.print("   Attendance   ");
  lcd.setCursor(0,1);
  lcd.print("   Monitoring   ");
}

void io_init()
{
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  pinMode(buzz, OUTPUT);
  digitalWrite(buzz, LOW);
  tone(buzz, 1200,500);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void led(int r, int g, int b)
{
  if(r) digitalWrite(red_led, HIGH);
  else digitalWrite(red_led, LOW);

  if(g) digitalWrite(green_led, HIGH);
  else digitalWrite(green_led, LOW);

  if(b) digitalWrite(blue_led, HIGH);
  else digitalWrite(blue_led, LOW);
}
