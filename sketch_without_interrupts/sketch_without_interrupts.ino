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
  BOOT, FADING, DEMO, TURN, FAILED
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
int divider;
int button_order;
int correct_presses = 0;
int score;

int last_button_press = 0;
int button_threshold = 500;

TimerOne* fading_timer; 
TimerOne* timer; 
int currIntensity = 0;
int fadeAmount = 5;
bool gameStarted = false; // Flag per indicare se il gioco è iniziato
unsigned long waitTime = 10000;
unsigned long startTime;

// To wake up from sleep mode
void wakeUp(){}

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

  fading_timer = new TimerOne();
  fading_timer->initialize();

  //interrupt che si attiva se schiacci uno dei bottoni
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN1), wakeUp, RISING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN2), wakeUp, RISING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN3), wakeUp, RISING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN4), wakeUp, RISING); 
}

void read_difficulty() {
  int potentiometer_value = analogRead(POT_PIN); 
  Serial.print("Potentiometer value = ");
  Serial.println(potentiometer_value);
  int difficulty = map(potentiometer_value, 0, 1023, 0, 3);
  Serial.print("Difficulty = ");
  Serial.println(difficulty);
  time_data.t2 = time_data.t2_delays[difficulty];
  time_data.t3 = time_data.t3_delays[difficulty];
}

void update_red_led_intensity() {
  // luce rossa inizia a pulsare
  analogWrite(LEDR_PIN, currIntensity);
  currIntensity = currIntensity + fadeAmount;

  if (currIntensity == 0 || currIntensity == 255) {
    fadeAmount = -fadeAmount;
  }
}

void boot() {
  Serial.println("\n------------------------");
  Serial.println("BOOT");

  // Primo messaggio sulla seriale
  Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start \n");

  // inizia il timer
  startTime = millis();
  gameStarted = false;

  fading_timer->attachInterrupt(update_red_led_intensity, 20000);

  noInterrupts();
  current_state = FADING;
  interrupts();
}

void light_green_leds() {
  //stato iniziale di gioco dove tutti i led verdi sono accesi
  digitalWrite(LEDG_PIN4, HIGH);
  digitalWrite(LEDG_PIN3, HIGH);
  digitalWrite(LEDG_PIN2, HIGH);
  digitalWrite(LEDG_PIN1, HIGH);
}

void fading() {
  //se gioco è iniziato
  if (!gameStarted) {
    if (millis() - startTime <= waitTime) {
      if (digitalRead(BUTTON_PIN1) == HIGH) {
        gameStarted = true; // Il gioco è iniziato
      } else {
        return;
      }
    } else {
      // se gioco non è iniziato in deep sleep
      Serial.println("sleep \n");
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_mode();
    }
  }

  fading_timer->detachInterrupt();

  light_green_leds();
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
  divider = 1000;
  correct_presses = 0;

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

  Serial.print("Button order = ");
  Serial.println(button_order);

  startTime = millis();

  noInterrupts();
  current_state = TURN;
  Serial.println("TURN");
  interrupts();
}

void finish_turn() {
  noInterrupts();
  if (correct_presses == 4) {
    score += 500;
    time_data.t1 = (int) time_data.t1 * 0.95;
    time_data.t2 = (int) time_data.t2 * 0.95;
    time_data.t3 = (int) time_data.t3 * 0.95;
    Serial.println("You won this round!");
  }

  Serial.print("Your current score is: ");
  Serial.println(score);

  int correct_presses = 0;
  interrupts();

  light_green_leds();
  delay(time_data.t1); //tempo attesa inizio gioco

  noInterrupts();
  current_state = DEMO;
  interrupts();
}

void turn() {
  int clac = -1; //bottone premuto
  int current_time = millis();

  noInterrupts();

  if (current_time - startTime > time_data.t3) {
    Serial.println("Timed out.");
    finish_turn();
    interrupts();
    return;
  }

  if (digitalRead(BUTTON_PIN1) == HIGH) {
    digitalWrite(LEDG_PIN1, HIGH);
    clac = 0;
  } else if (digitalRead(BUTTON_PIN2) == HIGH) {
    digitalWrite(LEDG_PIN2, HIGH);
    clac = 1;
  } else if (digitalRead(BUTTON_PIN3) == HIGH) {
    digitalWrite(LEDG_PIN3, HIGH);
    clac = 2;
  } else if (digitalRead(BUTTON_PIN4) == HIGH) {
    digitalWrite(LEDG_PIN4, HIGH);
    clac = 3;
  }

  if (clac != -1 && millis() - last_button_press > button_threshold) {
    last_button_press = millis();
    int clic = button_order / divider;

    Serial.print("Registered press of button ");
    Serial.println(clac);
    Serial.print("Expected press of button ");
    Serial.println(clic);

    if (clic == clac) {
      button_order = button_order % divider;
      divider = divider / 10;
      correct_presses++;
      Serial.println("Correct button pressed!");

      if (correct_presses == 4) {
        finish_turn();
        interrupts();
        return;
      }
    } else {
      Serial.println("Wrong button. Game over!");
      finish_turn();
    }
  }

  interrupts();
}

void loop() {
  switch (current_state) {
    case BOOT:
      boot();
      break;
    case FADING:
      fading();
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
