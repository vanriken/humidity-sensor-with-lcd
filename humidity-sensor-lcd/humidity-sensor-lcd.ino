#include <LiquidCrystal.h>
#include "SparkFun_SHTC3.h"
#include <stdint.h>

#define PIN_RS 2
#define PIN_ENABLE 3
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7
#define PIN_CONTRAST 9

#define LCD_NR_OF_COLS 16
#define LCD_NR_OF_ROWS 2

LiquidCrystal lcd(PIN_RS, PIN_ENABLE, PIN_D4, PIN_D5, PIN_D6, PIN_D7);
SHTC3 mySHTC3; // Declare an instance of the SHTC3 class

uint8_t message[16];
uint8_t buf[6];

void setup() {

  pinMode(PIN_CONTRAST, OUTPUT);
  analogWrite(PIN_CONTRAST, 50);
  
  Serial.begin(9600);
  Wire.begin();
  
  Serial.print("Starting the sensor. Result = ");   
  errorDecoder(mySHTC3.begin());  // To start the sensor you must call "begin()", the default settings use Wire (default Arduino I2C port)
  Serial.println();
  Serial.println("Starting the sensor, please wait.");
  
  lcd.begin(LCD_NR_OF_COLS, LCD_NR_OF_ROWS);
  
  lcd.write("SHTC3 sensor");
  lcd.setCursor(0,1);  // first column, second row
  lcd.print("Please wait");
  
  lcd.setCursor(15,1);
  lcd.blink();
  
  delay(5000);
}

void loop() {
  SHTC3_Status_TypeDef sensor_result;
  float temperature;
  float humidity;
  static float previous_temperature = 0;
  static float previous_humidity = 0;

  sensor_result = mySHTC3.update();
  
  temperature = mySHTC3.toDegC();
  humidity = mySHTC3.toPercent();

  if ((abs(temperature - previous_temperature) >= 0.1) ||
      (abs(humidity - previous_humidity) >= 0.1))
  {
    Serial.println(temperature);
    Serial.println(previous_temperature);
    Serial.println(humidity);
    Serial.println(previous_humidity);
    
    lcd.clear();

    // update the temperature on the LCD screen
    dtostrf(temperature, 3, 1, buf);
    sprintf(message, "Temp.: %s C", buf);
    
    lcd.print((char*) message);

    // update the humidity on the LCD screen
    dtostrf(humidity, 3, 1, buf);
    sprintf(message, "Hum. : %s %%", buf);
  
    lcd.setCursor(0,1);  // second column, first row
    lcd.print((char*) message);

    lcd.setCursor(15,1);
    lcd.blink();

    previous_temperature = temperature;
    previous_humidity = humidity;
  }
  delay(200);
}

void errorDecoder(SHTC3_Status_TypeDef message) // The errorDecoder function prints "SHTC3_Status_TypeDef" resultsin a human-friendly way
{
  switch(message)
  {
    case SHTC3_Status_Nominal : Serial.println("Nominal"); break;
    case SHTC3_Status_Error : Serial.println("Error"); break;
    case SHTC3_Status_CRC_Fail : Serial.println("CRC Fail"); break;
    default : Serial.println("Unknown return code"); break;
  }
}
