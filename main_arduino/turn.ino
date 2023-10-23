void turn()
{
    noInterrupts();
    if (clac != -1)
    {
        debug("Registered press of button " + String(clac));
        clic = button_order / multiplier;

        if (clic == clac)
        {
            clic = button_order / multiplier;
            button_order = button_order % multiplier;
            multiplier = multiplier / 10;
            attemps--;
            debug("Correct button pressed!");
        }
        else
        {
            attemps = 4;
            points = 0;
            delay(1000);
            digitalWrite(LEDG_PIN4, LOW);
            digitalWrite(LEDG_PIN3, LOW);
            digitalWrite(LEDG_PIN2, LOW);
            digitalWrite(LEDG_PIN1, LOW);
            digitalWrite(LEDR_PIN, HIGH); //led rosso si accende per un secondo
            noInterrupts();
            delay(1000);
            debug("Wrong button. Game over!!");
            
            digitalWrite(LEDR_PIN, HIGH);
            delay(10000);
                        current_state = BOOT;
            debug("pin rosso acceso");

            
            
            game_started = true; //torna in fase iniziale con luce rossa che pulsa
        }

        if (attemps == 0)
        {
            // TODO: Move to correct location.
            t1 = (int)t1 * 0.95;
            t2 = (int)t2 * 0.95;
            t3 = (int)t3 * 0.95;
            points++;
            Serial.println("New Point! Score:");
            Serial.println(points);

            attemps = 4;
            current_state = BOOT;
        }
        clac = -1;
    }
    interrupts();
}