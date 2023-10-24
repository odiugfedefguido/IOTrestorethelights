/*
 * GROUP MEMBERS
 * Chiara De Nardi
 * Federica Guiducci
 * Florian Knoch
 */

#include <Arduino.h>
#include <avr/sleep.h>
#include <EnableInterrupt.h>

// green LED pins
#define LEDG_PIN1 10 // button 2
#define LEDG_PIN2 11 // button 3
#define LEDG_PIN3 12 // button 4
#define LEDG_PIN4 13 // button 5

// red LED pin
#define LEDR_PIN 6

// potentiometer pin
#define POT_PIN A0

// button pins
#define BUTTON_PIN1 2
#define BUTTON_PIN2 3
#define BUTTON_PIN3 4
#define BUTTON_PIN4 5

// game states
typedef enum { BOOT, DEMO, TURN, FAILED } State;
State current_state = BOOT;

// time variables
int t0 = 10000; // milliseconds before going to sleep
int t1 = 1000; // milliseconds before turning on all green lights
int t2 = -1; // milliseconds to turn off each light
int t3 = -1; // milliseconds to turn all of them them back on 
// initial value -1 to keep track of whether this is a fresh boot or not

// variables to store the button order in a single int
int multiplier; 
int button_order;

// the button that was pressed last (-1 = none)
volatile int clac = -1;

// the button that is supposed to be pressed next
int clic = 0;

// how many buttons remain to be pressed in a turn
volatile int attempts = 4;

// the game score
volatile int points = 0;

// whether the game has begun already
bool game_started = false;

// function for debugging
void debug(char* message);

// functions to capture the clicked button
void on_button_1_clicked();
void on_button_2_clicked();
void on_button_3_clicked();
void on_button_4_clicked();

// restore from the sleep
void wake_up();

// functions of the state of the game's loop
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
