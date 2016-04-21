/* TIEFZUG by: Axel Lodyga
Last edit: 21.04.2016

*/

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol

/*-----( Declare Variables )-----*/
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
int max_saug_time = 1; // in Minuten
int max_heat_time = 3; //in Minuten
int SSR_1_out=A0;
int SSR_2_out=A1;
int Sauger_out=A2;
int endstop=12; //TASTER für oben
int SSR_1_in=11;
int SSR_2_in=10;
int Sauger_in=9;
int  tasterstatus_Sauger=0;
int  tasterstatus_endstop=0;
int  tasterstatus_switch1=0;
int  tasterstatus_switch2=0;
int stage = 1;
int state = 0;
int was_state =0;
int tasterstatus=0;
unsigned long start_heat = 0;
unsigned long start_saug = 0;
int startet_1 = 0;
int startet_2 = 0;
unsigned long saugzeit = 0;
unsigned long heizzeit = 0;
bool is = false;
void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  max_heat_time = max_heat_time*60*1000;
  max_saug_time = max_saug_time*60*1000;
  Serial.begin(9600);  // Used to type in characters

  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight


pinMode(SSR_1_out, OUTPUT);
pinMode(SSR_2_out, OUTPUT);
pinMode(SSR_1_in, INPUT);
pinMode(SSR_2_in, INPUT);
pinMode(Sauger_out, OUTPUT);
pinMode(Sauger_in, INPUT);
pinMode(endstop, INPUT);
// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  

//-------- Write characters on the display ------------------
// NOTE: Cursor Position: (CHAR, LINE) start at 0  
  lcd.setCursor(2,0); //Start at character 4 on line 0
  lcd.print("TIEFZUUUUUG");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("HI!attraktor.org");
  delay(3000);  

// Wait and then tell user they can start the Serial Monitor and type in characters to
// Display. (Set Serial Monitor option to "No Line Ending")
  lcd.clear();
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Initalisierung");
  lcd.setCursor(0,1);
  lcd.print("beendet");  
  lcd.clear();


}/*--(end setup )---*/
void counter_heat(){
      digitalWrite(Sauger_out, LOW);
      if (startet_1 == 0){
      start_heat = millis();
      startet_1 = 1;
      }
      lcd.setCursor(0,0);      
      lcd.print("HEIZEN!");
      lcd.setCursor(0,1);
      lcd.print(String((millis()-start_heat)/1000) +" sec");
      check_taster_while_heat();
      switch (stage){
        case 0:
        digitalWrite(SSR_1_out, LOW);
        digitalWrite(SSR_2_out, LOW);
        break;  
      
      case 1:
        digitalWrite(SSR_1_out, HIGH);
        break;  
      
      case 2:
       digitalWrite(SSR_2_out, HIGH);
        break;
      
      case 3:
       digitalWrite(SSR_1_out, HIGH);
       digitalWrite(SSR_2_out, HIGH);
      break;
      
      }
}
void counter_sauger(){
      digitalWrite(SSR_1_out, LOW);
       digitalWrite(SSR_2_out, LOW);
      if (startet_2 == 0){
      start_saug = millis();
      startet_2 = 1;
      }
      lcd.setCursor(0,0);      
      lcd.print("SAUGEN!");
      lcd.setCursor(0,1);
      lcd.print(String((millis()-start_saug)/1000) +" sec");
      digitalWrite(Sauger_out, HIGH);     
}
void check_taster_while_heat(){
  if (tasterstatus_switch1 == 0 && tasterstatus_switch2 == 0){
  
  stage = 0;
  }
  if (tasterstatus_switch1 == 1 && tasterstatus_switch2 == 1){
  stage = 3;
  }
  else if (tasterstatus_switch1 == 0 && tasterstatus_switch2 == 1){
  stage = 1;
  }
  else if (tasterstatus_switch1 == 1 && tasterstatus_switch2 == 0){
  stage =2;
  }
}

void menu(){
  

  lcd.setCursor(0,0);
  lcd.print("  --- MENU ---  ");
  if (tasterstatus_switch1 == 0 && tasterstatus_switch2 == 0){
  //delay(200);
  lcd.setCursor(0,1);
  lcd.print("NO  POWER AT ALL");
  stage = 0;
  }
  if (tasterstatus_switch1 == 1 && tasterstatus_switch2 == 1){
  //delay(200);
  lcd.setCursor(0,1);
  lcd.print("  FULL  POWER   ");
  stage = 3;
  }
  else if (tasterstatus_switch1 == 0 && tasterstatus_switch2 == 1){
  //delay(200);
  lcd.setCursor(0,1);
  lcd.print("    2  Heizer   ");
  stage = 1;
  }
  else if (tasterstatus_switch1 == 1 && tasterstatus_switch2 == 0){
  //delay(200);
  lcd.setCursor(0,1);
  lcd.print("    4  Heizer   ");
  stage =2;
  }
}
void ergebnisse(){
  lcd.setCursor(0,0);
  lcd.print("SAUGEN    HEIZEN");
  lcd.setCursor(0,1);
  lcd.print(String(saugzeit/1000)+"sec");
  lcd.setCursor(10,1);
  lcd.print(String(heizzeit/1000)+"sec");
  delay(15000);
  startet_1 = 0;
  startet_2 = 0;
  start_heat = 0;
  start_saug = 0;
  Serial.println("all resetet");
  }

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/

{
  tasterstatus_endstop=digitalRead(endstop);
  tasterstatus_switch1=digitalRead(SSR_1_in);
  tasterstatus_switch2=digitalRead(SSR_2_in);
  tasterstatus_Sauger=digitalRead(Sauger_in);
  

//  debug()
 
  if (tasterstatus_endstop == 1){
    counter_heat();
    state = 1;  
   }
    if (tasterstatus_Sauger == 1  && tasterstatus_endstop == 0|| (millis()-start_saug) == max_saug_time && tasterstatus_endstop == 0 ){
    saugzeit = millis()-start_saug;
    digitalWrite(Sauger_out, LOW);
    startet_2 = 0;
    state= 2;
    ergebnisse();
    }
   if (startet_1 == 1 && tasterstatus_endstop == 0 || (millis()-start_heat) == max_heat_time){
      heizzeit = millis()-start_heat;
      counter_sauger();
      state = 3;
     }
  
   if (startet_2 != 1 && tasterstatus_endstop == 0){
    menu();
    state =4;
   }

     if (state != was_state){
      
  lcd.clear();
  was_state = state;
  }
   
    
   
}/* --(end main loop )-- */


/* ( THE END ) */

