#define Button_1 6
#define Button_2 7
#define Button_3 8
#define Button_4 9

void setup() {
  // put your setup code here, to run once:
  pinMode(Button_1,INPUT_PULLUP);
  pinMode(Button_2,INPUT_PULLUP);
  pinMode(Button_3,INPUT_PULLUP);
  pinMode(Button_4,INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(Button_1)){
    delay(30);
    while(!digitalRead(Button_1));
    delay(30);
    Serial.print("1");
  }else if(!digitalRead(Button_2)){
    delay(30);
    while(!digitalRead(Button_2));
    delay(30);
    Serial.print("2");
  }else if(!digitalRead(Button_3)){
    delay(30);
    while(!digitalRead(Button_3));
    delay(30);
    Serial.print("3");
  }else if(!digitalRead(Button_4)){
    delay(30);
    while(!digitalRead(Button_4));
    delay(30);
    Serial.print("4");
  }
}
