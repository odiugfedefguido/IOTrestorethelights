void turn()
{
    if(millis() - start_time >= t3){
      Serial.println("Game Over, Time is over");
      current_state=BOOT;
    }
    
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
            debug("attempts = " + String(attemps));
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

            current_state = FAILED;
            interrupts();
            analogWrite(LEDR_PIN, 255); //led rosso si accende per un secondo
            delay(1000);
            analogWrite(LEDR_PIN, 0);

            debug("Wrong button. Game over!!");
            t3 = -1;
            Serial.println("Final score: " + String(points));
            delay(10000);
            noInterrupts();

            current_state = BOOT;

            
            
            game_started = false; //torna in fase iniziale con luce rossa che pulsa
        }

        if (attemps == 0)
        {
            t1 = (int)t1 * 0.95;
            t2 = (int)t2 * 0.95;
            t3 = (int)t3 * 0.95;
            points++;
            Serial.println("New Point! Score: " + String(points));

            attemps = 4;
            current_state = BOOT;
            game_started = false;
        }
        clac = -1;
    }
    interrupts();
}