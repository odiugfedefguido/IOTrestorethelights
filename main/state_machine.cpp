#include "state_machine.h"
#include "config.h" // Includi il file di configurazione
#include "utilities.h"

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

    //Serial.print("Turned off light #");
    //Serial.print(led_count);
    //Serial.print(" = ");
    //Serial.println(n);

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
  Serial.println("Go!");
  points=0;
  Serial.println("points:");
  Serial.println(points);
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
      //Serial.println("Correct button pressed!");
      
    } else {
      //Serial.println("Wrong button. Game over!");
      //interrupts();
      attemps=4;
      points=0;
      current_state = BOOT;
    }

    if(attemps==0){
      // TODO: Move to correct location.
      time_data.t1 = (int) time_data.t1 * 0.95;
      time_data.t2 = (int) time_data.t2 * 0.95;
      time_data.t3 = (int) time_data.t3 * 0.95;
      points++;
      Serial.println("New Point! Total Points:");
      Serial.println(points);

      attemps=4;
      current_state=BOOT;
    }
    clac = -1;
  }
  interrupts();
}
