#include "interrupts.h"
#include "config.h" // Includi il file di configurazione

void on_button_1_clicked(){
  if (current_state == TURN) {
    clac = 0;
    digitalWrite(LEDG_PIN1, HIGH);
    //Serial.println("Button 1 clicked");
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

