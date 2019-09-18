#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
#include <NewPing.h>

#define TRIGGER_PIN_0 A0
#define ECHO_PIN_0 A1
#define TRIGGER_PIN_1 A2
#define ECHO_PIN_1 A3
#define TRIGGER_PIN_2 A4
#define ECHO_PIN_2 A5

#define MAX_DISTANCE 200

NewPing sonar_0(TRIGGER_PIN_0, ECHO_PIN_0, MAX_DISTANCE); 
NewPing sonar_1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE); 
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE); 

unsigned int distance_0;
unsigned int distance_1;
unsigned int distance_2;

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

Servo servo_1;
Servo servo_2;

int pos = 0;
int openFlag = 0;

int trigPin_0 = A0;    // Trigger
int echoPin_0 = A1;    // Echo

int trigPin_1 = A2;    // Trigger
int echoPin_1 = A3;    // Echo

int trigPin_2 = A4;    // Trigger
int echoPin_2 = A5;    // Echo

long duration, cm, inches;

int RFID_status;
int RFID_read = 0;
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  servo_1.attach(3);
  servo_2.attach(5);
  servo_1.write(0);
  servo_2.write(0);
  
  pinMode(trigPin_0, OUTPUT);
  pinMode(echoPin_0, INPUT);
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);  
  
  Serial.println("Approximate your card to the reader...");
  Serial.println();

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  
  
  if (content.substring(1) == "E9 9F A2 63" && openFlag ==0) //change here the UID of the card/cards that you want to give access
  // codition= UID matched; openFlag = 0; 
  {
    Serial.println("Authorized access");
    Serial.println();
    unlock_open();
    openFlag = 1;
    //delay(900);
    //check_Ping();
    //delay(500);

            while (openFlag ==1){
                    check_Ping();
            }

    }
  }
    
   else  
   {
     Serial.println(" Access denied");
     delay(3000);
   }
} 

void ultrasonic_trigger() {
     servo_1.write(0);              // tell servo to go to position in variable 'pos' 
     delay(15);                       // waits 15ms for the servo to reach the position                                
     servo_2.write(0);              // tell servo to go to position in variable 'pos' 
     delay(15);                       // waits 15ms for the servo to reach the position 
}


void check_distance() {
  digitalWrite(trigPin_0, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin_0, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_0, LOW);
  
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  //pinMode(echoPin_0, INPUT);
  duration = pulseIn(echoPin_0, HIGH);

  //pinMode(echoPin_1, INPUT);
  duration = pulseIn(echoPin_1, HIGH);
 
   //pinMode(echoPin_2, INPUT);
  duration = pulseIn(echoPin_2, HIGH); 
  
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  //delay(250);
}

void unlock_open() {
    servo_1.write(0);                
    delay(3600);                          
    servo_2.write(0);  
    delay(15);
}

void lock_close() {                               
    servo_2.write(0);                
    delay(3600);                          
    servo_1.write(0);  
    delay(15);
}

/*void check_pulse() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin_0, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin_0, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_0, LOW);
  
  digitalWrite(trigPin_1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_1, LOW);

  digitalWrite(trigPin_2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW); 
}*/ 
//boleh remove sdh since ping library sdh ada included


void check_Ping() {
   distance_0 = sonar_0.ping_cm();
   Serial.print(distance_0);
   Serial.println("cm");
   delay(50);
   
   distance_1 = sonar_1.ping_cm();
   Serial.print(distance_1);
   Serial.println("cm");
   delay(50);
   
   distance_2 = sonar_2.ping_cm();
   Serial.print(distance_2);
   Serial.println("cm");
   delay(50);
   
    if (distance_0<= 5 || distance_1<= 5 || distance_2<= 5) {
      lock_close();
      int openFlag = 0;
    } 
}

/*void card_status() {
    RFID_status = MFRC522_Request(PICC_REQIDL, str);
    if (RFID_status != 1)
    {
         RFID_read 0;
    } 
}*/
