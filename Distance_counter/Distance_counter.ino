const int inPing = 8;
const int inEcho = 9;
const int outPing = 6;
const int outEcho = 7;
const int redPin = 10;
const int greenPin = 11;

//7 seg pins
const int a = 2;
const int b = 3;
const int c = 4;
const int d = 5;
const int e = 10;
const int f = 11;
const int g = 12;

const int D1 = A0;
const int D2 = A1;
const int D3 = A2;
const int D4 = A3;

const int maxInactiveDist = 20;

bool inTriggered = false;
bool outTriggered = false;

long outDist = 0;
long inDist = 0;

unsigned long inTriggeredAt = 99999999999;
unsigned long outTriggeredAt = 99999999999;

int people = 0;
int maxPeople = 11;

bool atCapacity = false;

void setup()
{
  Serial.begin(9600);
  pinMode(inPing, OUTPUT);
  pinMode(inEcho, INPUT);
  pinMode(outPing, OUTPUT);
  pinMode(outEcho, INPUT);
  pinMode(redPin, OUTPUT);

  pinMode(a, OUTPUT);     
  pinMode(b, OUTPUT);     
  pinMode(c, OUTPUT);     
  pinMode(d, OUTPUT);     
  pinMode(e, OUTPUT);     
  pinMode(f, OUTPUT);     
  pinMode(g, OUTPUT); 
    
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT); 
}

void loop()
{ 
  checkPeople();
  
//  updateLights();
  getPeopleThatCanEnter();

//  Serial.print("people: ");
//  Serial.print(people);
//  Serial.println();
  
//  delay(100);
}

void getPeopleThatCanEnter() {
  int peopleThatCanEnter = maxPeople - people;\
  if(peopleThatCanEnter <= 0) {
    peopleThatCanEnter = 0;
  }
  int ones = peopleThatCanEnter % 10;
  int tens = (peopleThatCanEnter / 10) % 10;
  set7Seg(ones, 1);
  delay(10);
  set7Seg(tens, 2);
}


/*
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
 */
int numbers[][7] {
//{ a, b, c, d, e, f, g }
  { LOW, LOW, LOW, LOW, LOW, LOW, HIGH }, //0
  { HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH }, // 1
  { LOW, LOW, HIGH, LOW, LOW, HIGH, LOW },// 2
  { LOW, LOW, LOW, LOW, HIGH, HIGH, LOW },//3
  { HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW },//4
  { LOW, HIGH, LOW, LOW, HIGH, LOW, LOW },//5
  { LOW, HIGH, LOW, LOW, LOW, LOW, LOW },//6
  { LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH },//7
  { LOW, LOW, LOW, LOW, LOW, LOW, LOW },//8
  { LOW, LOW, LOW, LOW, HIGH, LOW, LOW },//9
};

void set7Seg(int num, int digit) {
  if(digit == 2) {
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
  } else {
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
  }

  digitalWrite(a, numbers[num][0]);
  digitalWrite(b, numbers[num][1]);
  digitalWrite(c, numbers[num][2]);
  digitalWrite(d, numbers[num][3]);
  digitalWrite(e, numbers[num][4]);
  digitalWrite(f, numbers[num][5]);
  digitalWrite(g, numbers[num][6]);
}


void updatePeople(int incrament) {
  people += incrament;
  if(people <= 0) {
    people = 0;
  }
  if(people >= maxPeople) {
    atCapacity = true;
    Serial.println();
    Serial.print("at capacity");
  } else {
    atCapacity = false;
  }
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void updateLights() {
  if(atCapacity == false) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
  } else if(atCapacity == true) {
     digitalWrite(greenPin, LOW); 
     digitalWrite(redPin, HIGH);
  }
}

void checkPeople() {
  getDist();
  if(inDist < maxInactiveDist) {
    if(inTriggered == false) {
      inTriggered = true;
      inTriggeredAt = millis();
    }
  }
  
  if(outDist < maxInactiveDist) {
    if(outTriggered == false) {
     outTriggered = true;
     outTriggeredAt = millis(); 
    }
  }
  
  if(inTriggered && outTriggered) {
    if(inTriggeredAt < outTriggeredAt) {
      Serial.print("going in");
      updatePeople(1);
      Serial.println();
    }

    if(outTriggeredAt < inTriggeredAt) {
      Serial.print("going out");
      updatePeople(-1);
      Serial.println();
    } 
    delay(1000);
    inTriggered = false;
    outTriggered = false;
    inTriggeredAt = 99999999999;
    outTriggeredAt = 99999999999;
  }
}

void getDist() {
  long inPingTime;
  digitalWrite(inPing, LOW);
  delayMicroseconds(2);
  digitalWrite(inPing, HIGH);
  delayMicroseconds(10);
  digitalWrite(inPing, LOW);
  inPingTime = pulseIn(inEcho, HIGH);

  inDist = microsecondsToCentimeters(inPingTime);

  long outPingTime;
  digitalWrite(outPing, LOW);
  delayMicroseconds(2);
  digitalWrite(outPing, HIGH);
  delayMicroseconds(10);
  digitalWrite(outPing, LOW);
  outPingTime = pulseIn(outEcho, HIGH);

  outDist = microsecondsToCentimeters(outPingTime);
}
