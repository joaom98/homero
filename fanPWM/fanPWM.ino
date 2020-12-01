#include <FanController.h>

#define SENSOR_PIN 2 // Fio Verde
#define SENSOR_THRESHOLD 1000
#define PWM_PIN 9 // Fio Azul
FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN);


#include <PID_v1.h>
#include <EduIntro.h>

LM35 lm35(A0);  // creating the object 'lm35' on pin A0

double C;

//Define Variables we'll be connecting to
double Desejado, Entrada, Saida;

//Specify the links and initial tuning parameters
PID myPID(&Entrada, &Saida, &Desejado, 100, 500, 1, REVERSE);


double readTemperature (LM35 sensor, int samples) {
  double soma = 0;
   for ( int i=0 ; i < samples ; i++ ) {
      soma += sensor.readCelsius(); 
   }

   return soma / samples;
}

void setup()
{

  Serial.begin(9600);
  
  fan.begin();

  // Get duty cycle
  byte dutyCycle = fan.getDutyCycle();
  Serial.print("Duty cycle: ");
  Serial.println(dutyCycle, DEC);


  //initialize the variables we're linked to
  Desejado = 10;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, 100);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}

void loop()
{
  C = readTemperature(lm35, 200);       // Reading the temperature in Celsius degrees 
  Entrada = C;
  myPID.Compute();

  // Set fan duty cycle
  //fan.setDutyCycle(Saida);

  fan.setDutyCycle(Saida);


  Serial.print(C);
  Serial.print("\t");
  Serial.print(Saida);
  Serial.print("\t");
  Serial.print(fan.getSpeed());
  Serial.println("RPM");
}
