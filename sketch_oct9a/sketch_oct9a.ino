#define LEDG_PIN13 13
#define LEDG_PIN12 12
#define LEDG_PIN11 11
#define LEDG_PIN10 10
#define LEDr_PIN9 9
#define POT_PIN A0
#define BUTTON_PIN2 2
#define BUTTON_PIN4 3
#define BUTTON_PIN5 4
#define BUTTON_PIN6 5

//variabili per i tempi
int difficulty_delays[] = {1500, 1200, 900, 500};
int t1 = 2000; //millisecondi prima di iniziare il gioco
int t2 = 0; 
int t3= 5000; //millisecondi prima di spegnere il led dopo

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

void read_difficulty() {
  int potentiometer_value = analogRead(POT_PIN); 
  Serial.print("Potentiometer value = ");
  Serial.println(potentiometer_value);
  int difficulty = map(potentiometer_value, 0, 1023, 0, 3);
  Serial.print("Difficulty = ");
  Serial.println(difficulty);
  t2 = difficulty_delays[difficulty];
}

void loop() {

  
  // put your main code here, to run repeatedly:
  Serial.println("------------------------");
  //stato iniziale di gioco dove tutti i led verdi sono accesi
  digitalWrite(LEDG_PIN13, HIGH);
  digitalWrite(LEDG_PIN12, HIGH);
  digitalWrite(LEDG_PIN11, HIGH);
  digitalWrite(LEDG_PIN10, HIGH);
  digitalWrite(LEDr_PIN9, HIGH);
  delay(t1); //tempo attesa inizio gioco

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
          delay(t3);
          Serial.println("\n");
          Serial.println(salvo);
          Serial.println("\n");
          Serial.println("terzo");
          
        }
        break;
      case 2:
        if(digitalRead(LEDG_PIN12)== HIGH){
          digitalWrite(LEDG_PIN12, LOW);
          salvo= salvo+2*zero;
          zero = zero*10;
          cont--;
          delay(t3);
                    Serial.println("\n");
          Serial.println(salvo);
                    Serial.println("\n");
          Serial.println("secondo");
        }
        break;
      case 1:
        if(digitalRead(LEDG_PIN11)== HIGH){
          digitalWrite(LEDG_PIN11, LOW);
          salvo= salvo+1*zero;
          zero = zero*10;
          cont--;
          delay(t3);
                    Serial.println("\n");
          Serial.println(salvo);
                    Serial.println("\n");
          Serial.println("primo");
        }
        break;
      case 0:
        if(digitalRead(LEDG_PIN10)== HIGH){
          digitalWrite(LEDG_PIN10, LOW);
          salvo= salvo+0*zero;
          zero = zero*10;
          cont--;
          delay(t3);
                    Serial.println("\n");
          Serial.println(salvo);
                    Serial.println("\n");
          Serial.println("zero");
        }
        break;
    }
  }

  zero = zero /10;
  

  //inizio clic bottone utente
  int clic=0; //questa variabile contiene il numero del bottone che deve essere cliccato
  Serial.println("ordine numeri usciti");
  Serial.println(salvo);
  int clac; //numero premuto
  int buttonstate= digitalRead(BUTTON_PIN2);
  clic = salvo/zero;

  if(buttonstate== true){
    Serial.println("bottone premuto");
    digitalWrite(LEDr_PIN9, HIGH);
  }

  Serial.println("\n primo numero che premo");
  Serial.println(clic);

  


  //se il clic del bottone corrisponde a quello giusto allora vai avanti (ciclo che esci quando sbagli o quando finisci)
  //se clic sbagliato esci e fai ricominciare il gioco



  delay(1000000000000000000000);
  


}