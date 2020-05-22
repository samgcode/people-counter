const int ping = 12;
const int echo = 11;

void setup()
{
  Serial.begin(9600);
  pinMode(ping, OUTPUT);
  pinMode(echo, INPUT);
  
}

void loop()
{
  long pingTime, distance;
  digitalWrite(ping, LOW);
  delayMicroseconds(2);
  digitalWrite(ping, HIGH);
  delayMicroseconds(10);
  digitalWrite(ping, LOW);
  pingTime = pulseIn(echo, HIGH);
  
  distance = microsecondsToCentimeters(pingTime);
  
  Serial.print("cm: ");
  Serial.print(distance);
  Serial.println();
  delay(100);
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
