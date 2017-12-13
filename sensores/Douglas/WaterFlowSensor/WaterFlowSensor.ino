volatile int  flow_frequency;  // Mede os pulsos do medidor de fluxos
 float  l_min;          // Cálculo de litros/min                      
unsigned char flowmeter = 2;   // Pino de medição do Fluxo
unsigned long currentTime;
unsigned long cloopTime;

void flow ()                  // Função de Interrupção
{ 
   flow_frequency++;
} 

void setup()
{ 
   pinMode(flowmeter, INPUT);
   Serial.begin(9600); 
   attachInterrupt(0, flow, RISING); // Configura a Interrupção 
                                     // see http://arduino.cc/en/Reference/attachInterrupt
   sei();                            // Habilita interrução
   currentTime = millis();
   cloopTime = currentTime;
} 

void loop ()    
{
   currentTime = millis();
   // Calcula a cada segundo a taxa de litros/hour
   if(currentTime >= (cloopTime + 1000))
   {     
      cloopTime = currentTime;
      // Frequência de pulso (Hz) = 7.5Q, Q é taxa de fluxo em L/min. (Variação de +/- 3%)
      l_min = (flow_frequency / 7.5); // (Frequência de pulso x 60 min) / 7.5Q = Quociente de vazão em L/min 
      flow_frequency = 0;                   // Reinica o contador
      Serial.print(l_min);            // Imprime os litros/hour
      Serial.println(" L/min");
   }
}

