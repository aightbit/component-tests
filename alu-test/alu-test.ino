#include <ArduinoUnit.h>

#define CARRY_IN 0
#define A_BYTE 1
#define B_BYTE 5

#define SUM_BYTE 9
#define CARRY_OUT 13

void writeByte(int startPin, int value) {
  for(int offset=0; offset < 4; offset++) {
    int bitIsSet = (value >> offset) & 0x1;
    digitalWrite(startPin+offset, bitIsSet ? HIGH : LOW);
  }
}

int readByte(int startPin) {
  int value = 0;
  for(int offset=0; offset < 4; offset++) {
    value |= (digitalRead(startPin+offset) == HIGH ? 1 : 0) << offset;
  }
  return value;
}

test(add_one_two) 
{
  digitalWrite(CARRY_IN, 0);
  writeByte(A_BYTE, 1);
  writeByte(B_BYTE, 2);
  
  int result = readByte(SUM_BYTE);
  assertEqual(3, result);

  int carry = digitalRead(CARRY_OUT);
  assertEqual(LOW, carry);
}

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only

  for(int pin=0; pin <= 8; pin++) {
      pinMode(pin, OUTPUT);
  }

  for(int pin=9; pin <= 13; pin++) {
      pinMode(pin, INPUT);
  }

}

void loop()
{
  Test::run();
}
