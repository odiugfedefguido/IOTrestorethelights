#include <Arduino.h>
#include <avr/sleep.h>
#include <TimerOne.h>
#include <EnableInterrupt.h>

#define LEDG_PIN1 10 // button 2
#define LEDG_PIN2 11 // button 3
#define LEDG_PIN3 12 // button 4
#define LEDG_PIN4 13 // button 5

#define LEDR_PIN 6
#define POT_PIN A0

#define BUTTON_PIN1 2
#define BUTTON_PIN2 3
#define BUTTON_PIN3 4
#define BUTTON_PIN4 5

typedef enum { BOOT, DEMO, TURN, FAILED } State;
State current_state = BOOT;

// variabili per i tempi
typedef struct
{
    int t1; // millisecondi prima di iniziare il gioco
    int t2; // millisecondi prima di spegnere il led dopo
    int *t2_delays;
    int t3; // millisecondi per accendere i led
    int *t3_delays;
} TimeData;

TimeData time_data = 
{
    2000, 15000, (1500, 1200, 900, 500), 1000, (10000, 6000, 4000, 2000)
};

int multiplier;
int button_order;

volatile int clac = -1; // bottone premuto
int clic = 0;
volatile int attemps = 4;
volatile int points = 0;

TimerOne *timer;
int curr_intensity;
int fade_amount;
bool game_started = false; // Flag per indicare se il gioco è iniziato
unsigned long wait_time = 10000;
unsigned long start_time;

// debug
void debug(char* message);

// buttons
void on_button_1_clicked();
void on_button_2_clicked();
void on_button_3_clicked();
void on_button_4_clicked();
void wake_up();

// game loop
void setup();
void boot();
void demo();
void turn();

void loop()
{
    switch (current_state)
    {
    case BOOT:
        boot();
        break;
    case DEMO:
        demo();
        break;
    case TURN:
        turn();
        break;
    default:
        break;
    }
}
