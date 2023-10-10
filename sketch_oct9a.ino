#define LEDG_PIN13 13
#define LEDG_PIN12 12
#define LEDG_PIN11 11
#define LEDG_PIN10 10
#define LEDr_PIN9 9
#define POT_PIN A0
#define BUTTON_PIN2 2
#define BUTTON_PIN4 4
#define BUTTON_PIN5 5
#define BUTTON_PIN6 6

//variabili per i tempi
int t1 = 2000; //millisecondi prima di iniziare il gioco
int t3= 1000; //millisecondi prima di spegnere il led dopo

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDG_PIN13, OUTPUT);
  pinMode(LEDG_PIN12, OUTPUT);
  pinMode(LEDG_PIN11, OUTPUT);
  pinMode(LEDG_PIN10, OUTPUT);
  pinMode(LEDr_PIN9, OUTPUT);

  pinMode(BUTTON_PIN2, INPUT);
  pinMode(BUTTON_PIN4, INPUT);
  pinMode(BUTTON_PIN5, INPUT);
  pinMode(BUTTON_PIN6, INPUT);

  Serial.begin(9600);

   // Inizializza la generazione dei numeri casuali
  randomSeed(analogRead(0));
  


}

void loop() {
  // put your main code here, to run repeatedly:

  //stato iniziale di gioco dove tutti i led verdi sono accesi
  digitalWrite(LEDG_PIN13, HIGH);
  digitalWrite(LEDG_PIN12, HIGH);
  digitalWrite(LEDG_PIN11, HIGH);
  digitalWrite(LEDG_PIN10, HIGH);
  digitalWrite(LEDr_PIN9, HIGH);
  Serial.println("tutti led accesi");
  delay(t1); //tempo attesa inizio gioco

  //iniziano a spegnersi i led
  Serial.println("iniziano a spegnersi i led");
  pinMode(LEDr_PIN9, LOW);

  //manca la verifica che non venga spento due volte un led
  int salvo=0; //variabile usata per non usare un vettore
  int cont= 4; //led accesi

  //qui spengo i led, devo spegnerli in un tempo T3
  while(cont>0){
    int n= random(0, 4);
    switch(n){
      case 3: 
        if(digitalRead(LEDG_PIN13)== HIGH){
          digitalWrite(LEDG_PIN13, LOW);
          salvo= n*cont;
          cont--;
          delay(t3);
        }
        break;
      case 2:
        if(digitalRead(LEDG_PIN12)== HIGH){
          digitalWrite(LEDG_PIN12, LOW);
          salvo= n*cont;
          cont--;
          Serial.print("\n spengo led 2");
          delay(t3);
        }
        break;
      case 1:
        if(digitalRead(LEDG_PIN11)== HIGH){
          digitalWrite(LEDG_PIN11, LOW);
          salvo= n*cont;
          cont--;
          Serial.print("\n spengo led 1");
          delay(t3);
        }
        break;
      case 0:
        if(digitalRead(LEDG_PIN10)== HIGH){
          digitalWrite(LEDG_PIN10, LOW);
          salvo= n*cont;
          cont--;
          Serial.print("\n spengo led 0");
          delay(t3);
        }
        break;
    }
    
    Serial.print("\n cont valore fine switch");
    Serial.print(cont);
  }

  Serial.print("esce dal ciclo spegni");

  //inizio clic bottone utente
  int clic=0; //questa variabile contiene il numero del bottone che deve essere cliccato




}