#include <FanController.h>      // Biblioteca que controla a velocidade de um cooler

#define SENSOR_PIN 2            // Pino do sensor é o Digital 2 ligado no fio verde do cooler
#define SENSOR_THRESHOLD 1000   // Constante para quantidade de leituras do tacômetro
#define PWM_PIN 9               // Pino de controle de velocidade do cooler ligado no fio azul

FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN); // Criando a instância do cooler

#define VELOCIDADE 100 // Percentual da velocidade do cooler de 0 a 100
/*
 * Velocidade máxima do cooler: 3200 RPM (100%)
 * 
 * Velocidade mínima do cooler: 800 RPM  (0%)
 * 
 */

#include <EduIntro.h>
#define LM35_PIN A0 // Pino de saida do LM35 (Fio Verde do LM35 ligado na porta analógica A0)
LM35 lm35(LM35_PIN);  // creating the object 'lm35' on pin A0
double C;

double readTemperature (LM35 sensor, int samples) {
  double soma = 0;
   for ( int i=0 ; i < samples ; i++ ) {
      soma += sensor.readCelsius(); 
   }

   return soma / samples;
}

int velocidadeMapeada( int velocidade ) {
  return map(velocidade, 0, 100, 0, 65); // Valores descobertos empiricamente. O fan rodando a 60 na chamada de DutyCycle equilibra o lado frio em 1C
}


int velocidade_final = velocidadeMapeada(VELOCIDADE);

void setup()
{

  Serial.begin(9600); // Iniciando a comunicação serial
  
  fan.begin(); // Iniciando o fan

  
  fan.setDutyCycle(velocidade_final); // Seleção de velocidade (mudar VELOCIDADE para uma variável que seja um numero entre 0 e 100)

  Serial.println();
  Serial.println("VELOCIDADE\tRPM\tTEMPERATURA");

}

void loop()
{
  Serial.print(fan.getSpeed());
  Serial.print("RPM\t");

  C = readTemperature(lm35, 200);       // Reading the temperature in Celsius degrees
  Serial.println(C);

  if (Serial.available() > 0) {
    // Parse speed
    int input = Serial.parseInt();
    velocidade_final = velocidadeMapeada(input);

    // Print obtained value
    Serial.print("Setting duty cycle: ");
    Serial.println(velocidade_final, DEC);

    // Set fan duty cycle
    fan.setDutyCycle(velocidade_final);

    // Get duty cycle
    byte dutyCycle = fan.getDutyCycle();
    Serial.print("Duty cycle: ");
    Serial.println(dutyCycle, DEC);

    int trash = Serial.parseInt();
  }
  
}
