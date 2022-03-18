void setup() {
  pinMode(5, INPUT_PULLUP); 

}

void loop() {
 int x = digitalRead(5);
  if (x == HIGH) { 
    Serial.print("SWITCH HIGH");
  } else { 
    Serial.print("SWITCH LOW");
  }
}
