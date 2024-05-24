
/* This is a test skecth that can be used for internal testing of Nano 33 BLE sense sensors */
#include <stdlib.h>
#include <Arduino_HTS221.h>   //  Tempeature and Humidity sensor

#include <Arduino_LPS22HB.h>  // Pressure sensor

#include <Arduino_LSM9DS1.h>  // IMU 

#include <Arduino_APDS9960.h>   // Color sensor

String Message = "";
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  // Initiate the HTS2221 Sensor
  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }

  // Initiate the pressure sensor
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
  // initiate IMU
  if (!IMU.begin()) {  
    Serial.println("IMU init failed");  
    while (1) {}  
  } 
  // Initiate Color sensor
  if(!APDS.begin()){
    Serial.println("Failed to initialize colour sensor!");
  }

}

void loop() {
  // read all the sensor values
  float temperature = HTS.readTemperature();
  float humidity    = HTS.readHumidity();
  float pressure = BARO.readPressure();
  float x, y, z;  // IMU
  int r, g, b;  // Color sensor

  // Color sensor Test
  if (APDS.colorAvailable()) {

    // Read Color
    APDS.readColor(r, g, b);

    // print the values
    Serial.print("r = ");
    Serial.println(r);
    Serial.print("g = ");
    Serial.println(g);
    Serial.print("b = ");
    Serial.println(b);
    Serial.println();
  }
  
  // IMU test
  if (IMU.accelerationAvailable()) {

    // Read Accelerometer
    IMU.readAcceleration(x, y, z);

    Serial.print("x");
    Serial.print('\t');
    Serial.print("y");
    Serial.print('\t');
    Serial.print("z");
    Serial.print('\n');
    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }

  // Temperature and Humidity sensor test
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

  // Pressure sensor test
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  // print an empty line
  Serial.println();

  Message = "temp>";
  Message += temperature;

  Message += ">hum>";
  Message += humidity;

  Message += ">pres>";
  Message += pressure;

  Message += ">imu>";
  Message += x;
  Message += ", ";
  Message += y;
  Message += ", ";
  Message += z;

  Message += ">col>";
  Message += r;
  Message += ", ";
  Message += g;
  Message += ", ";
  Message += b;
  Message += ">";
  Message += "*";
  Message += ">";

  Serial1.println(Message);
  Serial.print(Message);




  // wait 1 second to print again
  delay(3000);
}