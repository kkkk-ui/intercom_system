void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(2, INPUT);
}

void music(){
  tone(13, 493); 
  delay(500);
  tone(13, 392);
  delay(500);
  tone(13, 293);
  delay(500);
  tone(13, 392);
  delay(500); 
  tone(13, 440);
  delay(500); 
  tone(13, 587);
  delay(500);
  noTone(13);
  delay(500);
  tone(13, 293);
  delay(500);
  tone(13, 440);
  delay(500); 
  tone(13, 493);
  delay(500); 
  tone(13, 440);
  delay(500); 
  tone(13, 293);
  delay(500);
  tone(13, 392);
  delay(1000); 
  noTone(13);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = digitalRead(2);
  if(buttonState == HIGH){
    music();
  }
  return 0;
}
