#include <Arduino.h>
#include <avr/sleep.h>
#include <TimerOne.h>

#define LEDG_PIN1 10 //button 2
#define LEDG_PIN2 11 //button 3
#define LEDG_PIN3 12 //button 4
#define LEDG_PIN4 13 //button 5

#define LEDR_PIN 9
#define POT_PIN A0

#define BUTTON_PIN1 2
#define BUTTON_PIN2 3
#define BUTTON_PIN3 4
#define BUTTON_PIN4 5

typedef enum {
  BOOT, DEMO, TURN, FAILED
} State;

State current_state = BOOT;

// variabili per i tempi

// millisecondi prima di iniziare il gioco
int t1 = 2000;

// millisecondi prima di spegnere il led dopo
int t2_delays[] = {1500, 1200, 900, 500};
int t2 = 1500; 

// millisecondi per accendere i led
int t3= 10000;
int t3_delays[] = {10000, 6000, 4000, 2000};

int multiplier;
int button_order;

volatile int clac = -1; //bottone premuto
int clic=0;

TimerOne* timer; 
int currIntensity = 0;
int fadeAmount = 5;
bool gameStarted = false; // Flag per indicare se il gioco è iniziato
unsigned long waitTime = 10000;
unsigned long startTime;

// TODO: Use or remove.
void wakeUp(){
  Serial.println("Button interrupt");
  Serial.println(random(20000, 40000));
}

void on_button_1_clicked(){
  if (current_state == TURN) {
    clac = BUTTON_PIN1;
    digitalWrite(LEDG_PIN1, HIGH);
    Serial.println("Button 1 clicked");
    wakeUp();
  }
}

void on_button_2_clicked(){
  if (current_state == TURN) { 
    clac = BUTTON_PIN2;
    digitalWrite(LEDG_PIN2, HIGH);
    Serial.println("Button 2 clicked");
    wakeUp();
  }
}

void on_button_3_clicked(){
  if (current_state == TURN) {
    clac = BUTTON_PIN3;
    digitalWrite(LEDG_PIN3, HIGH);
    Serial.println("Button 3 clicked");
    wakeUp();
  }
}

void on_button_4_clicked(){
  if (current_state == TURN) {
    clac = BUTTON_PIN4;
    digitalWrite(LEDG_PIN4, HIGH);
    Serial.println("Button 4 clicked");
    wakeUp();
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDG_PIN1, OUTPUT);
  pinMode(LEDG_PIN2, OUTPUT);
  pinMode(LEDG_PIN3, OUTPUT);
  pinMode(LEDG_PIN4, OUTPUT);
  pinMode(LEDR_PIN, OUTPUT);

  pinMode(BUTTON_PIN1, INPUT);
  pinMode(BUTTON_PIN2, INPUT);
  pinMode(BUTTON_PIN3, INPUT);
  pinMode(BUTTON_PIN4, INPUT);

  Serial.begin(9600);

  // Inizializza la generazione dei numeri casuali
  randomSeed(analogRead(0));

  //timer per lo sleep
  timer = new TimerOne();
  timer->setPeriod(10000); // 10 sec
  //timer.attachInterrupt();

  //interrupt per accendere luci
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN1), on_button_1_clicked, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN2), on_button_2_clicked, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN3), on_button_3_clicked, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN4), on_button_4_clicked, RISING);

}

void read_difficulty() {
  int potentiometer_value = analogRead(POT_PIN); 
  Serial.print("Potentiometer value = ");
  Serial.println(potentiometer_value);
  int difficulty = map(potentiometer_value, 0, 1023, 0, 3);
  Serial.print("Difficulty = ");
  Serial.println(difficulty);
  t2 = t2_delays[difficulty];
  t3 = t3_delays[difficulty];
}

void update_red_led_intensity() {
    // luce rossa inizia a pulsare
    analogWrite(LEDR_PIN, currIntensity);
    currIntensity = currIntensity + fadeAmount;
          Serial.println("val intesità \n");
          Serial.println(currIntensity);

    if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount;
    }
}

void boot() {
  Serial.println("\n------------------------");
  Serial.println("BOOT");

  
  //se gioco è iniziato
  if (!gameStarted) {
    // Primo messaggio sulla seriale
    Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start \n");

    // inizia il timer
    startTime = millis();

    //attendi pulsante b1 o che i 10 sec passino
    while ((!gameStarted)&&(millis() - startTime <= waitTime)) { 
      update_red_led_intensity();
      delay(100);
      if (digitalRead(BUTTON_PIN1) == HIGH) {
        gameStarted = true; // Il gioco è iniziato
        break;
      }
    }
  }

  // se gioco non è iniziato in deep sleep
  if (!gameStarted) {
    Serial.println("sleep \n");
    digitalWrite(LEDR_PIN, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
  }

  //stato iniziale di gioco dove tutti i led verdi sono accesi
  digitalWrite(LEDG_PIN4, HIGH);
  digitalWrite(LEDG_PIN3, HIGH);
  digitalWrite(LEDG_PIN2, HIGH);
  digitalWrite(LEDG_PIN1, HIGH);
  digitalWrite(LEDR_PIN, HIGH);

  delay(t1); //tempo attesa inizio gioco

  noInterrupts();
  current_state = DEMO;
  Serial.println("DEMO");
  interrupts();
}

void demo() {
  read_difficulty();

  //iniziano a spegnersi i led
  pinMode(LEDR_PIN, LOW);

  button_order = 0; //variabile usata per non usare un vettore
  int led_count = 0; //led accesi
  multiplier = 1;

  //qui spengo i led, devo spegnerli in un tempo T3
  while (led_count < 4) {
    int n = random(0, 4);

    switch (n) {
      case 3: 
        if (digitalRead(LEDG_PIN4) == LOW) {
          continue;
        } else {
          digitalWrite(LEDG_PIN4, LOW);
          break;
        }
      case 2:
        if (digitalRead(LEDG_PIN3) == LOW){
          continue;
        } else {
          digitalWrite(LEDG_PIN3, LOW);
          break;
        }
      case 1:
        if (digitalRead(LEDG_PIN2) == LOW){
          continue;
        } else {
          digitalWrite(LEDG_PIN2, LOW);
          break;
        }
      case 0:
        if (digitalRead(LEDG_PIN1) == LOW){
          continue;
        } else {
          digitalWrite(LEDG_PIN1, LOW);
          break;
        }
    }

    Serial.print("Turned off light #");
    Serial.print(led_count);
    Serial.print(" = ");
    Serial.println(n);

    button_order = button_order + n * multiplier;
    multiplier = multiplier * 10;
    led_count++; 
    delay(t2);
  }

  multiplier = multiplier / 10;

  Serial.print("Button order = ");
  Serial.println(button_order);

  noInterrupts();
  current_state = TURN;
  Serial.println("TURN");
  interrupts();
}

void turn() {
  noInterrupts();
  if (clac != -1) {
    Serial.print("Registered press of button ");
    Serial.println(clac);

    if (clic == clac) {
      clic = button_order / multiplier;
      button_order = button_order % multiplier;
      Serial.println("Correct button pressed!");
    } else {
      Serial.println("Wrong button. Game over!");
      interrupts();
      delay(10000000000000);
    }

    clac = -1;
  }
  interrupts();
}

void loop() {
  switch (current_state) {
    case BOOT:
      boot();
      break;
    case DEMO:
      demo();
      break;
    case TURN: 
      turn();
      break;
    default:
      break;
  }
}
