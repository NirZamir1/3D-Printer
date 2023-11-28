//current
//Speed in mm.s
// Define pin connections & motor's steps per revolution
float X_pos;
float Y_pos;
float Z_pos;
const int SwitchX = 11;
const int SwitchZ = 9;
const int SwitchY = 10;
const int dirPinX = 8                                                                                               ;
const int stepPinX = 3;
const int dirPinZ = 6;
const int stepPinZ = 7;
const int dirPinY = 4;
const int stepPinY = 5;
const float C = (38.19719+0.63*2)*PI;
const float stepsPerRevolution = 400;//מספר צעדים לסיבוב שלם כאשר המנוע במצב חצי
double CyclePerSecond =2.5;// זמן מחזור 0.4 ערך מינימלי
long timeDelay = (CyclePerSecond*(1000000))/stepsPerRevolution-2000;// מחשב את הדילי בשביל להשלים סיבוב כל מספר זמן המחזור
float StepResXY = C/stepsPerRevolution;
float StepResZ = 1/(stepsPerRevolution/2);

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPinZ, OUTPUT);
  pinMode(dirPinZ, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(SwitchX, INPUT);
  pinMode(SwitchY, INPUT);
  pinMode(SwitchZ, INPUT);
  Serial.begin(9600);
  Homing();
}
void loop()
{  
  /*Serial.println(digitalRead(SwitchX));
  Serial.println(digitalRead(SwitchZ));
  Serial.println(digitalRead(SwitchY));*/
  long mil = millis();
   if(Serial.available())
  {
    String Pos;
     Pos = Serial.readString();

  MoveToPos(Pos);
   
    mil = millis() - mil;
    Serial.println(mil);
   }
   
}
void Homing()
{
  //Homing X axis;
  while(!digitalRead(SwitchX))
  {
    digitalWrite(dirPinX,HIGH);
    Step(stepPinX);
    }
  X_pos = 0;
 
  //Homing Y axis;
  while(!digitalRead(SwitchY))
  {
    digitalWrite(dirPinY,HIGH);
    Step(stepPinY);
    }
  Y_pos = 0;
 
  //Homing Z axis;
 
  while(!digitalRead(SwitchZ))
  {
    digitalWrite(dirPinZ,HIGH);
    Step(stepPinZ);
    }
  Z_pos = 0;
  Serial.println("Finished Homing!");
  }
  void Step(int stepPin)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
void MoveToX(double Travel, double speed)
{
   if(Travel < 0){
    digitalWrite(dirPinX,1);
    Travel = -Travel;
   }
   else
       digitalWrite(dirPinX,0);
       
   float SpeedDelay;
   int stepNum = (int) Travel/StepResXY;
   SpeedDelay = (((int) Travel/speed) * 1000 - stepNum*2)/stepNum; // Delay in millis
   int delayVal;
   bool isMicro = false;
   if(SpeedDelay<15)
   {
    isMicro = true;
    delayVal = (int) (SpeedDelay*1000);
    }
    else
    {
      delayVal = (int)SpeedDelay;
      }
   Serial.print("StepRes: ");
   Serial.println(StepResXY);
   Serial.print("StepNum: ");
   Serial.println(stepNum);
   Serial.print("StepDelay: ");
   Serial.println(delayVal);
   if(!isMicro)
   for(int i = 0; i < stepNum; i++ )
   {
     Step(stepPinX);
     delay(SpeedDelay);
   }
   else
   {
    for(int i = 0; i < stepNum; i++ )
   {
     Step(stepPinX);
     delayMicroseconds(SpeedDelay);
   }
    }
}
void MoveToY(double Travel, double speed)
{
   if(Travel < 0){
    digitalWrite(dirPinY,1);
    Travel = -Travel;
   }
   else
       digitalWrite(dirPinY,0);

    float SpeedDelay;
   int stepNum = (int) Travel/StepResXY;
    SpeedDelay = (((int) Travel/speed) * 1000 - stepNum*2)/stepNum; // Delay in millis
   int delayVal;
   bool isMicro = false;
   if(SpeedDelay<15)
   {
    isMicro = true;
    delayVal = (int) (SpeedDelay*1000);
    }
    else
    {
      delayVal = (int)SpeedDelay;
      }
   Serial.print("StepRes: ");
   Serial.println(StepResXY);
   Serial.print("StepNum: ");
   Serial.println(stepNum);
   Serial.print("StepDelay: ");
   Serial.println(delayVal);
   if(!isMicro)
   for(int i = 0; i < stepNum; i++ )
   {
     Step(stepPinY);
     delay(SpeedDelay);
   }
   else
   {
    for(int i = 0; i < stepNum; i++ )
   {
     Step(stepPinY);
     delayMicroseconds(SpeedDelay);
   }
    }
   
}
void MoveToZ(double Travel, double speed)
{
   if(Travel < 0){
    digitalWrite(dirPinZ,1);
    Travel = -Travel;
   }
   else
       digitalWrite(dirPinZ,0);
       
   Serial.println(Travel);
   long stepNum = (long) Travel/StepResZ;
    if(speed> 1.25){
    speed = 1.25;
    }
   float SpeedDelay = ( (Travel/speed) * 1000 - stepNum*2)/stepNum; // Delay in millis
   int delayVal;
   bool isMicro = false;
   if(SpeedDelay<15)
   {
    isMicro = true;
    delayVal = (int) (SpeedDelay*1000);
    }
    else
    {
      delayVal = (int)SpeedDelay;
      }
   Serial.print("StepRes: ");
   Serial.println(StepResZ);
   Serial.print("StepNum: ");
   Serial.println(stepNum);
   Serial.print("StepDelay: ");
   Serial.println(delayVal);
   if(!isMicro)
   for(long i = 0; i < stepNum; i++ )
   {
     Step(stepPinZ);
     delay(SpeedDelay);
   }
   else
   {
    for(long i = 0; i < stepNum; i++ )
   {
     Step(stepPinZ);
     delayMicroseconds(SpeedDelay);
    }
   }
}
void MoveToPos(String Input)
{
  bool isNegative = false;
  Serial.println(Input);
  String arr[3];
  int k = 0;
  for(int i = 0; i<Input.length(); i++)
  {
    Serial.println(Input[i]);
    if(Input[i] == ',')
    {
      k++;
    }
    else
    {
      arr[k] += Input[i];
    }
    if(Input[i] == '-')
    {
      isNegative = true;
      }
  }
  float WantedX = arr[0].toInt();
  float WantedY = arr[1].toInt();
  float WantedZ = arr[2].toInt();
  if(isNegative)
   Serial.println("Value was negative");
  else
  {
  MoveToX(WantedX-X_pos,20);
  MoveToY(WantedY-Y_pos,20);
  MoveToZ(WantedZ-Z_pos,20);
  X_pos = WantedX;
  Y_pos = WantedY;
  Z_pos = WantedZ;
}
}




