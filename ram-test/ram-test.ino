#include <ArduinoUnit.h>

#define BUS 22

#define CLK 50

#define RA 36
#define RI 37
#define RO 42

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

void resetControlLinesAndBus() {
  low(CLK);
  low(RA);
  low(RI);
  low(RO);
  
  writeBus(0);
}


void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only

  pinMode(CLK, OUTPUT);

  pinMode(RA, OUTPUT);

  pinMode(RI, OUTPUT);
  pinMode(RO, OUTPUT);

  for(int pin=BUS; pin < BUS+8; pin++) {
      pinMode(pin, OUTPUT);
  }

  resetControlLinesAndBus();
}


void loop()
{
//  Test::run();

  for(int i = 0; i < 0xFF; i++) {
    resetControlLinesAndBus();

    
    high(RA);
    writeBus(i);
    tick();
    low(RA);

    int value = i;
    high(RI);
    
    writeBus(value);
    tick();
    Serial.print("STR["); Serial.print(i); Serial.print("] = ");
    Serial.println(value);
    low(RI);
  }

  for(int i = 0; i < 0xFF; i++) {
    resetControlLinesAndBus();

    high(RA);
    writeBus(i);
    tick();
    low(RA);

    // just to make sure that we're not re-reading our written data
    resetControlLinesAndBus();

    high(RO);
    int storedValue = readBus();
    Serial.print("LDR["); Serial.print(i); Serial.print("] = ");
    Serial.println(storedValue, DEC);
    if(storedValue != i) {
      Serial.println("wrong");
      BREAKPOINT
    }
    low(RO);
  }

  Serial.println("COMPLETED ROUND :)");
}
