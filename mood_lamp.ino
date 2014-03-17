const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;
int tempPin = 0;
int sensorPin = 1;

float temp0;
float temp1;
float temp2;
float temp3;
float temp4;
float temp5;
float temp6;
float averageTemp;

void setup()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{ 
  getTemp();
}

void getTemp()
{
  int sensorValue; 
  float voltage, degreesC, degreesF;
  voltage = analogRead(tempPin) * 0.004882814; // Get value from temp sensor and convert to voltage
  degreesC = (voltage - 0.5) * 100.0; // Convert voltage to degrees Celsius
  degreesF = degreesC * (9.0/5.0) + 32.0; // Convert Celsius to Farenheit
  
  sensorValue = constrain(map(degreesF, 68, 78, 0, 600), 0, 600); // Map degrees Farenheit from temp sensor to 0-600

// Average every seven temperature values
  temp6 = temp5;
  temp5 = temp4;
  temp4 = temp3;
  temp3 = temp2;
  temp2 = temp1;
  temp1 = temp0;
  temp0 = sensorValue;
  
  averageTemp = (temp0 + temp1 + temp2 + temp3 + temp4 + temp5 + temp6) / 7;
  
// Pass averageTemp to showRGB
  showRGB(averageTemp);
}

void showRGB(int color)
{
  int redBrightness;
  int greenBrightness;
  int blueBrightness;
  int potentiometer;
  float brightness;
  
// Get value from potentiometer and map from 100-0
  potentiometer = constrain(map(analogRead(sensorPin), 10, 1023, 100, 0), 0, 100);

  if (color <= 256) // If value from temp sensor is mapped to less than or equal to 256
  {
    redBrightness = 0; // red is always off
    greenBrightness = color; // green goes from off to on
    blueBrightness = (255 - color); // blue goes from on to off
  }
  else if (color <= 511) // If value from temp sensor is mapped to less than or equal to 511
  {
    redBrightness = color - 256; // red goes from off to on
    greenBrightness = 255 - (color - 256); // green goes from on to off
    blueBrightness = 0; // blue is always off
  }
  else // // If value from temp sensor is mapped to greater than or equal to 512
  {
    redBrightness = 255 - (color - 512); // red on to off
    greenBrightness = 0; // green is always off  
    blueBrightness = color - 512; // blue goes from off to on
  }

  Serial.print("red ");
  Serial.print(redBrightness);
  Serial.print(" green ");
  Serial.print(greenBrightness);
  Serial.print(" blue ");
  Serial.print(blueBrightness);
  Serial.print(" ");
//  Serial.println(color);
//  Serial.println(potentiometer);  
  
// Turn the mapped value from the potentiometer into a percentage
  brightness = (float)potentiometer/100;
  
  Serial.println(brightness);

// Write redBrightness, blueBrightness, and greenBrightness to the corresponding pin and multiply by brightness percentage to dim the light  
  analogWrite(RED_PIN, redBrightness * brightness);
  analogWrite(BLUE_PIN, blueBrightness * brightness);
  analogWrite(GREEN_PIN, greenBrightness * brightness);
}
