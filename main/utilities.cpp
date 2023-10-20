#include "utilities.h"
#include "config.h" // Includi il file di configurazione

void wakeUp() {
  // Implementazione della funzione wakeUp()
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
    //Serial.println("val intesità \n");
    //Serial.println(currIntensity);
    //Serial.println(fadeAmount);

    if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount;
    }
    delay(15);
}

