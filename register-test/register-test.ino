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
    pinMode(startPin+offset, OUTPUT);
  }
  
  for(int offset=0; offset < 8; offset++) {
    int bitIsSet = (value >> offset) & 0x1;
    digitalWrite(startPin+offset, bitIsSet ? HIGH : LOW);
  }
  
  delay(1);
}

int readByte(int startPin) {
  delay(1);
  
  for(int offset=0; offset < 8; offset++) {
    pinMode(startPin+offset, INPUT);
  }

  int value = 0;
  for(int offset=0; offset < 8; offset++) {
    // requires its own loop as reading sometimes doesn't reflect
    // the actual value if you just changed the pinMode before
    value |= (digitalRead(startPin+offset) == HIGH ? 1 : 0) << offset;
  }
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

void breakpoint() {
  Serial.println("send character to continue...");
  while(Serial.read() == -1) {
    delay(200);
  }
}

#define BREAKPOINT breakpoint();

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



  for(int i=0;i<=0xFF;i++) {
    int a = i;
    int b = 2;
    
    // disable all
    low(CLK);
    low(AO);
    low(BO);
    low(AI);
    low(BI);
    low(ADD);
    low(NEG);
    writeBus(0);

    high(AI);
    writeBus(a);
    tick();
    low(AI);

    high(BI);
    writeBus(b);
    tick();
    low(BI);

    writeBus(0);

    high(AO);
    int storedInA = readBus();
    if(storedInA != a) {
      Serial.print("AI: ");
      Serial.println(a, BIN);
      Serial.print("AO: ");
      Serial.println(readBus(), BIN);
      BREAKPOINT
    }
    low(AO);

    writeBus(0);

    high(BO);
    int storedInB = readBus();
    if(storedInB != b) {
      Serial.print("BI: ");
      Serial.println(b, BIN);
      Serial.print("BO: ");
      Serial.println(readBus(), BIN);
      BREAKPOINT
    }
    low(BO);

    writeBus(0);

    high(ADD);
    int addition = readBus();
    int correctAddition = (a + b) & 0xFF;
    if(addition != correctAddition) {
      Serial.print("AI: ");
      Serial.println(a, BIN);
      Serial.print("ADD: ");
      Serial.println(addition, BIN);
      Serial.println("Wrong");
      BREAKPOINT
    }
    low(ADD);

    high(ADD);
    high(NEG);
    int subtraction = readBus();
    int correctSubtraction = (a - b) & 0xFF;
    if(subtraction != correctSubtraction) {
      Serial.print("AI: ");
      Serial.println(a, BIN);
      Serial.print("SUBTRACT: ");
      Serial.println(subtraction, BIN);
      Serial.println("Wrong");
      
      BREAKPOINT
    }
    low(NEG);
    low(ADD);
  }

  Serial.println("COMPLETED ROUND :)");
}
