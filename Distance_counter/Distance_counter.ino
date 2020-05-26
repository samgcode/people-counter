const int inPing = 8;
const int inEcho = 9;
const int outPing = 6;
const int outEcho = 7;
const int redPin = 10;
const int greenPin = 11;

const int maxInactiveDist = 20;

bool inTriggered = false;
bool outTriggered = false;

long outDist = 0;
long inDist = 0;

unsigned long inTriggeredAt = 99999999999;
unsigned long outTriggeredAt = 99999999999;

int people = 0;
int maxPeople = 3;

bool atCapacity = false;

void setup()
{
  Serial.begin(9600);
  pinMode(inPing, OUTPUT);
  pinMode(inEcho, INPUT);
  pinMode(outPing, OUTPUT);
  pinMode(outEcho, INPUT);
  pinMode(redPin, OUTPUT);
}

void loop()
{
  checkPeople();
  
  updateLights();
  
  Serial.print("people: ");
  Serial.print(people);
  Serial.println();
  
  delay(100);
}

void updatePeople(int incrament) {
  people += incrament;
  if(people <= 0) {
    people = 0;
  }
  if(people >= maxPeople) {
    atCapacity = true;
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
//  Serial.print("in: ");
//  Serial.print(inDist);
//  Serial.println();
//  Serial.print("out: ");
//  Serial.print(outDist);
//  Serial.println();
//  
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
