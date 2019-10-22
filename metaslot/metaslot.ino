#include <Gamebuino-Meta.h>

unsigned long times[4] = {0,149974,0,149975}; // 0 Total, 1 Best, 2 Actual, 3 Last
int val = 1; //IR
int IRPin = 3; //pin

//TIME CONVERSION FUNCTION
int seconds(int counttoconvert){
  counttoconvert = counttoconvert * 0.04 * 1000;
  return ( (counttoconvert) / 1000) % 60 ;
}
int minutes(int counttoconvert){
  counttoconvert = counttoconvert * 0.04 * 1000;
  return ( (counttoconvert) / 1000) / 60 ;
}
int cents(int counttoconvert){
  counttoconvert = counttoconvert * 0.04 * 1000;
  return ( (counttoconvert) % 1000) / 10 ;
}


void setup(){
 pinMode(IRPin,INPUT);
 gb.begin();
}

void loop(){
  while(!gb.update());
  gb.display.clear();

  val = digitalRead(IRPin);
  if (gb.buttons.pressed(BUTTON_A)) {
    val = 0;
  }
  if (gb.buttons.pressed(BUTTON_MENU)) {
    times[0] = 0;
    times[1] = 149974;
    times[2] = 0;
    times[3] = 149975;
  }
  
  //DRAW TITLE
  gb.display.setColor(WHITE);
  gb.display.println("Meta Slot Lap-Timer");
  gb.display.println("");
 
  //SET TIME
  // 0 Total, 1 Best, 2 Actual, 3 Last
  times[0]++;
  times[2]++;
  //if (times[2] < times[1]) times[1]=times[2];
 

  //DETECTION
  if (val == 0) {
    if (times[2] < times[1]) times[1]=times[2];
    times[3] = times[2];
    times[2] = 0;
    val = 1;
  }






  //DRAW TIMES

  //time
  gb.display.print("Time : ");
  gb.display.print(minutes(times[0]));
  gb.display.print(":");
  gb.display.print(seconds(times[0]));
  gb.display.print(":");
  if (cents(times[0]) < 10) gb.display.print("0");
  gb.display.println(cents(times[0]));
  
  gb.display.println("");

  gb.display.print("Best : ");
  gb.display.print(minutes(times[1]));
  gb.display.print(":");
  gb.display.print(seconds(times[1]));
  gb.display.print(":");
  if (cents(times[1]) < 10) gb.display.print("0");
  gb.display.println(cents(times[1]));

  gb.display.println("");
  
  gb.display.print("Actual : ");
  gb.display.print(minutes(times[2]));
  gb.display.print(":");
  gb.display.print(seconds(times[2]));
  gb.display.print(":");
  if (cents(times[2]) < 10) gb.display.print("0");
  gb.display.println(cents(times[2]));
  
  gb.display.print("Last : ");
  gb.display.print(minutes(times[3]));
  gb.display.print(":");
  gb.display.print(seconds(times[3]));
  gb.display.print(":");
  if (cents(times[3]) < 10) gb.display.print("0");
  gb.display.println(cents(times[3]));

  
}
