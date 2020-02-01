// Seatbelt Indicator Project 
// UBC Enable 
// Written by: Nima and JFrench 
// Date: June 28, 2018

// need all this shit for the accelerometer
#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 accelgyro;

//logic for reed
 #define logic LOW

// Pin Assignment
  const int led = 10; 
  const int buzzer = 9; 
  int reed = 11;
  int val = 0;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int i = 0;
int big = 0; 
int little = 0; 
int delta = 0;
int data[100];
const int deltaThresh = 6000;

#define OUTPUT_READABLE_ACCELGYRO

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT); 
  pinMode(reed, INPUT); 

  // Not sure what this shit does but just leave it in
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
  #endif

  //Serial.begin(115200);

    // initialize device
//    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
//    Serial.println("Testing device connections...");
//    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

}

void loop() {

while( digitalRead(reed)== logic ) {                               // if the seatbelt gets undone
    digitalWrite(led, HIGH);                                     // turn the LED on
    
    // Filling the x-accel readings into the data array
    //Serial.println("Obtaitning 100 samples");
    for (int i = 0; i < 100; i++) { 
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);          // get the accelerometer readings                                                 
      data[i] = ax;
    //Serial.print(" ");
    //Serial.print(data[i]);
    //Serial.print(" ");
    }
    //Serial.println("100 sample window complete");


    
     // once filled, perform a calculation to find the delta of the data array 
       i = 0;
       big = data[0];
       little = data[0];
      for(int j=0; j<100; j++) { 
        if(data[j] > big){ 
          big = data[j]; 
        }
      }
      for(int j=0; j<100; j++) {
        if(data[j] < little){ 
          little = data[j]; 
        }
      }
      delta = abs(big - little); 
      //Serial.print("Current Delta Value is :");
      //Serial.println(delta); 
      
    // if the delta is large enough (indicating significant motion) turn the buzzer on
    if(delta > deltaThresh) { 
      //Serial.println("Movement Detected with delta value of:");
      //Serial.println(delta);
      digitalWrite(buzzer,LOW);
      delta = 0;
    } 
    
     // delay(5); 

//    #ifdef OUTPUT_READABLE_ACCELGYRO
//        // display tab-separated accel/gyro x/y/z values
//        Serial.print("a/g:\t");
//        Serial.print(ax); Serial.print("\t");
//        Serial.print(ay); Serial.print("\t");
//        Serial.println(az); 
//     #endif 
    
    // not sure if this section is necessary but havent yet tested it without
//    #ifdef OUTPUT_BINARY_ACCELGYRO
//        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
//        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
//        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
//        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
//        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
//        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
//    #endif 
 }

  // if the seatblet is done up, turn the indicators off
  //Serial.println("Seatbelt Closed");
  for(int i = 0; i < 100; i++) {
    data[i] = 0;
  }
  delta  = 0; 
  digitalWrite(led, LOW); 
  digitalWrite(buzzer, HIGH);

} 
