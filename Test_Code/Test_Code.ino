int led = 8;
int Init_PB = 7;

void setup() {

}

void loop() {
  bool flight_init = digitalRead(Init_PB);

  if(flight_init)
    analogWrite(led, 255);
  else
    analogWrite(led, 0);

}
