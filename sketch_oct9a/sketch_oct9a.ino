#include <Arduino.h>
#include <avr/sleep.h>
#include "TimerOne.h"

#define LEDG_PIN13 13 //button 5
#define LEDG_PIN12 12 //button 4
#define LEDG_PIN11 11 //button 3
#define LEDG_PIN10 10 //button 2
#define LEDr_PIN9 9
#define POT_PIN A0
#define BUTTON_PIN2 2
#define BUTTON_PIN3 3
#define BUTTON_PIN4 4
#define BUTTON_PIN5 5

typedef enum {
  BOOT, DEMO, TURN, FAILED
} State;

State current_state = BOOT;

//variabili per i tempi

//millisecondi prima di iniziare il gioco
int t1 = 2000;

// millisecondi prima di spegnere il led dopo
int t2_delays[] = {1500, 1200, 900, 500};
int t2 = 1500; 

// millisecondi per accendere i led
int t3= 10000;
int t3_delays[] = {10000, 6000, 4000, 2000};

volatile int clac; //bottone premuto
TimerOne* timer; 
bool gameStarted = false; // Flag per indicare se il gioco è iniziato
int currIntensity;
int fadeAmount;

void wakeUp(){}

void clic2(){
  clac=2;
  digitalWrite(LEDG_PIN10, HIGH);
  Serial.println("primo bottone");
}
void clic3(){
  clac=3;
  digitalWrite(LEDG_PIN11, HIGH);
  Serial.println("secondo bottone");
}

void clic4(){
  clac=4;
  digitalWrite(LEDG_PIN12, HIGH);
  Serial.println("terzo bottone");
}

void clic5(){
  clac=5;
  digitalWrite(LEDG_PIN13, HIGH);
  Serial.println("quarto bottone");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDG_PIN13, OUTPUT);
  pinMode(LEDG_PIN12, OUTPUT);
  pinMode(LEDG_PIN11, OUTPUT);
  pinMode(LEDG_PIN10, OUTPUT);
  pinMode(LEDr_PIN9, OUTPUT);

  pinMode(BUTTON_PIN2, INPUT);
  pinMode(BUTTON_PIN3, INPUT);
  pinMode(BUTTON_PIN4, INPUT);
  pinMode(BUTTON_PIN5, INPUT);

  currIntensity = 0;
  fadeAmount = 5;

  Serial.begin(9600);

  // Inizializza la generazione dei numeri casuali
  randomSeed(analogRead(0));

  //timer per lo sleep
  timer = new TimerOne();
  timer->setPeriod(10000); //10 sec
  //timer.attachInterrupt();


  //interrupt per accendere luci
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN2), clic2, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN3), clic3, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN4), clic4, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN5), clic5, RISING);
  //interrupt che si attiva se schiacci uno dei bottoni
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN2), wakeUp, RISING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN3), wakeUp, RISING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN4), wakeUp, RISING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN5), wakeUp, RISING); 
}

void read_difficulty() {
  int potentiometer_value = analogRead(POT_PIN); 
  Serial.print("Potentiometer value = ");
  Serial.println(potentiometer_value);
  int difficulty = map(potentiometer_value, 0, 1023, 0, 3);
  Serial.print("Difficulty = ");
  Serial.println(difficulty);
  t2 = t2_delays[difficulty];
  t3 = t3_delays[difficulty];
}

void boot() {
  Serial.println("\n------------------------");
  Serial.println("BOOT");
  //stato iniziale di gioco dove tutti i led verdi sono accesi
  digitalWrite(LEDG_PIN13, HIGH);
  digitalWrite(LEDG_PIN12, HIGH);
  digitalWrite(LEDG_PIN11, HIGH);
  digitalWrite(LEDG_PIN10, HIGH);
  digitalWrite(LEDr_PIN9, HIGH);

  delay(t1); //tempo attesa inizio gioco

  current_state = DEMO;
}

void demo() {
  Serial.println("DEMO");
  read_difficulty();

  //iniziano a spegnersi i led
  pinMode(LEDr_PIN9, LOW);

  //manca la verifica che non venga spento due volte un led
  int salvo=0; //variabile usata per non usare un vettore
  int cont= 4; //led accesi
  int zero= 1;

  //qui spengo i led, devo spegnerli in un tempo T3
  while(cont>0)
  {
    int n= random(0, 4);
    switch(n){
      case 3: 
        if(digitalRead(LEDG_PIN13)== HIGH){
          digitalWrite(LEDG_PIN13, LOW);
          salvo= salvo+3*zero;
          zero = zero*10;
          cont--;
          delay(t2);
          Serial.println(sprintf("\n%d – terzo", salvo));
        }
        break;
      case 2:
        if(digitalRead(LEDG_PIN12)== HIGH){
          digitalWrite(LEDG_PIN12, LOW);
          salvo= salvo+2*zero;
          zero = zero*10;
          cont--;
          delay(t2);
          Serial.println(sprintf("\n%d – secondo", salvo));
        }
        break;
      case 1:
        if(digitalRead(LEDG_PIN11)== HIGH){
          digitalWrite(LEDG_PIN11, LOW);
          salvo= salvo+1*zero;
          zero = zero*10;
          cont--;
          delay(t2);
          Serial.println(sprintf("\n%d – primo", salvo));
        }
        break;
      case 0:
        if(digitalRead(LEDG_PIN10)== HIGH){
          digitalWrite(LEDG_PIN10, LOW);
          salvo= salvo+0*zero;
          zero = zero*10;
          cont--;
          delay(t2);
          Serial.println(sprintf("\n%d – zero", salvo));
        }
        break;
    }
  }

  zero = zero /10;
  
  
  //inizio clic bottone utente
  int clic=0; //questa variabile contiene il numero del bottone che deve essere cliccato
  Serial.println("ordine numeri usciti");
  Serial.println(salvo);

  interrupts(); //abilito le interruzioni
  clic = salvo/zero;
  salvo= salvo%zero;

  

  Serial.println("\n primo numero che premo");
  Serial.println(clic);
  
  

  int i =0;
  //se il clic del bottone corrisponde a quello giusto allora vai avanti (ciclo che esci quando sbagli o quando finisci)
  while(i<4){
    
    if(clic==clac){
      clic = salvo/zero;
      salvo= salvo%zero;
    }
    else{
      noInterrupts();
      digitalWrite(LEDr_PIN9, HIGH);
      break;

      
    }
    i++;
  }
  //se clic sbagliato esci e fai ricominciare il gioco

  
  Serial.println("\n clac");
  Serial.println(clac);

  current_state = TURN;

  delay(100000000000000000000000);
}

void turn() {
  
}

void loop() {
  noInterrupts();
  switch (current_state) {
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
  interrupts();

    //se gioco è iniziato
  /*if(!gameStarted){
      //luce rossa inizia a pulsare
      analogWrite(LEDr_PIN9, currIntensity);
      currIntensity = currIntensity + fadeAmount;
      if (currIntensity == 0 || currIntensity == 255) {
        fadeAmount = -fadeAmount;
      }
      delay(20);

      // Primo messaggio sulla seriale
      Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start \n");

      //inizia il timer
      timer->start();

    while (!gameStarted) { // Attendi il pulsante B1
      if (digitalRead(BUTTON_PIN2) == HIGH) {
        gameStarted = true; // Il gioco è iniziato
        timer->stop(); // Interrompi il timer
        break;
      }
  }

  //se gioco non è iniziato in deep sleep
  if(!gameStarted){
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();

    //si sveglia quando viene premuto un pulsante qualsiasi
  }
*/ 


}
