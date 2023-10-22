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
            debug("Wrong button. Game over!");
            attemps = 4;
            points = 0;
            current_state = BOOT;
        }

        if (attemps == 0)
        {
            // TODO: Move to correct location.
            time_data.t1 = (int)time_data.t1 * 0.95;
            time_data.t2 = (int)time_data.t2 * 0.95;
            time_data.t3 = (int)time_data.t3 * 0.95;
            points++;
            Serial.println("New Point! Total Points:");
            Serial.println(points);

            attemps = 4;
            current_state = BOOT;
        }
        clac = -1;
    }
    interrupts();
}