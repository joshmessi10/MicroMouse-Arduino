//--------------TEST MISSING--------------
const int searchMoveTime = 2000;
const int searchTurnTime = 800; 
int searchPWMA = 125;
int searchPWMB = 75;

const int solveMoveTime = 1500;
const int solveTurnTime = 600; 
int solvePWMA = 200;
int solvePWMB = 125;

const int fastMoveTime = 1000;
const int fastTurnTime = 400; 
int fastPWMA = 250;
int fastPWMB = 150;


void beginning(){
  Serial.begin(115200);
  //--------------SENSING--------------
  servo1.attach(servoPin);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //--------------BLUETOOTH--------------
  SerialBT.begin("ESP32test"); //Bluetooth device name
  //--------------MOTOR--------------
  pinMode(PinENA, OUTPUT);
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinENB, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledG, OUTPUT);
}

//-----------SENSING-------------------------------------------
void scanning(){
  for(int posDegrees = 0; posDegrees <= 90; posDegrees=posDegrees+5) {servo1.write(posDegrees);delay(20);}
  digitalWrite(trigPin, LOW);delayMicroseconds(2);digitalWrite(trigPin, HIGH);delayMicroseconds(10);digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);distanceF = duration * SOUND_SPEED/2;delay(500);
  //Serial.print("Distance F (cm): ");Serial.println(distanceF);

  for(int posDegrees = 90; posDegrees <= 180; posDegrees=posDegrees+5) {servo1.write(posDegrees);delay(20);}
  digitalWrite(trigPin, LOW);delayMicroseconds(2);digitalWrite(trigPin, HIGH);delayMicroseconds(10);digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);distanceL = duration * SOUND_SPEED/2;delay(500);
  //Serial.print("Distance L (cm): ");Serial.println(distanceL);

  for(int posDegrees = 180; posDegrees >= 0; posDegrees=posDegrees-5) {servo1.write(posDegrees);delay(20);}
  digitalWrite(trigPin, LOW);delayMicroseconds(2);digitalWrite(trigPin, HIGH);delayMicroseconds(10);digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);distanceR = duration * SOUND_SPEED/2;delay(500);
  //Serial.print("Distance R (cm): ");Serial.println(distanceR);

  if(distanceL < 15){walls[0] = 1;}
  else{walls[0] = 0;}
  if(distanceF < 15){walls[1] = 1;}
  else{walls[1] = 0;}
  if(distanceR < 15){walls[2] = 1;}
  else{walls[2] = 0;}
}

//-----------BLUETOOTH-------------------------------------------
void send(){
    for (int i = 0; i < 11; i++) {
      for (int j = 0; j < 11; j++) {
        SerialBT.print(laberinto[i][j]);
      }
    }
}

//-----------Motors-------------------------------------------
void forwardM(int moveTime, int PWMA, int PWMB){
  analogWrite(PinENA,PWMA);
  analogWrite(PinENB,PWMB);
  digitalWrite (PinIN1, HIGH);
  digitalWrite (PinIN2, LOW);
  digitalWrite (PinIN3, HIGH);
  digitalWrite (PinIN4, LOW);
  delay(moveTime);
  stopM();
}
void backwardM(int moveTime, int PWMA, int PWMB){
  analogWrite(PinENA,PWMA);
  analogWrite(PinENB,PWMB);
  digitalWrite (PinIN1, LOW);
  digitalWrite (PinIN2, HIGH);
  digitalWrite (PinIN3, LOW);
  digitalWrite (PinIN4, HIGH);
  delay(moveTime);
  stopM();
}
 
void leftM(int turnTime, int PWMA, int PWMB){
  char newDir;
  switch(dir){
    case 'N':
      newDir = 'W';
      break;
    case 'E':
      newDir = 'N';
      break;
    case 'S':
      newDir = 'E';
      break;
    case 'W':
      newDir = 'S';
      break;
  }
  dir = newDir;

  analogWrite(PinENA,PWMA);
  analogWrite(PinENB,PWMB);
  digitalWrite (PinIN1, HIGH);
  digitalWrite (PinIN2, LOW);
  digitalWrite (PinIN3, LOW);
  digitalWrite (PinIN4, HIGH);
  delay(turnTime);
  stopM();
}
void rightM(int turnTime, int PWMA, int PWMB){
  char newDir;
  switch(dir){
    case 'N':
      newDir = 'E';
      break;
    case 'E':
      newDir = 'S';
      break;
    case 'S':
      newDir = 'W';
      break;
    case 'W':
      newDir = 'N';
      break;
  }
  dir = newDir;
  analogWrite(PinENA,PWMA);
  analogWrite(PinENB,PWMB);
  digitalWrite (PinIN1, LOW);
  digitalWrite (PinIN2, HIGH);
  digitalWrite (PinIN3, HIGH);
  digitalWrite (PinIN4, LOW);
  delay(turnTime);
  stopM();
}
 
void stopM(){
  digitalWrite (PinIN1, LOW);
  digitalWrite (PinIN2, LOW);
  digitalWrite (PinIN3, LOW);
  digitalWrite (PinIN4, LOW);
}

void DFS_search(){
  if(!mappingChk){
    scanning();
    mapping();
    decisionMaking();
    robotMovementDFS();
    bluetoothConnection();
  }
}

void mapping(){
  int frontPos[2];
  int leftPos[2];
  int rightPos[2];
  switch (dir)
    {
    case 'N':
        frontPos[0] = x-1;
        frontPos[1] = y;
        leftPos[0] = x;
        leftPos[1] = y-1;
        rightPos[0] = x;
        rightPos[1] = y+1;
      break;
    case 'E':
        frontPos[0] = x;
        frontPos[1] = y+1;
        leftPos[0] = x-1;
        leftPos[1] = y;
        rightPos[0] = x+1;
        rightPos[1] = y;
      break;
          
    case 'S':
        frontPos[0] = x+1;
        frontPos[1] = y;
        leftPos[0] = x;
        leftPos[1] = y+1;
        rightPos[0] = x;
        rightPos[1] = y-1;
      break;
    case 'W':
        frontPos[0] = x;
        frontPos[1] = y+1;
        leftPos[0] = x+1;
        leftPos[1] = y;
        rightPos[0] = x-1;
        rightPos[1] = y;
      break;
    }
    if(walls[0] = 1){
      laberinto[leftPos[0]][leftPos[1]] = '/';
    }
    else{
      laberinto[leftPos[0]][leftPos[1]] = '+';
    }
    if(walls[1] = 1){
      laberinto[frontPos[0]][frontPos[1]] = '/';
    }
    else{
      laberinto[frontPos[0]][frontPos[1]] = '+';
    }
    if(walls[2] = 1){
      laberinto[rightPos[0]][rightPos[1]] = '/';
    }
    else{
      laberinto[rightPos[0]][rightPos[1]] = '+';
    }
}

void decisionMaking(){
  if (walls[0] == 0 && walls[1] == 0 && walls[2] == 1){
    st_move = LEFT;
  }
  if (walls[0] == 1 && walls[1] == 0 && walls[2] == 1){
    st_move = FRONT;
  }
  if (walls[0] == 0 && walls[1] == 1 && walls[2] == 0){
    st_move = RIGHT;
  }
  if (walls[0] == 1 && walls[1] == 0 && walls[2] == 0){
    st_move = RIGHT;
  }
  if (walls[0] == 1 && walls[1] == 1 && walls[2] == 0){
    st_move = RIGHT;
  }
  if (walls[0] == 0 && walls[1] == 1 && walls[2] == 1){
    st_move = LEFT;
  }
  if (walls[0] == 1 && walls[1] == 1 && walls[2] == 1){
    st_move = RETURN;
  }
  else{
    st_move = EXCEPT;
  }
}

void robotMovementDFS(){
  int frontPos[2];
  int leftPos[2];
  int rightPos[2];
  int timeM, timeT, PWMA, PWMB;
  timeM = searchMoveTime;
  timeT = searchTurnTime;
  PWMA = searchPWMA;
  PWMB = searchPWMB;
  switch (dir)
    {
    case 'N':
        frontPos[0] = x-2;
        frontPos[1] = y;
        leftPos[0] = x;
        leftPos[1] = y-2;
        rightPos[0] = x;
        rightPos[1] = y+2;
      break;
    case 'E':
        frontPos[0] = x;
        frontPos[1] = y+2;
        leftPos[0] = x-2;
        leftPos[1] = y;
        rightPos[0] = x+2;
        rightPos[1] = y;
      break;
          
    case 'S':
        frontPos[0] = x+2;
        frontPos[1] = y;
        leftPos[0] = x;
        leftPos[1] = y+2;
        rightPos[0] = x;
        rightPos[1] = y-2;
      break;
    case 'W':
        frontPos[0] = x;
        frontPos[1] = y+2;
        leftPos[0] = x+2;
        leftPos[1] = y;
        rightPos[0] = x-2;
        rightPos[1] = y;
      break;
    }
  switch(st_move){
    case FRONT:
      x = frontPos[0];
      y = frontPos[1];
      forwardM(timeM, PWMA, PWMB);
      stopM();
      path += 'S';
      break;
    case LEFT:
      x = leftPos[0];
      y = leftPos[1];
      leftM(timeT, PWMA, PWMB);
      forwardM(timeM, PWMA, PWMB);
      stopM();
      path += 'L';
      break;
    case RIGHT:
      x = rightPos[0];
      y = rightPos[1];
      rightM(timeT, PWMA, PWMB);
      forwardM(timeM, PWMA, PWMB);
      stopM();
      path += 'R';
      break;
    case RETURN:
      rightM(timeT, PWMA, PWMB);
      rightM(timeT, PWMA, PWMB);
      forwardM(timeM, PWMA, PWMB);
      stopM();  
      path += 'B';
      break;
    case EXCEPT:
      st_robot = WAIT;
      break;
  }
}

void bluetoothConnection(){
  if(SerialBT.available()){
    send();
    for (int i = 0; i < 100; i++) {
      int input = SerialBT.read();
      delay(10);
      if(input == 10){
        setGoal();
        mappingChk = true;
        st_robot = WAIT;
        break;
      }
    }
  }
}

void DFS_solve(){
  execute_path(solve_path);
}

String replace_rules(String path) {
    while (path.indexOf("B") != -1) {
        path.replace("LBR", "B");
        path.replace("LBS", "R");
        path.replace("RBL", "B");
        path.replace("SBL", "R");
        path.replace("SBS", "B");
        path.replace("LBL", "S");
        path.replace("RBR", "S");
    }
    return path;
}

void execute_path(String path) {
  int frontPos[2];
  int leftPos[2];
  int rightPos[2];
  int timeM, timeT, PWMA, PWMB;
  switch(st_robot){
    case SOLVE_DFS:
      timeM = solveMoveTime;
      timeT = solveTurnTime;
      PWMA = solvePWMA;
      PWMB = solvePWMB;
      break;
    case FAST_DFS:
      timeM = fastMoveTime;
      timeT = fastTurnTime;
      PWMA = fastPWMA;
      PWMB = fastPWMB;
      break;
  }
  for (int i = 0; i < path.length(); i++) {
    switch (dir)
    {
    case 'N':
        frontPos[0] = x-2;
        frontPos[1] = y;
        leftPos[0] = x;
        leftPos[1] = y-2;
        rightPos[0] = x;
        rightPos[1] = y+2;
      break;
    case 'E':
        frontPos[0] = x;
        frontPos[1] = y+2;
        leftPos[0] = x-2;
        leftPos[1] = y;
        rightPos[0] = x+2;
        rightPos[1] = y;
      break;
          
    case 'S':
        frontPos[0] = x+2;
        frontPos[1] = y;
        leftPos[0] = x;
        leftPos[1] = y+2;
        rightPos[0] = x;
        rightPos[1] = y-2;
      break;
    case 'W':
        frontPos[0] = x;
        frontPos[1] = y+2;
        leftPos[0] = x+2;
        leftPos[1] = y;
        rightPos[0] = x-2;
        rightPos[1] = y;
      break;
    }
    char move = path[i];
    laberinto[x][y]='G';
    if (move == 'S') {
      forwardM(timeM, PWMA, PWMB);
      x = frontPos[0];
      y = frontPos[1];
    } else if (move == 'L') {
      leftM(timeT, PWMA, PWMB);
      forwardM(timeM, PWMA, PWMB);
      x = leftPos[0];
      y = leftPos[1];
    } else if (move == 'R') {
      rightM(timeT, PWMA, PWMB);
      forwardM(timeM, PWMA, PWMB);
      x = rightPos[0];
      y = rightPos[1];
    }
    send();
  }
  st_robot = WAIT;
}


void setGoal(){
  goalX = x;
  goalY = y;
}

void redOn(){
  digitalWrite(ledR, HIGH);
  digitalWrite(ledY, LOW);
  digitalWrite(ledG, LOW);
}
void yellowOn(){
  digitalWrite(ledR, LOW);
  digitalWrite(ledY, HIGH);
  digitalWrite(ledG, LOW);
}
void greenOn(){
  digitalWrite(ledR, LOW);
  digitalWrite(ledY, LOW);
  digitalWrite(ledG, HIGH);
}

void execute_FF() {
  int frontPos[2];
  int leftPos[2];
  int rightPos[2];
  int timeM, timeT, PWMA, PWMB;
  switch(st_robot){
    case SOLVE_FF:
      timeM = solveMoveTime;
      timeT = solveTurnTime;
      PWMA = solvePWMA;
      PWMB = solvePWMB;
      break;
    case FAST_FF:
      timeM = fastMoveTime;
      timeT = fastTurnTime;
      PWMA = fastPWMA;
      PWMB = fastPWMB;
      break;
  }
  switch (dir)
  {
  case 'N':
      frontPos[0] = x-2;
      frontPos[1] = y;
      leftPos[0] = x;
      leftPos[1] = y-2;
      rightPos[0] = x;
      rightPos[1] = y+2;
    break;
  case 'E':
      frontPos[0] = x;
      frontPos[1] = y+2;
      leftPos[0] = x-2;
      leftPos[1] = y;
      rightPos[0] = x+2;
      rightPos[1] = y;
    break;
        
  case 'S':
      frontPos[0] = x+2;
      frontPos[1] = y;
      leftPos[0] = x;
      leftPos[1] = y+2;
      rightPos[0] = x;
      rightPos[1] = y-2;
    break;
  case 'W':
      frontPos[0] = x;
      frontPos[1] = y+2;
      leftPos[0] = x+2;
      leftPos[1] = y;
      rightPos[0] = x-2;
      rightPos[1] = y;
    break;
  }
  char move;
  laberinto[x][y]='G';
  if(FFW[x2][y2] == 0){
    st_robot = WAIT;
  }
  else{
    // ------------IT DEPENDS__________
    if (move == 'S') {
      forwardM(timeM, PWMA, PWMB);
      x = frontPos[0];
      y = frontPos[1];
    } else if (move == 'L') {
      leftM(timeT, PWMA, PWMB);
      forwardM(timeM, PWMA, PWMB);
      x = leftPos[0];
      y = leftPos[1];
    } else if (move == 'R') {
      rightM(timeT, PWMA, PWMB);
      forwardM(timeM, PWMA, PWMB);
      x = rightPos[0];
      y = rightPos[1];
    }
  }   
  send();
}


/*
For Later:
switch(st_robot){
    case MAP_DFS:
      timeM = searchMoveTime;
      timeT = searchTurnTime;
      PWMA = searchPWMA;
      PWMB = searchPWMB;
      break;
    case MAP_FF:
      timeM = searchMoveTime;
      timeT = searchTurnTime;
      PWMA = searchPWMA;
      PWMB = searchPWMB;
      break;
    case SOLVE_DFS:
      timeM = solveMoveTime;
      timeT = solveTurnTime;
      PWMA = solvePWMA;
      PWMB = solvePWMB;
      break;
    case SOLVE_FF:
      timeM = solveMoveTime;
      timeT = solveTurnTime;
      PWMA = solvePWMA;
      PWMB = solvePWMB;
      break;
    case FAST_DFS:
      timeM = solveMoveTime;
      timeT = solveTurnTime;
      PWMA = fastPWMA;
      PWMB = fastPWMB;
      break;
    case FAST_FF:
      timeM = fastMoveTime;
      timeT = fastTurnTime;
      PWMA = fastPWMA;
      PWMB = fastPWMB;
      break;
    
  }
*/