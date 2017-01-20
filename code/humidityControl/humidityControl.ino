#include <DHT.h>
#include <math.h>

#define RELAIS 8 
#define OUTSIDE 9
#define INSIDE 10
#define INDICATORLED 13

double absHumidity(double temp, double relHumidity){
  double exponent=(17.67 * temp) / (temp+243.5);
  return 13.2471488 * exp(exponent) * relHumidity / (273.15+temp);
}


DHT dhtI(INSIDE, DHT22);
DHT dhtA(OUTSIDE, DHT22);

void setup() {
  Serial.begin(9600);
  Serial.println("Start!");
  pinMode(RELAIS, OUTPUT);
  pinMode(INDICATORLED, OUTPUT);
  dhtI.begin();
  dhtA.begin();
  digitalWrite(RELAIS, HIGH);
  digitalWrite(INDICATORLED, LOW);
}

void loop() {


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read relative humidity and temperature as Celsius (default)
  //Outside
  float relHumA = dhtA.readHumidity();
  float tempA = dhtA.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(relHumA) || isnan(tempA)) {
    Serial.println("Failed to read from DHT Outside sensor!");
    digitalWrite(INDICATORLED, HIGH);
    return;
  }
  double humA = absHumidity(tempA, relHumA);
  //Inside
  float relHumI = dhtI.readHumidity();
  float tempI = dhtI.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(relHumI) || isnan(tempI)) {
    Serial.println("Failed to read from DHT Inside sensor!");
    digitalWrite(INDICATORLED, HIGH);
    return;
  }
  double humI = absHumidity(tempI, relHumI);
  
  Serial.print("humA=");
  Serial.print(relHumA);
  Serial.print(" tempA=");
  Serial.print(tempA);
  Serial.print(" aHumA=");
  Serial.print(humA);
  Serial.print(" humI=");
  Serial.print(relHumI);
  Serial.print(" tempI=");
  Serial.print(tempI);
  Serial.print(" aHumI=");
  Serial.println(humI);

  // reduce the humidity in the basement but make sure you dont freeze it ;)
  if ( humA < humI && tempI > 5.){ 
    digitalWrite(RELAIS, LOW); // LOW means ON
  }else{
    digitalWrite(RELAIS, HIGH); // HIGH means OFF
  }
  
  digitalWrite(INDICATORLED, LOW); // Indicate everything is ok
  // Wait 30 seconds between measurements.
  delay(30000);

  
}
