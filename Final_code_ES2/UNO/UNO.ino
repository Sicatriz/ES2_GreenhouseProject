#include <stdlib.h>
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <SDISerial.h>

#define DATALINE_PIN 2
#define INVERTED 1

String VWC;   //volumetric water content
String EC;    // electrical conductivity
String TEMP;  // temperature
char X='+';
char *FTEvalue=new char[20];

String Message = "";
String temperature = "0";
String humidity = "0";
String pressure = "0";
String imu_x = "0", imu_y = "0", imu_z = "0";  // IMU
String color_r = "0", color_g = "0", color_b = "0";  // Color sensor
bool parseFlag = false;
int screenState = 0;

LiquidCrystal_I2C lcd(0x20, 16, 2); // I2C address 0x27, 16 column and 2 rows

SDISerial sdi_serial_connection(DATALINE_PIN, INVERTED);

char* get_measurement(){
	char* service_request = sdi_serial_connection.sdi_query("?M!",150);
	//you can use the time returned above to wait for the service_request_complete
	char* service_request_complete = sdi_serial_connection.wait_for_response(150);
	//dont worry about waiting too long it will return once it gets a response
	return sdi_serial_connection.sdi_query("?D0!",150);
}

void setup() {
  sdi_serial_connection.begin(); // start our SDI connection 
  Serial.begin(9600); // Initialize the Serial communication at 9600 baud rate
  
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("starting...");
  delay(3000);
}

void loop() {
  
  if (Serial.available()) { // Check if data is available to read
    char receivedChar = Serial.read(); // Read the incoming byte
    Message += receivedChar;
    // Check for the end of a message
    if (receivedChar == '*') {
      parseFlag = true;
    }
  }

  if (parseFlag == true) {
    
 
    char* response = get_measurement(); // get measurement data
    //lcd.clear();
    //lcd.print("measured 1");
    String FTE = response;
    
    int tmp1 = '+';
    int tmp2 = FTE.indexOf(tmp1);
    
    while (tmp2 == -1) {
      char* response = get_measurement(); // get measurement data
      //lcd.clear();
      //lcd.print("measured 2");
      FTE = response;
      tmp2 = FTE.indexOf(tmp1);
    }

    FTE.replace('+', ' '); // Replace '+' signs with spaces

    int FTElength = FTE.length();

    if (FTEvalue[5] == '+') {
      VWC = FTE.substring(2, 5);
      EC = FTE.substring(6, 10);
      TEMP = FTE.substring(11, 15);
    } else {
      VWC = FTE.substring(2, 6);
      EC = FTE.substring(7, 11);
      TEMP = FTE.substring(12, 17);
    }
    float VWCvalue = VWC.toInt() / 100.0;
    //lcd.clear();
    //lcd.print("got vwc");

    // Split the message by '*'
    int delimiterIndex = Message.indexOf('*');
    if (delimiterIndex != -1) {
      String data = Message.substring(0, delimiterIndex);
      // Reset message buffer for next iteration
      Message = Message.substring(delimiterIndex + 1);
      // Process the data
      int tempIndex = data.indexOf("temp>");
      int humIndex = data.indexOf("hum>");
      int presIndex = data.indexOf("pres>");
      int imuIndex = data.indexOf("imu>");
      int colIndex = data.indexOf("col>");
      if (tempIndex != -1) {
        temperature = data.substring(tempIndex + 5, humIndex - 1);
      }
      if (humIndex != -1) {
        humidity = data.substring(humIndex + 4, presIndex - 1);
      }
      if (presIndex != -1) {
        pressure = data.substring(presIndex + 5, imuIndex - 1);
      }
      if (imuIndex != -1) {
        int comma1 = data.indexOf(',', imuIndex);
        int comma2 = data.indexOf(',', comma1 + 1);
        imu_x = data.substring(imuIndex + 4, comma1);
        imu_y = data.substring(comma1 + 2, comma2);
        imu_z = data.substring(comma2 + 2, colIndex - 1);
      }
      if (colIndex != -1) {
        int comma1 = data.indexOf(',', colIndex);
        int comma2 = data.indexOf(',', comma1 + 1);
        color_r = data.substring(colIndex + 4, comma1);
        color_g = data.substring(comma1 + 2, comma2);
        color_b = data.substring(comma2 + 2);
      }
      /*
      // Print the parsed values
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity: ");
      Serial.println(humidity);
      Serial.print("Pressure: ");
      Serial.println(pressure);
      Serial.print("IMU - X: ");
      Serial.println(imu_x);
      Serial.print("IMU - Y: ");
      Serial.println(imu_y);
      Serial.print("IMU - Z: ");
      Serial.println(imu_z);
      Serial.print("Color - R: ");
      Serial.println(color_r);
      Serial.print("Color - G: ");
      Serial.println(color_g);
      Serial.print("Color - B: ");
      Serial.println(color_b);


      */


    }

    switch(screenState)
      {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Temperature:");
          lcd.setCursor(0, 1);
          lcd.print(temperature);
        break;
        case 1:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Humidity:");
          lcd.setCursor(0, 1);
          lcd.print(humidity);
        break;
        case 2:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Pressure:");
          lcd.setCursor(0, 1);
          lcd.print(pressure);
        break;
        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("IMU: x ");
          lcd.print(imu_x);

          lcd.setCursor(0, 1);
          lcd.print(" y ");
          lcd.print(imu_y);
          lcd.print(" z ");
          lcd.print(imu_z);

        break;
        case 4:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Color: r ");
          lcd.print(color_r);

          lcd.setCursor(0, 1);
          lcd.print(" g ");
          lcd.print(color_g);
          lcd.print(" b ");
          lcd.print(color_b);

        break;
        case 5:
          lcd.clear();                 // clear display
          lcd.setCursor(0, 0);         // move cursor to   (0, 0)
          lcd.print("VWC1 = ");        // print message at (0, 0)
          lcd.setCursor(7, 0);
          lcd.print(VWCvalue);
          lcd.setCursor(0, 1);
          lcd.print("EC = ");
          lcd.setCursor(5, 1);
          lcd.print(EC);
          lcd.setCursor(12, 0);
          lcd.print("Temp");
          lcd.setCursor(12, 1);
          lcd.print(TEMP);
        break;
       
        default:
          return 0;
        break;
      }
    delay(2000);
    screenState++;
    if(screenState == 6)
    {
      screenState = 0;
    }
    
    parseFlag = false; // Reset parseFlag
  }
}