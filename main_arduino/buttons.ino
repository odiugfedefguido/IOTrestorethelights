void wake_up() {}

void check_button_input(int button, int pin)
{
  if (current_state == TURN) 
  {
    clac = button;
    digitalWrite(pin, HIGH);
    debug("Button " + String(button) + " clicked");
    wake_up();
  }
}

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