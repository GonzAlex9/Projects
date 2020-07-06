#include <IRremote.h>
#include <Servo.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(12,10, 4, 5, 6, 7); //crea objeto LC (RS,Enable,d4,d5,d6,d7)
int IRpin = 11;  // pin for the IR sensor
const int pinVermell = 8;
const int pinVerd = 3;
const int pulsador = 2;
IRrecv irrecv(IRpin);
decode_results results;
Servo myservo;

//
int cont=0;
int vect[4];
int contra[4] = {16724175, 16718055, 16743045, 16716015};

int num=10; //segundos para cierre puerta

int estaon=HIGH;

void setup()
{
 lcd.begin(16, 2); //dimensiones pantalla
 Serial.begin(9600);
 irrecv.enableIRIn(); // Start the receiver
 myservo.attach(9);  // attaches the servo on pin 9 to the servo object
 myservo.write(0);
 pinMode(pinVermell , OUTPUT);  //definir pin como salida
 pinMode(pinVerd , OUTPUT);  //definir pin como salida
 pinMode(pulsador,INPUT);

}
void loop() 
{
  
  while(digitalRead(pulsador)==LOW){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pulsa ON/OFF");
    lcd.setCursor(0,1);
    lcd.print("PUERTA CERRADA");
    
    // Turn off the display:
    lcd.noDisplay();
    delay(500);
    // Turn on the display:
    lcd.display();
    delay(1000);
    
      
    if (irrecv.decode(&results)) 
     {
       irrecv.resume();   // Receive the next value
     }
    if (results.value == 16753245)  // change according to your IR remote button number
      {
        executar();
        delay(2000);
      }
  }
  delay(500);
  myservo.write(180);
  digitalWrite(pinVerd , HIGH);
  digitalWrite(pinVermell,LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Puerta abierta");
  lcd.setCursor(0,1);
  lcd.print("des del interior");
  delay(5000);
  while(num>=0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Se cierra en..");
          if (num==0){
            myservo.write(0);
            digitalWrite(pinVerd , LOW);
            digitalWrite(pinVermell , HIGH);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" PUERTA CERRADA");
          }else{
            lcd.setCursor(0,1);
            lcd.print("        ");
            lcd.print(num);
          }
          delay(1000);
          num--;
  }
  num=10;
}


void executar(){
  lcd.clear();
  lcd.print("PIN acceso:");
  delay(500);
  
  Serial.println("INTRODUCE CONTRASEÑA PARA ACCEDER: ");
  digitalWrite(pinVermell , HIGH);
  while(cont<4){
    if (irrecv.decode(&results)) {
      vect[cont]=results.value;
      Serial.println("VALOR:");
      Serial.println(results.value, DEC);
      lcd.setCursor(cont,1);
      lcd.print(numero(results.value));
      
      cont++;
      irrecv.resume();   // Receive the next value
      delay(15);
    }
    
  }

  for (int i=0; i<4;++i){
    if (vect[i]==contra[i]){
      delay(15);
      if (i==3){
        myservo.write(180);
        lcd.clear();
        lcd.print("CORRECTO!!");
        lcd.setCursor(0,1);
        lcd.print("Adelante..");
        Serial.println("PIN CORRECTO!!");
        Serial.println("Puerta abierta.");
        digitalWrite(pinVermell , LOW);
        delay(20);
        digitalWrite(pinVerd , HIGH); 
        delay(3000);
        Serial.println("En 10 segons es tanca la porta...");
        
        while(num>=0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Se cierra en..");
          if (num==0){
            myservo.write(0);
            digitalWrite(pinVerd , LOW);
            digitalWrite(pinVermell , HIGH);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" PUERTA CERRADA");
          }else{
            lcd.setCursor(0,1);
            lcd.print("        ");
            lcd.print(num);
          }
          delay(1000);
          num--;
        }
        num=10;
        //delay(10000);
        
        break;
      }
    }
    else{
      lcd.clear();
      lcd.print("INCORRECTE!!");
      Serial.println("Contrassenya INCORRECTE!!");
      delay(5000);
      break;
    }
  }
  cont=0;
  memset(vect, 0, sizeof(vect));
}

int numero(int valor){
  switch(valor){
    case 16728455:
      return 0;
      break;
    case 16724175:
      return 1;
      break;
    case 16718055:
      return 2;
      break;
    case 16743045:
      return 3;
      break;
    case 16716015:
      return 4;
      break;
    case 16726215:
      return 5;
      break;
    case 16734885:
      return 6;
      break;
    case 16728765:
      return 7;
      break;
    case 16730805:
      return 8;
      break;
    case 16732845:
      return 9;
      break;
  }
}
