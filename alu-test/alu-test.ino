#include <ArduinoUnit.h>

//#define CARRY_IN 0
//#define A_BYTE 1
//#define B_BYTE 5
//
//#define SUM_BYTE 9
//#define CARRY_OUT 13

#define CARRY_IN 22
#define A_BYTE 23
#define B_BYTE 27

#define SUM_BYTE 31
#define SUM_CARRY 35

void writeByte(int startPin, int value) {
  for(int offset=0; offset < 4; offset++) {
    int bitIsSet = (value >> offset) & 0x1;
    digitalWrite(startPin+offset, bitIsSet ? HIGH : LOW);
  }
}

int readByte(int startPin) {
  delay(10);
  int value = 0;
  for(int offset=0; offset < 4; offset++) {
    value |= (digitalRead(startPin+offset) == HIGH ? 1 : 0) << offset;
  }
  return value;
}

test(add_zero_zero) 
{
  digitalWrite(CARRY_IN, LOW);
  writeByte(A_BYTE, 0);
  writeByte(B_BYTE, 0);
  
  assertEqual(0, readByte(SUM_BYTE));
  assertEqual(LOW, digitalRead(SUM_CARRY));
}

test(add_one_two) 
{
  digitalWrite(CARRY_IN, LOW);
  writeByte(A_BYTE, 1);
  writeByte(B_BYTE, 2);
  
  assertEqual(3, readByte(SUM_BYTE));
  assertEqual(LOW, digitalRead(SUM_CARRY));
}

test(add_four_four) 
{
  digitalWrite(CARRY_IN, LOW);
  writeByte(A_BYTE, 4);
  writeByte(B_BYTE, 4);
  
  assertEqual(8, readByte(SUM_BYTE));
  assertEqual(LOW, digitalRead(SUM_CARRY));
}

test(add_fifteen_one) 
{
  digitalWrite(CARRY_IN, LOW);
  writeByte(A_BYTE, 15);
  writeByte(B_BYTE, 1);
  
  assertEqual(0, readByte(SUM_BYTE));
  assertEqual(HIGH, digitalRead(SUM_CARRY));
}

test(add_carry_in) 
{
  digitalWrite(CARRY_IN, HIGH);
  writeByte(A_BYTE, 1);
  writeByte(B_BYTE, 2);
  
  assertEqual(4, readByte(SUM_BYTE));
  assertEqual(LOW, digitalRead(SUM_CARRY));
}

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only

  pinMode(CARRY_IN, OUTPUT);

  for(int pin=A_BYTE; pin < B_BYTE; pin++) {
      pinMode(pin, OUTPUT);
  }

  for(int pin=B_BYTE; pin < SUM_BYTE; pin++) {
      pinMode(pin, OUTPUT);
  }

  for(int pin=SUM_BYTE; pin < SUM_CARRY; pin++) {
      pinMode(pin, INPUT);
  }

  pinMode(SUM_CARRY, INPUT);

}

void loop()
{
  Test::run();
  
//writeByte(A_BYTE, 3);
//writeByte(B_BYTE, 1);
//
//Serial.print(readByte(SUM_BYTE), BIN);
//Serial.print(" C:");
//Serial.print(digitalRead(SUM_CARRY));
//Serial.print("\n");
//delay(1000);
}
