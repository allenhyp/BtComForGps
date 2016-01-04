

String message;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available())
  {
    message+=char(Serial.read());
  }
  if(!Serial.available())
  {
    if(message!="")
    Serial.println(message);
    message = "";
  }
  delay(1000);
}
