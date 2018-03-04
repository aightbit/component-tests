#define CLK 50

#define BUS 22

#define RST 34
#define CE 42
#define CO 43
#define J 44

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only

  pinMode(CLK, OUTPUT);

  for(int pin=BUS; pin < BUS+8; pin++) {
      pinMode(pin, OUTPUT);
  }
  
  pinMode(RST, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(CO, OUTPUT);
  pinMode(J, OUTPUT);
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
  // disable all
  low(CLK);
  low(RST);
  low(CE);
  low(CO);
  low(J);
  writeBus(0);

  // examine RST & CE & CO
  high(RST);
  tick();
  low(RST);
  
  for(int i=0;i<=0xFF;i++) {
    high(CO);
    int storedInPc = readBus();
    if(storedInPc != i) {
      Serial.print("CE: ");
      Serial.println(i, BIN);
      Serial.print("PC: ");
      Serial.println(storedInPc, BIN);
      BREAKPOINT
    }
    low(CO);

    
    high(CE);
    tick();
    low(CE);
  }

  // examine J & CO
  for(int i=0;i<=0xFF;i++) {
    high(J);
    writeBus(i);
    tick();
    low(J);
    
    high(CO);
    int storedInPc = readBus();
    
    if(storedInPc != i) {
      Serial.print("J: ");
      Serial.println(i, BIN);
      Serial.print("PC: ");
      Serial.println(storedInPc, BIN);
      BREAKPOINT
    }
    low(CO);
    
  }

  Serial.println("COMPLETED ROUND :)");
}

