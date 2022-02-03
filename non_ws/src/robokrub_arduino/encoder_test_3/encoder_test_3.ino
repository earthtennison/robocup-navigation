 #include <Servo.h>
 
 int val; 
 int encoder0PinA = 2;
 int encoder0PinB = 3;
 int Zsignal = 4;
 int encoder0Pos = 0;
 int encoder0PinALast = LOW;
 int n = LOW; 
 int time_H;
 int time_L;
 float t_period;
 float Frequency; 

 Servo mobileBase1, mobileBase2;

 void setup() {   
   pinMode (encoder0PinA,INPUT_PULLUP);
   pinMode (encoder0PinB,INPUT_PULLUP);
   pinMode (Zsignal,INPUT_PULLUP);
   Serial.begin (115200);
   mobileBase1.attach(5);
   mobileBase2.attach(8);
   mobileBase1.write(95);
   mobileBase2.write(95);
 } 

 void loop() {    
   n = digitalRead(encoder0PinA);
   Serial.println(n);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {    // Signal A and B Direction  
     if (digitalRead(encoder0PinB) == LOW) {
       encoder0Pos--;
       Serial.print (" CW, ");       //  Direction CW
     } else {
       encoder0Pos++;       
       Serial.print (" CCW, ");    //  Direction CCW       
     }
     
    //--------- Frequency Monitor ------------------   

     time_H = pulseIn(encoder0PinA,HIGH);
     time_L = pulseIn(encoder0PinA,LOW);     
     t_period = time_H+time_L;
     t_period = t_period/1000;
     Frequency = 1000/t_period;
     Serial.print(" F = ");
     Serial.print(Frequency/2000*60); 
     Serial.println(" Hz ");          
 
   } 
   encoder0PinALast = n; 
 } 
