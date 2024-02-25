
int interval = 500;

void setup() {
  // put your setup code here, to run once:

  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    String readString = Serial.readStringUntil('\r');
    readString.trim();
    interval = readString.toInt();
    Serial.printf("interval = %d", interval);
  }

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(interval);              // wait for a second
}
