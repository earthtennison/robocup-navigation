
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Arduino.h>
#include <math.h>

//Setting the timer used for interrupts
#define USE_TIMER_1     false 
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include <TimerInterrupt.h>
#define TIMER_INTERVAL_MS 50L

//Declare PID variables
extern "C"
{
#include <PID.h>
}

#define PID_KP 0.4f
#define PID_KI 1.0f
#define PID_KD 0.0f

#define PID_TAU 0.05f

#define PID_LIM_MIN -80.0f
#define PID_LIM_MAX 80.0f

#define PID_LIM_MIN_INT -40.0f
#define PID_LIM_MAX_INT 40.0f

#define SAMPLE_TIME_S 0.05f

//Import encoder library and servo for motor control
#include <Encoder.h>
#include <Servo.h>

//Declare pin for encoders and motor
#define ENCA1 2
#define ENCA2 3
#define ENCB1 18
#define ENCB2 19
#define IN1 5
#define IN2 8

// TimerInterrupt ITimer;
Servo wheel1, wheel2;
Encoder enc1(ENCA1, ENCA2), enc2(ENCB1, ENCB2);
PIDController pid_left = {PID_KP,
                     PID_KI, PID_KD,
                     PID_TAU,
                     PID_LIM_MIN, PID_LIM_MAX,
                     PID_LIM_MIN_INT, PID_LIM_MAX_INT,
                     SAMPLE_TIME_S};

PIDController pid_right = {PID_KP,
                     PID_KI, PID_KD,
                     PID_TAU,
                     PID_LIM_MIN, PID_LIM_MAX,
                     PID_LIM_MIN_INT, PID_LIM_MAX_INT,
                     SAMPLE_TIME_S};

long posPrev[2] = {0, 0};
float setRPM = -7.0;
float velocity[2] = {0.0,0.0};
long pos[2] = {0, 0};

unsigned long currT = 0;
unsigned long prevT = 0;

void setMotor(int pwr, Servo wheel)
{
  // Serial.println(pwr);
  // wheel1.write(90 + pwr);
  wheel.write(90 + pwr);
}

void controlMotor()
{
  // Read Encoder
  velocity[0] = (posPrev[0]-pos[0]) * 1e3 / ((float)TIMER_INTERVAL_MS);
  velocity[1] = (pos[1]-posPrev[1]) * 1e3 / ((float)TIMER_INTERVAL_MS);
  velocity[0] = (velocity[0] / 1600.0)*60.0;
  velocity[1] = (velocity[1] / 1600.0)*60.0;

  posPrev[0] = pos[0];
  posPrev[1] = pos[1];

  //Compute PID
  int pwr_left = PIDController_Update(&pid_left, setRPM, velocity[0]);
  int pwr_right = PIDController_Update(&pid_right, setRPM, velocity[1]);

  Serial.println(millis());
  // if (millis()>8000){
  //   // Serial.println('HI');
  //   pwr_left = 0;
  //   pwr_right = 0;
  // }
  
  setMotor(pwr_left, wheel1);
  setMotor(pwr_right,wheel2);
  Serial.println("-------------------------------------------------");
  Serial.print("SETRPM: ");
  Serial.println(setRPM);
  Serial.println("**LEFT**");
  Serial.println(static_cast<String>("1: position: ") + pos[0]);
  Serial.println(static_cast<String>("2: velocity: ") + velocity[0]);
  Serial.println(static_cast<String>("3: commanded voltage: ") + pwr_left);
  Serial.println("**RIGHT**");

  Serial.println(static_cast<String>("1: position: ") + pos[1]);
  Serial.println(static_cast<String>("2: velocity: ") + velocity[1]);
  Serial.println(static_cast<String>("3: commanded voltage: ") + pwr_right);

  // Serial.println(static_cast<String>("3: propotional: ") + pid_left.proportional);
  // Serial.println(static_cast<String>("3: integral: ") + pid_left.integral);
  // Serial.println(static_cast<String>("3: derivative: ") + pid_left.differentiator);

  Serial.println("-------------------------------------------------");

  // if(currT-prevT>5000){
  //   setRPM += 1;
  //   prevT = currT;
  // }

}

void setup()
{
  Serial.begin(115200);
  // pinMode(ENCA, INPUT_PULLUP);
  // pinMode(ENCB, INPUT_PULLUP);
  wheel1.attach(IN1);
  wheel2.attach(IN2);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB
  }
  //Init PID
  PIDController_Init(&pid_left);
  PIDController_Init(&pid_right);
  // Init timer ITimer1
  ITimer2.init();
  if (ITimer2.attachInterruptInterval(TIMER_INTERVAL_MS, controlMotor))
  {
    Serial.print(F("Starting  ITimer3 OK, millis() = "));
    Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));

  // wheel1.write(100);
  // wheel2.write(100);
}

void loop()
{
  currT = millis();
  pos[0] = enc1.read();
  pos[1] = enc2.read();
  // Serial.println(M_PI);
  // setRPM = (int) Serial.read();
  // Serial.println("HELLO");
  // put your main code here, to run repeatedly:
  // Serial.println(pos);
}