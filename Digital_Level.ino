include<Wire.h>
#include <math.h>
const int MPU=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double pitch,roll;
int LED1 = 5;
int LED2 = 3;
int LED3 = 4;
const int buzzer = 10;

void setup(){
pinMode(5, LED1);
pinMode(3, LED2);
pinMode(4, LED3);
pinMode(buzzer, OUTPUT);
Wire.begin();
Wire.beginTransmission(MPU);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
}


void loop(){
Wire.beginTransmission(MPU);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU,14,true);

int AcXoff,AcYoff,AcZoff,GyXoff,GyYoff,GyZoff;
int temp,toff;
double t,tx,tf;

//Acceleration data correction
AcXoff = -950;
AcYoff = -300;
AcZoff = 0;

//Temperature correction
toff = -1600;

//Gyro correction
GyXoff = 480;
GyYoff = 170;
GyZoff = 210;

//read accel data
AcX=(Wire.read()<<8|Wire.read()) + AcXoff;
AcY=(Wire.read()<<8|Wire.read()) + AcYoff;
AcZ=(Wire.read()<<8|Wire.read()) + AcYoff;

//read temperature data
temp=(Wire.read()<<8|Wire.read()) + toff;
tx=temp;
t = tx/340 + 36.53;
tf = (t * 9/5) + 32;

//read gyro data
GyX=(Wire.read()<<8|Wire.read()) + GyXoff;
GyY=(Wire.read()<<8|Wire.read()) + GyYoff;
GyZ=(Wire.read()<<8|Wire.read()) + GyZoff;

//get pitch/roll
getAngle(AcX,AcY,AcZ);

//send the data out the serial port
Serial.print("Roll = "); Serial.println(roll);

if (roll > -3 && roll < 3) 
{
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, LOW);
  digitalWrite(LED3, LOW);
  tone(buzzer, 5000); // Send 1KHz sound signal...
}

if (10 > roll && roll > 3) 
{
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED3, LOW);
  noTone(buzzer);
}

if (-3 > roll && roll > -10) 
{
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, LOW);
  digitalWrite(LED3, HIGH);
  noTone(buzzer);
}

if (roll < -10) 
{
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  noTone(buzzer);

}

if (roll > 10) 
{
  digitalWrite(LED3, LOW);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  noTone(buzzer);

}


delay(50);
}





//convert the accel data to pitch/roll
void getAngle(int Vx,int Vy,int Vz) {
double x = Vx;
double y = Vy;
double z = Vz;

pitch = atan(x/sqrt((y*y) + (z*z)));
roll = atan(y/sqrt((x*x) + (z*z)));
//convert radians into degrees
pitch = pitch * (180.0/3.14);
roll = roll * (180.0/3.14) ;
}
