void demo()
{
    if (t3 == -1) {
      read_difficulty();
    }

    // iniziano a spegnersi i led
    pinMode(LEDR_PIN, LOW);

    Serial.println("Go!");

    button_order = 0;  // variabile usata per non usare un vettore
    int led_count = 0; // led accesi
    multiplier = 1;

    // qui spengo i led, devo spegnerli in un tempo T3
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
        delay(t2);
    }

    multiplier = multiplier / 10;

    debug("Button order = " + String(button_order));

    noInterrupts();
    start_time = millis();
    debug("t3 = " + String(t3));
    current_state = TURN;
    debug("TURN");

    points = 0;
    Serial.println("points: " + String(points));
    interrupts();
}