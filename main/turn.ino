void game_over();

void turn()
{
    if(millis() - start_time >= t3){
        debug("Time is over");
        game_over();
        return;
    }
    
    noInterrupts();
    if (clac != -1)
    {
        debug("Registered press of button " + String(clac));
        clic = button_order / multiplier;

        if (clic == clac)
        {
            debug("Correct button pressed!");
            debug("attempts = " + String(attemps));
            clic = button_order / multiplier;
            button_order = button_order % multiplier;
            multiplier = multiplier / 10;
            attemps--;
        }
        else
        {
            debug("Wrong button. Game over!");
            game_over();
        }

        if (attemps == 0)
        {
            t1 = (int)t1 * 0.95;
            t2 = (int)t2 * 0.95;
            t3 = (int)t3 * 0.95;
            points++;
            Serial.println("New Point! Score: " + String(points));

            attemps = 4;
            current_state = DEMO;
            game_started = false;
        }

        interrupts();
        delay(200);
        clac = -1;
    }
    interrupts();
}

void game_over() 
{
  Serial.println("Game over! Final score: " + String(points));

  digitalWrite(LEDG_PIN4, LOW);
  digitalWrite(LEDG_PIN3, LOW);
  digitalWrite(LEDG_PIN2, LOW);
  digitalWrite(LEDG_PIN1, LOW);

  current_state = FAILED;
  interrupts();
  analogWrite(LEDR_PIN, 255); //led rosso si accende per un secondo
  delay(1000);
  analogWrite(LEDR_PIN, 0);
  delay(10000);
  noInterrupts();

  // reset values
  attemps = 4;
  points = 0;
  t3 = -1;
  current_state = BOOT;            
  game_started = false;
}