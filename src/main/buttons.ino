// restore from the sleep
void wake_up() {}

 /*  
 * Check if the button is being pressed.  
 * It's checked just if it is in the TURN states.  
 */
void check_button_input(int button, int pin)
{
  wake_up();
  if (current_state == TURN) 
  {
    clac = button;
    digitalWrite(pin, HIGH);
    debug("Button " + String(button) + " clicked");
    interrupts();
    delay(100);
    noInterrupts();
  }
}

// functions to capture the clicked button and the corresponding led
void on_button_1_clicked()
{
  check_button_input(0, LEDG_PIN1);
}

void on_button_2_clicked()
{
  check_button_input(1, LEDG_PIN2);
}

void on_button_3_clicked()
{
  check_button_input(2, LEDG_PIN3);
}

void on_button_4_clicked()
{
  check_button_input(3, LEDG_PIN4);
}