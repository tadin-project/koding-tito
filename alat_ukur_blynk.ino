// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance
////////////////////
//freq/
/* https://www.nyebarilmu.com */
/////////////////////////////
//Freq


const int input_sensor = A1; // nama lain dari A0 yaitu input_sensor
float V2f = 0;
float Vsquare = 0;
float V = 0;
float x = 0;
float Vrmss = 0;
float N = 10000;
float f = 0;
float fs = 0;
float freq = 0;
//////////////////////////////////////// 
//Resistor
float Vrb,Vra,Ra=10,Rb,Ram,Vt,Vx;
float r10,r100,r1k,r10k,r100k,r1m;
////////////////////////////////////////
//Inductor
float Vti,Vtxi,bacai,baca2i,totalInd,ind;
////////////////////////////////////////
//Capacitor
float Vtc,Vtxc,bacac,baca2c,totalc,farad;
////////////////////////////////////////
float voltblynk;
float arusblynk;
float resblynk;
float indblynk;
float faradblynk;
float pfblynk;
float freqblynk;
//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
////////////////////////////////////////


void setup()
{  
  Serial.begin(9600);
  Serial3.begin(9600);
  emon1.voltage(A0, 235, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(A1,4);       // Current: input pin, calibration.
  //////////////////
  //LCD
  lcd.begin();
  /////////////////

  //////////////////////
  //Resistor
  pinMode(A7,INPUT_PULLUP);
  pinMode(A8,INPUT_PULLUP);
  pinMode(A9,INPUT_PULLUP);
  pinMode(A10,INPUT_PULLUP);
  pinMode(A11,INPUT_PULLUP);
  pinMode(A12,INPUT_PULLUP);
  //////////////////////
  //Rotary
  pinMode(22,INPUT_PULLUP);
  pinMode(24,INPUT_PULLUP);
  pinMode(26,INPUT_PULLUP);
  pinMode(28,INPUT_PULLUP);
  pinMode(30,INPUT_PULLUP);
  pinMode(32,INPUT_PULLUP);
  pinMode(34,INPUT_PULLUP);
  //////////////////////
}

void loop()
{
  skip:
  //emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  //Serial.println(String(supplyVoltage)+";"+String(Irms)+";"+String(freq)+";"+String(powerFActor)+";"+String(Rb)+";"+String(farad)+";"+String(ind)+";");
   
  emon1.calcVI(2000,1000);         // Calculate all. No.of half wavelengths (crossings), time-out
  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;             //extract Irms into Variable

  int Dpf = digitalRead(22);
  int Dv = digitalRead(24);
  int Da = digitalRead(26);
  int Df = digitalRead(28);
  int Dr = digitalRead(30);
  int Dl = digitalRead(32);
  int Dc = digitalRead(34);
  Serial.println(String(Dv)+";"+String(Da)+";"+String(Df)+";"+String(Dpf)+";"+String(Dr)+";"+String(Dc)+";"+String(Dl)+";");
  

// freq
  if (Df== 0 && Dpf==1 && Dv==1 && Da==1 && Dr==1 && Dl==1 && Dc==1)
  {
    for(int i=0;i<N;i++)
    {
      x = (analogRead(input_sensor)-510);  
      V = ((0.00000159*x*x*x)-(0.000329*x*x)+(1.029*x-1.230));
      V2f = V*V;
      Vsquare = Vsquare + V2f;

      if (V>0&&f==0)
      {
        f++; 
      }
      else if (V<0&&f==1)
      {
        fs++;
        f=0;  
      }
      delay(0.1);
    }
  float freq = fs/2; 
 // float freq = V; 
  Serial.println("frequensi");
  Serial.println(freq);
  Vsquare = 0;
  fs = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("frequensi");
  lcd.setCursor(0,1);
  lcd.print(String(freq) +" Hz");
  freqblynk = freq;
  goto skip;
  }
////////////////////////////

// Voltage
 if (Df== 1 && Dpf==1 && Dv==0 && Da==1 && Dr==1 && Dl==1 && Dc==1)
 {
  if (supplyVoltage < 1.5
  )
  {
    supplyVoltage = 0;
  }
    Serial.println("Voltage");
  Serial.println(supplyVoltage);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Voltage");
  lcd.setCursor(0,1);
  lcd.print(String(supplyVoltage) +" V");
  voltblynk = supplyVoltage;
  goto skip;
 }
//////////////

// Current
 if (Df== 1 && Dpf==1 && Dv==1 && Da==0 && Dr==1 && Dl==1 && Dc==1)
 {
  Serial.println("Arus");
  Serial.println(Irms);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Arus");
  lcd.setCursor(0,1);
  lcd.print(String(Irms)+" A");
  arusblynk = Irms;
  goto skip;
 }
//////////////

// Resistor
 if (Df== 1 && Dpf==1 && Dv==1 && Da==1 && Dr==0 && Dl==1 && Dc==1)
 {
  r10 = analogRead(A12);
  r100 = analogRead(A11);
  r1k = analogRead(A10);
  r10k = analogRead(A9);
  r100k = analogRead(A8);
  r1m = analogRead(A7);
  Vt = analogRead(A2);
  Vt = Vt / 1023.00;
  Vt = Vt * 4.8;

  r10 = r10 / 1023.00;
  r10 = r10 * 4.8;
  
  r100 = r100 / 1023.00;
  r100 = r100 * 4.8;
  
  r1k = r1k / 1023.00;
  r1k = r1k * 4.8;
  
  r10k = r10k / 1023.00;
  r10k = r10k * 4.8;
  
  r100k = r100k /1023.00;
  r100k = r100k * 4.8;
  
  r1m = r1m / 1023.00;
  r1m = r1m * 4.8;


  if (r10 < 3.3)
  {
    Ra = 10.00;
    Vx = 4.95;
  }
  else if (r10 > 3.3)
  {
    r10 = 0.00;
    
  }

  if (r100 < 3.3 && r100 > 0.8)
  {
    Ra = 100.00;
    Vx = 4.95;
  }
  else if (r100 < 0.8 || r100 >3.3)
  {
    r100 = 0.00;
  }

  if (r1k < 3.3 && r1k > 0.8)
  {
    Ra = 1000.00;
    Vx = 4.8;
  }
  else if (r1k < 0.8 || r1k > 3.3)
  {
    r1k = 0.00;
  }

  if (r10k < 3.3 && r10k > 0.8)
  {
    Ra = 10000.00;
    Vx = 4.8;
  }
  else if (r10k < 0.8 || r10k > 3.3)
  {
    r10k = 0.00;
  }

  if (r100k < 4.3 && r100k > 1.4)
  {
    Ra = 100000.00;
     Vx = 4.8;
  }
  else if (r100k < 1.4 || r100k > 4.3)
  {
    r100k = 0.00;
  }
  if (r1m < 3.3 && r1m > 0.33)
  {
    Ra = 1000000.00;
     Vx = 4.8;
  }
  else if (r1m < 0.33 || r1m > 3.3)
  {
    r1m = 0.00;
  }

  if (r100 > 0)
  {
   Vrb = r10 + r100 + r1k + r10k + r100k;
   Vra = Vx - Vrb;
   Rb = Vrb / Vra;
   Rb = Rb * Ra;  
   //Rb = Rb * 2.8627;
  }

  if (r1k > 0)
  {
   Vrb = r10 + r100 + r1k + r10k + r100k;
   Vra = Vx - Vrb;
   Rb = Vrb / Vra;
   Rb = Rb * Ra;  
   //Rb = Rb * 3.5587;
  }

  if (r10k > 0)
  {
   Vrb = r10 + r100 + r1k + r10k + r100k;
   Vra = Vx - Vrb;
   Rb = Vrb / Vra;
   Rb = Rb * Ra;  
   Rb = Rb * 0.755;
  }

  if (r100k > 0)
  {
   Vrb = r10 + r100 + r1k + r10k + r100k;
   Vra = Vx - Vrb;
   Rb = Vrb / Vra;
   Rb = Rb * Ra;  
   Rb = Rb * 0.25125;
  }

  if (r100k > 0)
  {
   Vrb = r10 + r100 + r1k + r10k + r100k;
   Vra = Vx - Vrb;
   Rb = Vrb / Vra;
   Rb = Rb * Ra;  
   
   Rb = Rb * 0.263157;
  }
  
  Vrb = r10 + r100 + r1k + r10k + r100k;    
  if (Vrb <= 0)
  {
   Rb = 0;
   Ra = 10;
  }  
    
  Serial.println("Resistor");
  Serial.println(String(Rb));
  Serial.println(String(Ra)+";"+String(Vrb)+";");
  Serial.println(String(r10)+";"+String(r100)+";"+String(r1k)+";"+String(r10k)+";"+String(r100k)+";"+String(r1m)+";");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Resistor");
  lcd.setCursor(0,1);
  lcd.print(String(Rb) +" Ohm");
  resblynk = Rb;
  delay(1000);
  goto skip;
 }
//////////////////////////////////////

// Inductor
 if (Df== 1 && Dpf==1 && Dv==1 && Da==1 && Dr==1 && Dl==0 && Dc==1)
 {
  analogWrite(11,32);
  baca2i = 0;
  for (int ni = 0; ni <= 20000; ni++)
  {
    if (analogRead(A6)>0)
    {
      Vti = analogRead(A6);
      Vtxi = Vtxi + Vti; 
    }
 
  }
 
  totalInd = (((Vtxi/20000)/1023)*5);
  //total = Vtx * 0.0000488758;

  if (totalInd < 0.44 && totalInd > 0)
  {
    ind = ((totalInd*12857.14)-757.14);
    ind = ind*0.9;
  }

  if (totalInd < 0.575 && totalInd > 0.44)
  {
    ind = ((totalInd*17647.05)-2147.05);
    ind = ind*0.9;
  }

  if (totalInd < 0.73 && totalInd > 0.575)
  {
    ind = ((totalInd*11612.9)+1522.58);
    ind = ind*0.9;
  }

  if (totalInd < 0.9155 && totalInd > 0.73)
  {
    ind = ((totalInd*34501.34)-15185.98);
    ind = ind*0.9;
  }

  if (totalInd < 1.5 && totalInd > 0.9155)
  {
    ind = ((totalInd*20281.69)-2167.887);
    ind = ind*0.9;
  }
  Serial.println("Inductor");
  Serial.println(ind);
  Serial.println(totalInd);
  Vtxi = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Inductor");
  lcd.setCursor(0,1);
  lcd.print(String(ind) +" mH");
  indblynk = ind;
  goto skip;
 }
//////////////

// Capasitor
 if (Df== 1 && Dpf==1 && Dv==1 && Da==1 && Dr==1 && Dl==1 && Dc==0)
 {
  analogWrite(11,32);
  baca2c = 0;

  for (int nc = 0; nc <= 10000; nc++)
  {
    if (analogRead(A3)>0)
    {
      Vtc = analogRead(A3);
      Vtxc = Vtxc + Vtc; 
    }
 
  }

  
  totalc = (((Vtxc/10000)/1023)*5);

  if (totalc < 5 && totalc > 2.45)
  {
    farad = ((totalc*-4.86)+21.82);
  }

  if (totalc < 2.45 && totalc > 1.65)
  {
    farad = ((totalc*-12.5)+40.375);
  }

  if (totalc < 1.65 && totalc > 1.25)
  {
    farad = ((totalc*-26.31)+62.89);
  }

  if (totalc < 1.25 && totalc > 0.6)
  {
    farad = ((totalc*-94.6)+148.24);
    farad = farad/10;
  }

  if (totalc < 0.6 && totalc > 0.5)
  {
    farad = ((totalc*169.491)+20.559);
    farad = farad/10;
  }

  if (totalc < 0.5 && totalc > 0.20)
  {
    farad = ((totalc*-745.712)+480.313);
    farad = farad/2.8;
  }

  if (totalc < 0.20 && totalc > 0)
  {
    farad = 0;
  }

  
  Serial.println("Capacitor");
  Serial.println(farad);
  Serial.println(totalc);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Capacitor");
  lcd.setCursor(0,1);
  lcd.print(String(farad) +" uF");
  faradblynk = farad;
  Vtxc = 0;
  goto skip;

 }
//////////////
// Cosp
 if (Df== 1 && Dpf==0 && Dv==0 && Da==1 && Dr==1 && Dl==1 && Dc==1)
 {
  Serial.println("Power Factor");
  Serial.println(String(powerFActor,4));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Power Factor");
  lcd.setCursor(0,1);
  lcd.print(String(powerFActor,4));
  pfblynk = powerFActor;
  goto skip;
 }
  Serial3.println(String(freqblynk)+";"+String(indblynk)+";"+String(faradblynk)+";"+String(voltblynk)+";"+String(arusblynk)+";"+String(pfblynk)+";"+String(resblynk)+";");
//////////////

}
