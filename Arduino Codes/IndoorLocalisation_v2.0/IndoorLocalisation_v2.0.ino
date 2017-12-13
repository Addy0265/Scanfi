//Wheel Params
#define PI_APP 3.1459265359
#define WHEEL_BASE 22 //cm
#define WHEEL_DIAMETER 7 //cm
#define WHEEL_RADIUS WHEEL_DIAMETER/2.0
#define WHEEL_CIRCUMFERENCE (2*PI_APP*(WHEEL_RADIUS))


// Encoder Params
#define RPM 60
#define TICKS_PER_ROTATION1 6240 //14040
#define TICKS_PER_ROTATION2 6240 //18720
#define OFFSET 0

// Communication Params
int mov_x = 0, mov_y = 0;
String inputString = "";         // a string to hold incoming data
String x_str = "", y_str = "";
boolean stringComplete = false;  // whether the string is complete
int ack = 101;

char data[100];
int i = 0;

//Chassis Parameters
const float bot360Circum = 2*PI_APP*WHEEL_BASE/2.0;

//Global Position of Motors
int g_angle[2] = {0,0};

void _parse()
{
  int len = inputString.length();
  int i=0, j=0, k=0;
  
  for(i=0; inputString[i] != ' '; i++)
  {
    x_str += inputString[i];  
  }
  i++;
  x_str[i] = '\0';
  
  for(j=0; inputString[i] != '\n'; i++,j++)
  {
    y_str += inputString[i];  
  }
  i++;
  j++;
  y_str[j] = '\0';
  
  mov_x = x_str.toInt();
  mov_y = y_str.toInt();
}

void _print()
{
      Serial.print("\t Coordinates from PI ");
      Serial.print("\t Mov_X : "); Serial.print(mov_x);
      Serial.print("\t Mov_Y : "); Serial.println(mov_y);  
}

void reInit(){
  inputString = "";
  x_str = "";
  y_str = "";
  stringComplete = false;
}

void sendAck(){
    Serial.println(ack);
}

void handlePiData(){
  if (stringComplete) {
    _parse();
    //_print();
    sendAck();
    reInit();    
  }
}

void turnMotor(int angle, int side){
  int tpr = 0;
  int rect_angle = 0;
  
  if(side == 1)
  {
    tpr = TICKS_PER_ROTATION1;
    rect_angle = angle + g_angle[0];
    g_angle[0] = rect_angle;
  }
  else if(side == 2)
  {
    tpr = TICKS_PER_ROTATION2;
    rect_angle = angle + g_angle[1];
    g_angle[1] = rect_angle; 
  }

  
  long ticks = (tpr/360.0)*rect_angle;
  char cnt[20];
  String pCmd = 'P' + String(ticks) + '\r';
  
  int i = 0;
  
  //Serial.print(ticks);
  
  for(i=0; i<pCmd.length(); i++)
  {
    cnt[i] = (char)pCmd[i];
    if(side == 1)
      Serial1.write(cnt[i]);
    else if(side == 2)
      Serial2.write(cnt[i]);
  }
}

void testRun1(){
  delay(1000);
  Serial.println("Yo!!");
  delay(3000);
  Serial1.write("p2000\r");
  delay(3000);
  Serial1.write("p5000\r");
  delay(3000);
  Serial1.write("p6240\r");
  delay(3000);
  Serial1.write("p0\r");
}

void testRun2(){
  turnMotor(180, 1);
  turnMotor(180, 2);
  delay(5000);
  turnMotor(360, 1);
  turnMotor(360, 2);
  delay(5000);
  turnMotor(0, 1);
  turnMotor(0, 2);
}

void botTurn(int angle){
  float distToMove = (bot360Circum/360.0)*angle;
  int wheelAngle = (distToMove/(WHEEL_CIRCUMFERENCE))*360.0;
  int wheelAngle1 = wheelAngle, wheelAngle2 = wheelAngle;

  if(wheelAngle > 0)
  {
    wheelAngle1 = wheelAngle-OFFSET;
    wheelAngle2 = wheelAngle;
  }
  else if(wheelAngle < 0)
  {
    wheelAngle1 = wheelAngle+OFFSET;
    wheelAngle2 = wheelAngle;
  }
  
  turnMotor(wheelAngle1, 1);
  turnMotor(wheelAngle2, 2);
}

void botMove(int distance){
  int wheelAngle = (distance*360.0)/(WHEEL_CIRCUMFERENCE);
  int wheelAngle1 = wheelAngle, wheelAngle2 = wheelAngle;

  if(wheelAngle > 0)
  {
    wheelAngle1 = wheelAngle-OFFSET;
    wheelAngle2 = wheelAngle;
  }
  else if(wheelAngle < 0)
  {
    wheelAngle1 = wheelAngle+OFFSET;
    wheelAngle2 = wheelAngle;
  }
  
  turnMotor(wheelAngle1, 1);
  turnMotor(-wheelAngle2, 2);
}

void scan(int d)
{
  botMove(d);
  delay(8000);
  botMove(-d);
  delay(8000);
  botTurn(90);
  delay(3000);
  botMove(d);
  delay(8000);
  botMove(-d);
  delay(8000);
  botTurn(-90);
  delay(3000);
}

void setup() {
  Serial.begin(38400);
  Serial1.begin(38400);
  Serial2.begin(38400); 

  inputString.reserve(10);
  x_str.reserve(4);
  y_str.reserve(4);
  
  //testRun2();
  //botTurn(90);
  //botTurn(180);
  //botMove(10);
  scan(40);
   //turnMotor(90, 1);
   //turnMotor(-158, 2);
  Serial.print("Angle1: "); Serial.print(g_angle[0]); Serial.print("\t Angle2: "); Serial.println(g_angle[1]);
  
}

void loop() {
  
 // delay(100);
 handlePiData();
  
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
