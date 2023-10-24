void setup()
{
    // put your setup code here, to run once:
    pinMode(LEDG_PIN1, OUTPUT);
    pinMode(LEDG_PIN2, OUTPUT);
    pinMode(LEDG_PIN3, OUTPUT);
    pinMode(LEDG_PIN4, OUTPUT);
    pinMode(LEDR_PIN, OUTPUT);

    pinMode(BUTTON_PIN1, INPUT);
    pinMode(BUTTON_PIN2, INPUT);
    pinMode(BUTTON_PIN3, INPUT);
    pinMode(BUTTON_PIN4, INPUT);

    Serial.begin(9600);

    // Inizializza la generazione dei numeri casuali
    randomSeed(analogRead(0));

    curr_intensity = 0;
    fade_amount = 5;

    // interrupt per accendere luci
    enableInterrupt(BUTTON_PIN1, on_button_1_clicked, RISING);
    enableInterrupt(BUTTON_PIN2, on_button_2_clicked, RISING);
    enableInterrupt(BUTTON_PIN3, on_button_3_clicked, RISING);
    enableInterrupt(BUTTON_PIN4, on_button_4_clicked, RISING);
}