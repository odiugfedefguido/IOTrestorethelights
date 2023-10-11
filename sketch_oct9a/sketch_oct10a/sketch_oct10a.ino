#define LEDG_PIN13 13
#define LEDG_PIN12 12
#define LEDG_PIN11 11
#define LEDG_PIN10 10
#define LEDr_PIN9 9
#define POT_PIN A0
#define BUTTON_PIN2 2
#define BUTTON_PIN4 3
#define BUTTON_PIN5 4
#define BUTTON_PIN6 5

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDG_PIN13, OUTPUT);
  pinMode(LEDG_PIN12, OUTPUT);
  pinMode(LEDG_PIN11, OUTPUT);
  pinMode(LEDG_PIN10, OUTPUT);
  pinMode(LEDr_PIN9, OUTPUT);

  pinMode(BUTTON_PIN2, INPUT);
  pinMode(BUTTON_PIN4, INPUT);
  pinMode(BUTTON_PIN5, INPUT);
  pinMode(BUTTON_PIN6, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = digitalRead(BUTTON_PIN2);
 if (buttonState == HIGH) {
 digitalWrite(LEDG_PIN12, HIGH);
 //Serial.println(“ON”);
 } else {
 digitalWrite(LEDG_PIN12, LOW);
 //Serial.println(“OFF”);
 }
}
