//SENSOR DE VIBRAÇÃO
int pino_A2 = A2;
int pino_D3 = 3;
int leitura_analogica = 0;
int leitura_analogica_ant = 0;
int leitura_digital = 0;


int led = 6;

//SENSOR DE CHAMA
int pino_D0 = 7;
int pino_A0 = A1;
int pino_D2 = 2;
int valor_a = 0;
int valor_d = 0;
float seno;
int frequencia;

void setup(){
  Serial.begin(9600);
  
  //SENSOR DE CHAMA
  pinMode(pino_A0, INPUT);
  pinMode(pino_D0, INPUT);
  pinMode(pino_D2,OUTPUT);
  pinMode(led, OUTPUT);

  //SENSOR DE VIBRAÇÃO
  pinMode(pino_A2, INPUT);
  pinMode(pino_D3, INPUT);
  
}

void loop(){

  //SENSOR DE CHAMA
  int valor_d = digitalRead(pino_D0);
  
  if (valor_d != 1){
    for (frequencia = 150; frequencia < 1800; frequencia += 1) {
      tone(pino_D2, frequencia, 10); 
      delay(1);
    }
    for (frequencia = 1800; frequencia > 150; frequencia -= 1) {
      tone(pino_D2, frequencia, 10); 
      delay(1);
    }
  }else{
    noTone(pino_D2);
    digitalWrite(led, LOW);
  }


  //SENSOR DE VIBRAÇÃO
  leitura_digital = digitalRead(pino_D3);
  
  if (leitura_digital != 1){
    digitalWrite(led,HIGH);
    tone(pino_D2, 300, 500); 
    delay(200);
  }
  
  digitalWrite(led, LOW);
  
  delay(10);
  
}

