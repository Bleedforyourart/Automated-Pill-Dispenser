#include <LiquidCrystal.h> // include LCD library code
#include <Wire.h> // include Wire library code (needed for I2C protocol devices)
#include <Stepper.h>

LiquidCrystal lcd(50, 51, 5, 4, 3, 2); // LCD module connections (RS, E, D4, D5, D6, D7)
Stepper beans(200,8,9,10,11);
 
void setup() {
  pinMode(45, INPUT_PULLUP);                      // Time (green)
  pinMode(44, INPUT_PULLUP);                      // changer (Red)
  pinMode(6, INPUT_PULLUP);                      // Advil (black)
  pinMode(7, INPUT_PULLUP);                      // Centrum (white)
  pinMode(23, OUTPUT);        //buzzer
  pinMode(25,OUTPUT);         //LED
  pinMode(24,OUTPUT);         // LED2
  Serial.begin(9600);
  lcd.begin(16, 2);
  Wire.begin();                                  // Join i2c bus
  beans.setSpeed(100);
}
 
char Time[]     = "TIME:  :  :  ";    // Array text for time
char AdvilTime[] = "TIME:  :  ";      // text array for Advil time 
char CetrumTime[] = "TIME:  :  ";     // text array for centrum time
char AdvilMenu[] = "Advil Menu";      
char CentrumMenu[] = "Centrum Menu";
char PillTimes[16];         // Text array to store the dispensed times
char PillDoses[16];         // text array to store dosage ammount
byte i, second, minute, hour, a, c;   //variables
byte aminute2,ahour2, aminute, ahour, adose, adose2; // More variables for advil
byte cminute,chour, cdose;      //more variables for centrum

void clock_display(){         //function to display the clock
  // Convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour >> 4)   * 10 + (hour & 0x0F);

  // End conversion
  Time[12]     = second % 10 + 48;
  Time[11]     = second / 10 + 48;
  Time[9]      = minute % 10 + 48;
  Time[8]      = minute / 10 + 48;
  Time[6]      = hour   % 10 + 48;
  Time[5]      = hour   / 10 + 48;
  lcd.setCursor(0, 0);
  lcd.print(Time);delay(1500);                    // Display time array
}

void PillDisplay(){       //function to display the pill dispense times and dosage
   
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("Dispense Time:");
  lcd.setCursor(0,1);
  sprintf(PillTimes,"A:%02u:%02u C:%02u:%02u",ahour,aminute,chour,cminute); //write valus of time ahour,aminute,chour,cminute into PillTimes array
  lcd.print(PillTimes);
  delay(1500);
  lcd.setCursor(0,1);
  sprintf(PillTimes,"A:%02u:%02u C:%02u:%02u",ahour2,aminute2,chour,cminute); //same as the other but over writes ahour,aminute for ahour2,aminute2
  lcd.print(PillTimes);
  delay(1500);
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("Dosage:");
  lcd.setCursor(0,1);
  sprintf(PillDoses,"A:%01u  A:%01u  C:%01u",adose,adose2,cdose); //write value of adoes,adose2, cdose into pilldoses array
  lcd.print(PillDoses);
  delay(1500);lcd.clear();
}

void blink_parameter(){
  byte j = 0;
  while(j < 10 && digitalRead(8) && digitalRead(9)){
    j++;
    delay(50);
  }
}

void blink_aparameter(){
  byte j = 0;
  while(j < 10 && digitalRead(6) && digitalRead(9)){
    j++;
    delay(50);
  }
}

void blink_cparameter(){
  byte j = 0;
  while(j < 10 && digitalRead(7) && digitalRead(9)){
    j++;
    delay(50);
  }
}

byte Clock(byte x, byte y, byte parameter){   //function to set real time
  char text[3]; //varable to store value 
  lcd.clear();
  lcd.print(Time);
  while(!digitalRead(45));                        // Wait until button (pin #8) released
    while(true){
      while(!digitalRead(44)){                      // If button is pressed
        parameter++;
        if(i == 0 && parameter > 23){parameter = 0;}    // If hours > 23 ==> hours = 0
          
        if(i == 1 && parameter > 59){parameter = 0;}    // If minutes > 59 ==> minutes = 0
          
        sprintf(text,"%02u", parameter);  //write value of parameter into "text" array
        lcd.setCursor(x, y);
        lcd.print(text);
        delay(200);                                // Wait 200ms
        }
      lcd.setCursor(x, y);
      lcd.print("  ");                             // Display two spaces
      blink_parameter();
      sprintf(text,"%02u", parameter);
      lcd.setCursor(x, y);
      lcd.print(text);
      blink_parameter();
      if(!digitalRead(45)){                         // If button (pin #8) is pressed
        i++;                                       // Increament 'i' for the next parameter
        return parameter;                          // Return parameter value and exit
      }
    }
}

byte Advil_Menu(byte x,byte y, byte aparameter){
   char text[3];
   if(a==0){lcd.print(AdvilMenu);}
   if(a==3){lcd.print("Second Dose:");}
   while(!digitalRead(6));
     while(true){
      //lcd.setCursor(0,0);
      while(!digitalRead(44)){                      // If button  is pressed
        aparameter++;
        if(a == 0 && aparameter > 23){aparameter = 0;}
        if(a == 1 && aparameter > 59){aparameter = 0;}
        if(a == 3 && aparameter > 23){aparameter = 0;}
        if(a == 4 && aparameter > 59){aparameter = 0;}
        sprintf(text,"%02u", aparameter);  //write the value of minute/hour into the text array
        lcd.setCursor(x, y);    //move to its corresponding position
        lcd.print(text);        // display the minute/hour
        delay(200);              
      }
      lcd.setCursor(0,1);
      lcd.print(AdvilTime);
      lcd.setCursor(x, y); 
      lcd.print("  ");                         
      blink_aparameter();
      sprintf(text,"%02u", aparameter);
      lcd.setCursor(x, y);
      lcd.print(text);
      blink_aparameter();
      if(!digitalRead(6)){                         // If button is pressed
        a++;                                       // Increament 'i' for the next parameter
        return aparameter;                          // Return parameter value and exit
      }
     }
}

byte Centrum_Menu(byte x,byte y, byte cparameter){
   char text[3];
   if(c==0){lcd.print(CentrumMenu);}
   while(!digitalRead(7));
     while(true){
      while(!digitalRead(44)){                      // If button is pressed
        cparameter++;
        if(c == 0 && cparameter > 23){cparameter = 0;}               // If hours > 23 ==> hours = 0
          
        if(c == 1 && cparameter > 59){cparameter = 0;}               // If minutes > 59 ==> minutes = 0

          
        sprintf(text,"%02u", cparameter);  //write the value of minute/hour into the text array
        lcd.setCursor(x, y);    //move to its corresponding position
        lcd.print(text);        // display the minute/hour
        delay(200);              //
      }
      lcd.setCursor(0,1);
      lcd.print(CetrumTime);
      lcd.setCursor(x, y); 
      lcd.print("  ");                         
      blink_cparameter();
      sprintf(text,"%02u", cparameter);
      lcd.setCursor(x, y);
      lcd.print(text);
      blink_cparameter();
      if(!digitalRead(7)){                         // If button is pressed
        c++;                                       // Increament 'i' for the next parameter
        return cparameter;                          // Return parameter value and exit
      }
     }
}

byte Advil_Dosage( byte adose){     //function to set the dosage amount for advil 1
  char dose[2];
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Advil Dose?:");
  while(!digitalRead(6));
  while(true){
    while(!digitalRead(44)){
      adose++;
      if(a == 2 && adose > 2){adose = 0;}
      sprintf(dose,"%02u", adose); 
      lcd.setCursor(14,0);    
      lcd.print(dose);        
      delay(200);  
    }
    lcd.setCursor(14,0); 
    lcd.print("  ");                         
    blink_aparameter();
    sprintf(dose,"%02u", adose);
    lcd.setCursor(14,0);
    lcd.print(dose);
    blink_aparameter();
    if(!digitalRead(6)){                         // If button (pin #6) is pressed
       a++;
       lcd.clear();
       return adose;                          // Return parameter value and exit
     }
  }
}

byte Advil_Dosage_Two( byte adose2){        // function to set advil dosage 2
  char dose[2];
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Advil Dose?:");
  while(!digitalRead(6));
  while(true){
    while(!digitalRead(44)){
      adose2++;
      if(a == 5 && adose2 > 2){adose2 = 0;}
      sprintf(dose,"%02u", adose2); 
      lcd.setCursor(14,0);    
      lcd.print(dose);        
      delay(200);  
    }
    lcd.setCursor(14,0); 
    lcd.print("  ");                         
    blink_aparameter();
    sprintf(dose,"%02u", adose2);
    lcd.setCursor(14,0);
    lcd.print(dose);
    blink_aparameter();
    if(!digitalRead(6)){                         // If button is pressed
       a++;
       lcd.clear();
       return adose2;                          // Return parameter value and exit
     }
  }
}

byte Centrum_Dosage( byte cdose){       //function to set dosage for centrum 
  char dose[2];
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Centrum Dose?:");
  while(!digitalRead(7));
  while(true){
    while(!digitalRead(44)){
      cdose++;
      if(c == 2 && cdose > 1){cdose = 0;}
      sprintf(dose,"%02u", cdose); 
      lcd.setCursor(14,0);    
      lcd.print(dose);        
      delay(200);  
    }
    lcd.setCursor(14,0); 
    lcd.print("  ");                         
    blink_aparameter();
    sprintf(dose,"%02u", cdose);
    lcd.setCursor(14,0);
    lcd.print(dose);
    blink_cparameter();
    if(!digitalRead(7)){                         // If button is pressed
       c++;
       lcd.clear();
       return cdose;                          // Return parameter value and exit
     }
  }
}

void dispense_Advil(int ahour, int aminute, int adose){   //function to dispense first advil amount. takes ahour,aminute and adose as parameter
  if (hour == ahour && minute == aminute){  //if RTC time equals the dispense time (minute and hour) then:
    tone(23,200);
    digitalWrite(25,HIGH);  //led
    digitalWrite(24,HIGH);  //led
    for(int i =0; i <adose ; i++){  //loop to dispense the numeber of doses selected
      digitalWrite(24,HIGH);
      digitalWrite(25,LOW);
      lcd.clear(); lcd.setCursor(0,0);
      lcd.print("Dispensing!");
      beans.step(-512);           //steps to dispense 
      digitalWrite(24,LOW);
      digitalWrite(25,HIGH);
      delay(500);
      beans.step(512);          //steps to bring back
      digitalWrite(24,HIGH);
      digitalWrite(25,LOW);
      delay(500);
      digitalWrite(24,LOW);
      digitalWrite(25,HIGH);
    }
    lcd.clear(); lcd.setCursor(0,0);
    lcd.print("Advil"); lcd.setCursor(0,1);
    lcd.print("Dispensed!");      // when done display dispensed
    digitalWrite(25,HIGH);        // keep leds on 
    digitalWrite(24,HIGH);        // keep leds on 
    delay(59500);                 // wait for about a minute and continue with code
    noTone(23);           
    digitalWrite(25,LOW);
    digitalWrite(24,LOW);
  }
}

void dispense_Advil_Two(int ahour2, int aminute2,int adose2){   //same as previous. parameters are changed to compare the second dose times
  if (hour == ahour2 && minute == aminute2){
    tone(23,200);
    digitalWrite(25,HIGH);
    digitalWrite(24,HIGH);
    for(int i =0; i <adose2 ; i++){
      digitalWrite(24,HIGH);
      digitalWrite(25,LOW);
      lcd.clear(); lcd.setCursor(0,0);
      lcd.print("Dispensing!");
      beans.step(-512);
      digitalWrite(24,LOW);
      digitalWrite(25,HIGH);
      delay(500);
      beans.step(512);
      digitalWrite(24,HIGH);
      digitalWrite(25,LOW);
      delay(500);
      digitalWrite(24,LOW);
      digitalWrite(25,HIGH);
    }
    lcd.clear(); lcd.setCursor(0,0);
    lcd.print("Advil"); lcd.setCursor(0,1);
    lcd.print("Dispensed!");
    digitalWrite(25,HIGH);
    digitalWrite(24,HIGH);
    delay(59500);
    noTone(23);
    digitalWrite(25,LOW);
    digitalWrite(24,LOW);
  }
}

void dispense_Centrum(int chour, int cminute, int cdose){ //same as previous but do it for centrum
  if (hour == chour && minute == cminute){
    tone(23,200);
    digitalWrite(25,HIGH);
    for(int i =0; i <cdose ; i++){
      digitalWrite(24,HIGH);
      digitalWrite(25,LOW);
      lcd.clear(); lcd.setCursor(0,0);
      lcd.print("Dispensing!");
      beans.step(512);
      digitalWrite(24,LOW);
      digitalWrite(25,HIGH);
      delay(500);
      beans.step(-512);
      digitalWrite(24,HIGH);
      digitalWrite(25,LOW);
      delay(500);
      digitalWrite(24,LOW);
      digitalWrite(25,HIGH);
    }
    lcd.clear(); lcd.setCursor(0,0);
    lcd.print("Centrum"); lcd.setCursor(0,1);
    lcd.print("Dispensed!");
    digitalWrite(25,HIGH);
    digitalWrite(24,HIGH);
    delay(59500);
    noTone(23);
    digitalWrite(25,LOW);
    digitalWrite(24,LOW);
  }
}

void loop() {
  if(!digitalRead(45)){                           // If buttonis pressed
      i = 0;
      hour   = Clock(5, 0, hour);     // set the RTC hour
      minute = Clock(8, 0, minute);   // set the RTC minute
      
      // Convert decimal to BCD
      minute = ((minute / 10) << 4) + (minute % 10);
      hour = ((hour / 10) << 4) + (hour % 10);
      // End conversion
      // Write data to DS1307 RTC
      Wire.beginTransmission(0x68);               // Start I2C protocol with DS1307 address
      Wire.write(0);                              // Send register address
      Wire.write(0);                              // Reset sesonds and start oscillator
      Wire.write(minute);                         // Write minute
      Wire.write(hour);                           // Write hour
      Wire.write(1);                              // Write day (not used)
      Wire.endTransmission();                     // Stop transmission and release the I2C bus
      delay(200);                                 // Wait 200ms
    }
    
    if(!digitalRead(6)){  //if button is pressed
      lcd.clear();
      a=0;
      ahour = Advil_Menu(5,1,ahour);      // set dispense hour (1)
      aminute = Advil_Menu(8,1,aminute);  // set dispense minute (1)
      adose = Advil_Dosage(adose);        // set dosage1
      ahour2 = Advil_Menu(5,1,ahour2);    // set dispense hour (2)
      aminute2 = Advil_Menu(8,1,aminute2);   // set dispense minute (2)
      adose2 = Advil_Dosage_Two(adose2);    //set dosage2
      delay(200);
     }

    if(!digitalRead(7)){        //same as previous. just change variable names
      lcd.clear();
      c=0;
      chour = Centrum_Menu(5,1,chour);
      cminute = Centrum_Menu(8,1,cminute);
      cdose = Centrum_Dosage(cdose);
      delay(200);
      }

    Wire.beginTransmission(0x68);                 // Start I2C protocol with DS1307 address
    Wire.write(0);                                // Send register address
    Wire.endTransmission(false);                  // I2C restart
    Wire.requestFrom(0x68, 7);                    // Request 7 bytes from DS1307 and release I2C bus at end of reading
    second = Wire.read();                         // Read seconds from register 0
    minute = Wire.read();                         // Read minuts from register 1
    hour   = Wire.read();                         // Read hour from register 2
    Wire.read();                                        //next 3 functions are to check RTC and dispense times
    dispense_Advil(ahour,aminute,adose);delay(5);         
    dispense_Advil_Two(ahour2,aminute2,adose2);delay(5);
    dispense_Centrum(chour,cminute,cdose);delay(5); 

    //repeat it so RTC variable parameters are updated and it can be accurate (had to test it couple times to get it right)
    Wire.beginTransmission(0x68);                 
    Wire.write(0);                                
    Wire.endTransmission(false);                  
    Wire.requestFrom(0x68, 7);
    second = Wire.read();                         
    minute = Wire.read();                         
    hour   = Wire.read();                         
    Wire.read();                                  
    dispense_Advil(ahour,aminute,adose);delay(5);
    dispense_Advil_Two(ahour2,aminute2,adose2);delay(5);
    dispense_Centrum(chour,cminute,cdose);delay(5);
    
    Wire.beginTransmission(0x68);                 
    Wire.write(0);                                
    Wire.endTransmission(false);                  
    Wire.requestFrom(0x68, 7);
    second = Wire.read();                         
    minute = Wire.read();                         
    hour   = Wire.read();                         
    Wire.read();                                  
    clock_display();delay(5);                             // Display time 
    dispense_Advil(ahour,aminute,adose);delay(5);
    dispense_Advil_Two(ahour2,aminute2,adose2);delay(5);
    dispense_Centrum(chour,cminute,cdose);delay(5);

    Wire.beginTransmission(0x68);                 
    Wire.write(0);                                
    Wire.endTransmission(false);                  
    Wire.requestFrom(0x68, 7);
    second = Wire.read();                         
    minute = Wire.read();                         
    hour   = Wire.read();                         
    Wire.read();                                  
    dispense_Advil(ahour,aminute,adose);delay(5);
    dispense_Advil_Two(ahour2,aminute2,adose2);delay(5);
    dispense_Centrum(chour,cminute,cdose);delay(5);

    Wire.beginTransmission(0x68);                 
    Wire.write(0);                                
    Wire.endTransmission(false);                  
    Wire.requestFrom(0x68, 7);
    second = Wire.read();                         
    minute = Wire.read();                         
    hour   = Wire.read();                         
    Wire.read();                                  
    PillDisplay();delay(5);         //display the pill dispense times and dosage
    dispense_Advil(ahour,aminute,adose);delay(5);
    dispense_Advil_Two(ahour2,aminute2,adose2);delay(5);
    dispense_Centrum(chour,cminute,cdose);delay(5);

    Wire.beginTransmission(0x68);                 
    Wire.write(0);                                
    Wire.endTransmission(false);                  
    Wire.requestFrom(0x68, 7);
    second = Wire.read();                         
    minute = Wire.read();                         
    hour   = Wire.read();                         
    Wire.read();                                  
    dispense_Advil(ahour,aminute,adose);delay(5);
    dispense_Advil_Two(ahour2,aminute2,adose2);delay(5);
    dispense_Centrum(chour,cminute,cdose);delay(5);
}
