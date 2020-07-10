#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define led1 D5
#define buzz D8

TinyGPSPlus  gps;              // The TinyGPS++ object
SoftwareSerial  ss(4, 5) ;   // The serial connection to the GPS device.
WiFiServer server(80);

const char* ssid = "Redmi";
const char* password = "shruti123"; //wifi cponnection

float latitude , longitude;
int year , month , date, hour , minute , second;  //gps variable declaration
String date_str , time_str , lat_str , lng_str;
int pm;

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D6;
const uint8_t sda = D7;

// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

void setup() 
{
  
  Serial.begin(9600);
  //Serial.begin(115200);
  //Accelerometer connection
  Wire.begin(sda, scl);
  MPU6050_Init();
  ss.begin(9600);
  
  //wifi connection print
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

  //BUZZER LED
  pinMode(led1, OUTPUT);
  pinMode( buzz, OUTPUT);
  
}

void loop() {
  HTTPClient http;
  double Ax, Ay, Az, T, Gx, Gy, Gz;
  
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  T = (double)Temperature/340+36.53; //temperature formula
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor;
  Gz = (double)GyroZ/GyroScaleFactor;

  Serial.print("Ax: "); Serial.print(Ax);
  Serial.print(" Ay: "); Serial.print(Ay);
  Serial.print(" Az: "); Serial.print(Az);
  Serial.print(" T: "); Serial.print(T);
  Serial.print(" Gx: "); Serial.print(Gx);
  Serial.print(" Gy: "); Serial.print(Gy);
  Serial.print(" Gz: "); Serial.println(Gz);

  delay(800);

  //GPS
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        Serial.println(lat_str);
        Serial.println(lng_str);
       }
    }
  }

//LED BUZZ
  if(Ax<= -70 || Ax>=100)
  {
    digitalWrite(led1,1);
    digitalWrite(buzz,1);
    String url = "http://iot99.000webhostapp.com/sms.php?long=";
    url = url + String(lng_str)+ "&lat=" + String(lat_str);
    http.begin(url);
    int httpCode = http.GET();
    Serial.println(httpCode);
    if(httpCode > 0){
    // start of fetching get process
      String response = "";
      response = http.getString();
      Serial.println(response);
    }
    http.end();
  }
  else if(Az<=-0.98 || Az>=100)
  {
    digitalWrite(led1,1);
    digitalWrite(buzz,1);
    String url="http://iot99.000webhostapp.com/sms.php?long=";
    url= url+String(lng_str)+"&lat="+String(lat_str);
    http.begin(url);
    int httpCode = http.GET();
    Serial.println(httpCode);
    if(httpCode > 0){
    // start of fetching get process
      String response = "";
      response = http.getString();
      Serial.println(response);
    }
    http.end();
  }
  else if( Gx<=-94 || Gx>=100)
  {
      digitalWrite(led1,1);
      digitalWrite(buzz,1);
      String url="http://iot99.000webhostapp.com/sms.php?long=";
      url= url+String(lng_str)+"&lat="+String(lat_str);
      http.begin(url);
      int httpCode = http.GET();
      Serial.println(httpCode);
      if(httpCode > 0){
      // start of fetching get process
      String response = "";
      response = http.getString();
      Serial.println(response);
    }
    http.end();
  }

  else if( Gy<=-100 || Gy>=100)
  {
      digitalWrite(led1,1);
      digitalWrite(buzz,1);
      String url="http://iot99.000webhostapp.com/sms.php?long=";
      url= url+String(lng_str)+"&lat="+String(lat_str);
      http.begin(url);
      int httpCode = http.GET();
      Serial.println(httpCode);
      if(httpCode > 0){
    // start of fetching get process
      String response = "";
      response = http.getString();
      Serial.println(response);
      }
      http.end();
   }
   else if( Gz<=-60 || Gz>=100)
   {
      digitalWrite(led1,1);
      digitalWrite(buzz,1);
      String url="http://iot99.000webhostapp.com/sms.php?long=";
      url= url+String(lng_str)+"&lat="+String(lat_str);
      http.begin(url);
      int httpCode = http.GET();
      Serial.println(httpCode);
      if(httpCode > 0){
    // start of fetching get process
      String response = "";
      response = http.getString();
      Serial.println(response);
      }
      http.end();
   }
   else
   {
      digitalWrite(led1,0);
      digitalWrite(buzz,0);
  }
}

void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(450);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}
