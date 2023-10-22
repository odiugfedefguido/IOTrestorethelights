void update_red_led_intensity();
void read_difficulty();

void boot()
{
    debug("\n------------------------\nBOOT");
    
    // se gioco è iniziato
    if (!game_started)
    {
        // Primo messaggio sulla seriale
        Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start \n");

        // inizia il timer
        start_time = millis();

        // attendi pulsante b1 o che i 10 sec passino
        while ((!game_started) && (millis() - start_time <= wait_time))
        {
            update_red_led_intensity();
            if (digitalRead(BUTTON_PIN1) == HIGH)
            {
                game_started = true; // Il gioco è iniziato
                break;
            }
        }
    }

    // se gioco non è iniziato in deep sleep
    if (!game_started)
    {
        debug("sleep");
        digitalWrite(LEDR_PIN, LOW);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
    }

    // stato iniziale di gioco dove tutti i led verdi sono accesi
    digitalWrite(LEDG_PIN4, HIGH);
    digitalWrite(LEDG_PIN3, HIGH);
    digitalWrite(LEDG_PIN2, HIGH);
    digitalWrite(LEDG_PIN1, HIGH);
    digitalWrite(LEDR_PIN, HIGH);

    delay(time_data.t1); // tempo attesa inizio gioco

    noInterrupts();
    current_state = DEMO;
    debug("DEMO");
    interrupts();
}

void read_difficulty()
{
    int potentiometer_value = analogRead(POT_PIN);
    debug("Potentiometer value = " + String(potentiometer_value));

    int difficulty = map(potentiometer_value, 0, 1023, 0, 3);
    debug("Difficulty = " + String(difficulty));

    time_data.t2 = time_data.t2_delays[difficulty];
    time_data.t3 = time_data.t3_delays[difficulty];
}

void update_red_led_intensity()
{
    analogWrite(LEDR_PIN, curr_intensity);
    curr_intensity = curr_intensity + fade_amount;

    if (curr_intensity == 0 || curr_intensity == 255)
    {
        fade_amount = -fade_amount;
    }
    delay(15);
}