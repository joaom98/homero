#include <PID_v1.h>
#include <EduIntro.h>

#define RelayPin 6
LM35 lm35(A0);  // creating the object 'lm35' on pin A0
double C;

//Define Variables we'll be connecting to
double Desejado, Entrada, Saida;

//Specify the links and initial tuning parameters
PID myPID(&Entrada, &Saida, &Desejado, 2000, 500, 1, DIRECT);

int WindowSize = 2000;
unsigned long windowStartTime;

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
  
  pinMode(RelayPin, OUTPUT);

  windowStartTime = millis();

  //initialize the variables we're linked to
  Desejado = 0;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}

void loop()
{
  C = readTemperature(lm35, 200);       // Reading the temperature in Celsius degrees 
  Entrada = C;
  myPID.Compute();

  Serial.print(C);
  // Serial.print("\n");

  /************************************************
     turn the output pin on/off based on pid output
   ************************************************/
  unsigned long now = millis();
  if (now - windowStartTime > WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if (Saida > now - windowStartTime) {
    digitalWrite(RelayPin, LOW);
    Serial.print(" DESLIGADO\n");
  }
  else {
    digitalWrite(RelayPin, HIGH);
    Serial.print(" LIGADO\n");
  }
}
