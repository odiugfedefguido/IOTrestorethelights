#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
  BOOT, DEMO, TURN, FAILED
} State;

extern State current_state;

void boot();
void demo();
void turn();

#endif
