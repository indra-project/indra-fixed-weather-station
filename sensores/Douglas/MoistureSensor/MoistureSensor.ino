int sensor_pin = A0; 
int output_value ;
unsigned long currentTime;
unsigned long cloopTime;

void setup() {
  Serial.begin(9600);
  Serial.println("Realizando leitura de humidade");
  pinMode(output_value, OUTPUT);

  currentTime = millis();
  cloopTime = currentTime;
}

void loop() {

  currentTime = millis();
   if(currentTime >= (cloopTime + 1000))
   {  
    cloopTime = currentTime;
    output_value= analogRead(sensor_pin);
    output_value = map(output_value,1024,0,0,100);
    Serial.print("Umidade: ");
    Serial.print(output_value);
    Serial.println("%");  
  }
}
