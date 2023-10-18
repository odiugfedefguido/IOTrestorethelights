#include <Arduino.h>
#include <avr/sleep.h>
#include <TimerOne.h>
#include <EnableInterrupt.h>

#define LEDG_PIN1 10 //button 2
#define LEDG_PIN2 11 //button 3
#define LEDG_PIN3 12 //button 4
#define LEDG_PIN4 13 //button 5

#define LEDR_PIN 6
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
typedef struct {
  int t1; // millisecondi prima di iniziare il gioco
  int t2; // millisecondi prima di spegnere il led dopo
  int *t2_delays;
  int t3; // millisecondi per accendere i led
  int *t3_delays;
} TimeData;

TimeData time_data = {
  2000, 15000, (1500, 1200, 900, 500), 1000, (10000, 6000, 4000, 2000)
};

int multiplier;
int button_order;

volatile int clac = -1; //bottone premuto
int clic=0;
volatile int attemps = 4;

TimerOne* timer; 
int currIntensity;
int fadeAmount;
bool gameStarted = false; // Flag per indicare se il gioco è iniziato
unsigned long waitTime = 10000;
unsigned long startTime;

void wakeUp(){}

void on_button_1_clicked(){
  if (current_state == TURN) {
    clac = 0;
    digitalWrite(LEDG_PIN1, HIGH);
    Serial.println("Button 1 clicked");
    wakeUp();
  }
}

void on_button_2_clicked(){
  if (current_state == TURN) { 
    clac = 1;
    digitalWrite(LEDG_PIN2, HIGH);
    Serial.println("Button 2 clicked");
    wakeUp();
  }
}

void on_button_3_clicked(){
  if (current_state == TURN) {
    clac = 2;
    digitalWrite(LEDG_PIN3, HIGH);
    Serial.println("Button 3 clicked");
    wakeUp();
  }
}

void on_button_4_clicked(){
  if (current_state == TURN) {
    clac = 3;
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

  currIntensity = 0;
  fadeAmount =  5;

  //timer per lo sleep
  timer = new TimerOne();
  timer->setPeriod(10000); // 10 sec
  //timer.attachInterrupt();

  //interrupt per accendere luci
  enableInterrupt(BUTTON_PIN1, on_button_1_clicked, RISING);
  enableInterrupt(BUTTON_PIN2, on_button_2_clicked, RISING);
  enableInterrupt(BUTTON_PIN3, on_button_3_clicked, RISING);
  enableInterrupt(BUTTON_PIN4, on_button_4_clicked, RISING);

}

void read_difficulty() {
  int potentiometer_value = analogRead(POT_PIN); 
  Serial.print("Potentiometer value = " + String(potentiometer_value));

  int difficulty = map(potentiometer_value, 0, 1023, 0, 3);
  Serial.print("Difficulty = " + String(difficulty));
  
  time_data.t2 = time_data.t2_delays[difficulty];
  time_data.t3 = time_data.t3_delays[difficulty];
}

void update_red_led_intensity() {
    // luce rossa inizia a pulsare
    analogWrite(LEDR_PIN, currIntensity);
    currIntensity = currIntensity + fadeAmount;
    Serial.println("val intesità \n");
    Serial.println(currIntensity);
    Serial.println(fadeAmount);

    if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount;
    }
    delay(15);
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

  delay(time_data.t1); //tempo attesa inizio gioco

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
    delay(time_data.t2);
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
    clic = button_order / multiplier;
    Serial.println("multipler");
    Serial.println(multiplier);
    Serial.println("clic");
    Serial.println(clic);
    if (clic == clac) {
      clic = button_order / multiplier;
      button_order = button_order % multiplier;
      multiplier= multiplier/10;
      attemps--;
      Serial.println("Correct button pressed!");
      
    } else {
      Serial.println("Wrong button. Game over!");
      //interrupts();
      attemps=4;
      current_state = BOOT;
    }

    if(attemps==0){
      // TODO: Move to correct location.
      time_data.t1 = (int) time_data.t1 * 0.95;
      time_data.t2 = (int) time_data.t2 * 0.95;
      time_data.t3 = (int) time_data.t3 * 0.95;
      Serial.println("You won this round!");

      attemps=4;
      current_state=BOOT;
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
