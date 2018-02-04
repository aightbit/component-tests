#include <ArduinoUnit.h>


#define CLK 50

#define BUS 22

#define AI 42
#define BI 43
#define ADD 46
#define NEG 47
#define AO 36
#define BO 37

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only

  pinMode(CLK, OUTPUT);

  for(int pin=BUS; pin < BUS+8; pin++) {
      pinMode(pin, OUTPUT);
  }
  
  pinMode(AI, OUTPUT);
  pinMode(BI, OUTPUT);
  pinMode(ADD, OUTPUT);
  pinMode(NEG, OUTPUT);
  pinMode(AO, OUTPUT);
  pinMode(BO, OUTPUT);
}

void writeByte(int startPin, int value) {
  for(int offset=0; offset < 8; offset++) {
    int bitIsSet = (value >> offset) & 0x1;
    pinMode(startPin+offset, OUTPUT);
    digitalWrite(startPin+offset, bitIsSet ? HIGH : LOW);
  }
  delay(1);
}

int readByte(int startPin) {
  int value = 0;
  for(int offset=0; offset < 8; offset++) {
    pinMode(startPin+offset, INPUT);
    value |= (digitalRead(startPin+offset) == HIGH ? 1 : 0) << offset;
  }
//  delay(1);
  return value;
}

int readBus() {
  return readByte(BUS) & 0xFF;
}

void writeBus(int value) {
  writeByte(BUS, value & 0xFF);
}

void high(int pin) {
  digitalWrite(pin, HIGH);
  delay(1);
}

void low(int pin) {
  digitalWrite(pin, LOW);
  delay(1);
}

void tick() {
  high(CLK);
  low(CLK);
}

void loop()
{
//  Test::run();

//    int i = 1;
//
//    // disable all
//    high(RAM_CS);
//    high(RAM_WE);
////    writeBus(0);
//
//    writeByte(ADDR_BUS, 0);
//
////    writeBus(2);
//pinMode(22, OUTPUT);
//high(22);
//
//    low(RAM_WE);
//    low(RAM_CS);
//    Serial.print("STR: ");
//    Serial.println(i);
//    delay(1000);
//    high(RAM_CS);
//    high(RAM_WE);
//   
////    writeBus(0);
////
////    high(RAM_WE);
////    low(RAM_CS);
////    delay(1000);
////    Serial.print("LDR: ");
////    Serial.println(readBus());
////    high(RAM_CS);
////    high(RAM_WE);
//}










  for(int i=0;;i = (i+1) & 0xFF) {
    // disable all
    low(CLK);
    high(AO);
    high(BO);
    high(AI);
    high(BI);
    high(ADD);
    low(NEG);
    writeBus(0);

    Serial.print("AI: ");
    Serial.println(i);
    writeBus(i);
    low(AI);
    tick();
    high(AI);

    Serial.print("BI: ");
    Serial.println(2);
    writeBus(2);
    low(BI);
    tick();
    high(BI);

    low(AO);
    Serial.print("AO: ");
    Serial.println(readBus(), BIN);
    high(AO);

    low(BO);
    Serial.print("BO: ");
    Serial.println(readBus(), BIN);
    high(BO);

    writeBus(0);

    low(ADD);
    int addition = readBus();
    Serial.print("ADD: ");
    Serial.println(addition, BIN);
    
    int correctAddition = (i + 2) & 0xFF;
    if(addition != correctAddition) {
      Serial.println("Wrong");
    }
    high(ADD);

    low(ADD);
    high(NEG);
    int subtraction = readBus();
    Serial.print("SUBTRACT: ");
    Serial.println(subtraction, BIN);
    
    int correctSubtraction = (i - 2) & 0xFF;
    if(subtraction != correctSubtraction) {
            Serial.println("Wrong");
    }
    low(NEG);
    high(ADD);

  }
}
