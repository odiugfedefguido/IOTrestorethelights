void game_over();

void turn()
{
    // if the user doesn't finish the game on time --> time out
    if(millis() - start_time >= t3){
        debug("Time is over");
        game_over();
        return;
    }
    
    noInterrupts();

    // if the user has pressed a button
    if (clac != -1)
    {
        debug("Registered press of button " + String(clac));

        // to obtain which button to press
        // ie: 3201/1000 = 3 -> button 3
        clic = button_order / multiplier; 

        // if the user has pressed the correct button
        if (clic == clac)
        {
            debug("Correct button pressed!");
            debug("attempts = " + String(attempts));
            clic = button_order / multiplier;
            button_order = button_order % multiplier;
            multiplier = multiplier / 10;
            attempts--;
        }
        else
        {
            debug("Wrong button. Game over!");
            game_over();
        }

        // when you finished to press all buttons
        if (attempts == 0)
        {
            t1 = (int)t1 * 0.95;
            t2 = (int)t2 * 0.95;
            t3 = (int)t3 * 0.95;
            points++;
            Serial.println("New Point! Score: " + String(points));

            attempts = 4;
            current_state = DEMO;
            game_started = false;
        }

        interrupts();
        delay(200);
        clac = -1;
    }
    interrupts();
}

// function to manage the game over
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
  // goes into sleep for 10 second
  delay(10000); 
  noInterrupts();

  // reset values
  attempts = 4;
  points = 0;
  t3 = -1;
  current_state = BOOT;            
  game_started = false;
}