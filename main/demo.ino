void demo()
{
    // turn off red LED
    pinMode(LEDR_PIN, LOW);
    Serial.println("Go!");

    if (t3 == -1) {
      read_difficulty();
    }

    // create button order at random
    button_order = 0;  // variable used to not use an array
    int led_count = 0; // LEDs that have been turned off
    multiplier = 1;

    // turn off LEDs with delay t3
    while (led_count < 4)
    {
        int n = random(0, 4);

        switch (n)
        {
        case 3:
            if (digitalRead(LEDG_PIN4) == LOW)
            {
                continue;
            }
            else
            {
                digitalWrite(LEDG_PIN4, LOW);
                break;
            }
        case 2:
            if (digitalRead(LEDG_PIN3) == LOW)
            {
                continue;
            }
            else
            {
                digitalWrite(LEDG_PIN3, LOW);
                break;
            }
        case 1:
            if (digitalRead(LEDG_PIN2) == LOW)
            {
                continue;
            }
            else
            {
                digitalWrite(LEDG_PIN2, LOW);
                break;
            }
        case 0:
            if (digitalRead(LEDG_PIN1) == LOW)
            {
                continue;
            }
            else
            {
                digitalWrite(LEDG_PIN1, LOW);
                break;
            }
        }

        debug("Turned off light #" + String(led_count) + " = " + String(n));

        button_order = button_order + n * multiplier;
        multiplier = multiplier * 10;
        led_count++;

        if (led_count < 4)
        {
            delay(t2);
        }
    }

    multiplier = multiplier / 10;

    noInterrupts();

    // prepare turn
    start_time = millis();
    current_state = TURN;
    points = 0;

    debug("Button order = " + String(button_order));
    debug("TURN");

    interrupts();
}