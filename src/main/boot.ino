void update_red_led_intensity();
void read_difficulty();

int t2_delays[] = {1500, 1200, 900, 500};
int t3_delays[] = {8000, 5400, 2600, 1600};

// the game's starting timestamp 
unsigned long start_time;

// the red LED's intensity
int curr_intensity;

// how much the intensity of the red LED is reduced/increased by on every tick
int fade_amount;

void boot()
{
  debug("\n------------------------\nBOOT");
  
  // while the game has not yet begun …
  while (!game_started)
  {
    // initial message
    Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start \n");

    // set the starting timestamp
    start_time = millis();
    
    // pulsate the red LED for at most 10 seconds
    while ((!game_started) && (millis() - start_time <= t0))
    {
      update_red_led_intensity();
      // if button B1 is pressed, start the game
      if (digitalRead(BUTTON_PIN1) == HIGH)
      {
        game_started = true;
        break;
      }
    }
  
    // after ten seconds without a button press, enter deep sleep mode
    if (!game_started)
    {
      interrupts();
      debug("sleep");
      digitalWrite(LEDR_PIN, LOW);
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_mode();
      delay(500);
    }

  }

  interrupts();

  // turn off all LEDs
  digitalWrite(LEDG_PIN4, LOW);
  digitalWrite(LEDG_PIN3, LOW);
  digitalWrite(LEDG_PIN2, LOW);
  digitalWrite(LEDG_PIN1, LOW);
  digitalWrite(LEDR_PIN, LOW);

  // wait for T1 milliseconds
  delay(t1);

  // turn on green lights 
  digitalWrite(LEDG_PIN4, HIGH);
  digitalWrite(LEDG_PIN3, HIGH);
  digitalWrite(LEDG_PIN2, HIGH);
  digitalWrite(LEDG_PIN1, HIGH);

  noInterrupts();
  // set state to DEMO
  current_state = DEMO;
  debug("DEMO");
  interrupts();
}

/*
 * Read the difficulty based on the potentiometer value.
 * Set t2 and t3 accordingly.
 */
void read_difficulty()
{
  debug("t2: " + String(t2));
  int potentiometer_value = analogRead(POT_PIN);
  debug("Potentiometer value = " + String(potentiometer_value));

  int difficulty = map(potentiometer_value, 0, 1023, 0, 3);
  debug("Difficulty = " + String(difficulty));

  t2 = t2_delays[difficulty];
  t3 = t3_delays[difficulty];
  debug("t2: " + String(t2));
}

/*
 * Set the intensity of the red light.
 * Create a pulsating effect. Based on the lecture slides.
 */
void update_red_led_intensity()
{
  analogWrite(LEDR_PIN, curr_intensity);
  curr_intensity = curr_intensity + fade_amount;

  // invert "direction" when a peak or low end is reached
  if (curr_intensity == 0 || curr_intensity == 255)
  {
    fade_amount = -fade_amount;
  }
  
  interrupts();
  delay(15);
  noInterrupts();
}