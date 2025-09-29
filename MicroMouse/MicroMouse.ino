//BLUETOOTH
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


//--------------SERVOMOTOR--------------
#include <Servo.h>

static const int servoPin = 13;

Servo servo1;

//--------------SENSING--------------
const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceL;
float distanceF;
float distanceR;

//--------------MOTOR--------------
const int PinENA = 14;
const int PinIN1 = 26;
const int PinIN2 = 27;
const int PinIN3 = 25;
const int PinIN4 = 33;
const int PinENB = 32;

//--------------MAP--------------
char laberinto[11][11] = {
  {'/', '/', '/', '/', '/', '/', '/', '/', '/', '/', '/'},
  {'/', '?', '+', '?', '+', '?', '+', '?', '-', '?', '/'},
  {'/', '+', '/', '+', '/', '-', '/', '+', '/', '+', '/'},
  {'/', '?', '-', '?', '-', '?', '-', '?', '+', '?', '/'},
  {'/', '+', '/', '+', '/', '+', '/', '-', '/', '-', '/'},
  {'/', '?', '-', '?', '-', '?', '+', '?', '+', '?', '/'},
  {'/', '-', '/', '+', '/', '-', '/', '-', '/', '+', '/'},
  {'/', '?', '+', '?', '+', '?', '+', '?', '+', '?', '/'},
  {'/', '+', '/', '-', '/', '-', '/', '-', '/', '+', '/'},
  {'/', '?', '-', '?', '+', '?', '+', '?', '+', '?', '/'},
  {'/', '/', '/', '/', '/', '/', '/', '/', '/', '/', '/'}
  //0    1    2    3    4    5    6    7    8    9   10
};

int x = 9;
int y = 1;
int x2 = 5;
int y2 = 1;
int goalX, goalY;
bool mappingChk = false;
char dir = 'N';

int walls[3] = {0,0,0}; //LFR

//------------LEDS---------------
int ledR = 16;
int ledY = 17;
int ledG = 19;

//------------STATES---------------
typedef enum{
  WAIT = 0,
  MAP_DFS,
  MAP_FF,
  SOLVE_DFS,
  SOLVE_FF,
  FAST_DFS,
  FAST_FF
}ST_ROBOT;

//------------MOVE---------------
typedef enum{
  STOP=0,
  FRONT,
  RIGHT,
  LEFT,
  RETURN,
  EXCEPT
}ST_MOVE;

ST_ROBOT st_robot = WAIT;
ST_MOVE st_move = STOP;


//-----------DFS---------------
String path;
String solve_path;


//-----------Flood Fill ------------
byte FFW[5][5]={
  {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
  {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
  {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
  {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
  {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}
};

int FFH[7][7]={
  {99,99,99,99,99,99,99},
  {99,0,0,0,0,0,99},
  {99,0,0,0,0,0,99},
  {99,0,0,0,0,0,99},
  {99,0,0,0,0,0,99},
  {99,0,0,0,0,0,99},
  {99,99,99,99,99,99,99}
};

void setup() {
  beginning();
}

void loop() {
  reading();
  process();
}

void reading(){
  if(SerialBT.available()){
    int input = SerialBT.read();
    if(input == 0){
      st_robot = WAIT;
    }
    if(input == 11){
      st_robot = MAP_DFS;
      yellowOn();
    }
    if(input == 12){
      st_robot = MAP_FF;
      yellowOn();
    }
    if(input == 21 && mappingChk){
      st_robot = SOLVE_DFS;
      solve_path = replace_rules(path);
      greenOn();
    }
    if(input == 22 && mappingChk){
      st_robot = SOLVE_FF;
      greenOn();
    }
    if(input == 31 && mappingChk){
      st_robot = FAST_DFS;
      solve_path = replace_rules(path);
      greenOn();
    }
    if(input == 32 && mappingChk){
      st_robot = FAST_FF;
      greenOn();
    }

  }
  
  
}

void process(){
  switch(st_robot){
    case WAIT:
      stopM();
      redOn();
      break;
    case MAP_DFS:
      DFS_search();
      break;
    case SOLVE_DFS:
      DFS_solve();
      break;
    case FAST_DFS:
      DFS_solve();
      break;

  }
}

