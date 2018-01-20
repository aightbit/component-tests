#include <ArduinoUnit.h>

#define BUS 22

#define CLK 40

#define ADDR 42

#define RAM_CS 50
#define RAM_WE 52

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
  delay(1);
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

void resetControlLinesAndBus() {
  low(CLK);
  high(ADDR);
  high(RAM_CS);
  high(RAM_WE);
  
  writeBus(0);
}


void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only

  pinMode(CLK, OUTPUT);

  pinMode(ADDR, OUTPUT);

  pinMode(RAM_CS, OUTPUT);
  pinMode(RAM_WE, OUTPUT);

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
    
    low(ADDR);
    writeBus(i);
    tick();
    high(ADDR);

    int value = i;
    low(RAM_WE);
    low(RAM_CS);
    writeBus(value);
    Serial.print("STR["); Serial.print(i); Serial.print("] = ");
    Serial.println(value);
    high(RAM_CS);
    high(RAM_WE);
  }

  for(int i = 0; i < 0xFF; i++) {
    resetControlLinesAndBus();

    low(ADDR);
    writeBus(i);
    tick();
    high(ADDR);

    // just to make sure that we're not re-reading our written data
    resetControlLinesAndBus();

    low(RAM_CS);
    int storedValue = readBus();
    Serial.print("LDR["); Serial.print(i,BIN); Serial.print("] = ");
    Serial.println(storedValue, BIN);
    if(storedValue != i) {
      Serial.println("wrong");
    }
    high(RAM_CS);
  }
}
