#include <Gamebuino-Meta.h>

const char* main_menu[] = {
"Training",
"2 Slotcars",
"4 Slotcars",
};

byte i;
byte mode = 0; //0 = training, 1 = 2 players, 2 = 4 players
byte pilots = 4;
long race_time = 0;
long prev_race_time = 0;
long slot_time[4] = {0,0,0,0};
long prev_slot_time[4] = {0,0,0,0};
long best_slot_time[4] = {300000,300000,300000,300000};
long last_time[][5] = { {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0} };
byte last_temp;
byte IR[4] = {1,1,1,1}; //IR detector 1 false, 0 true
byte prev_IR[4] = {1,1,1,1}; 
boolean newlap[4] = {false, false, false, false};
byte IRPin[4] = {3,4,5,6}; // IR pins


//TIME CONVERSION FUNCTION (time in millis)
byte seconds(long timetoconvert){
  return ( (timetoconvert) / 1000) % 60 ;}
byte minutes(long timetoconvert){
  return ( (timetoconvert) / 1000) / 60 ;}
byte cents(int timetoconvert){
  return ( (timetoconvert) % 1000) / 10 ;}

//DRAW TIME true in second argument to println
void drawTime(int timetodraw, boolean ln){
  gb.display.print(" ");
  gb.display.print(minutes(timetodraw));
  gb.display.print(":");
  gb.display.print(seconds(timetodraw));
  gb.display.print(":");
  if (cents(timetodraw) < 10) gb.display.print("0");
  if (!ln) {
    gb.display.print(cents(timetodraw));
    gb.display.print(" - ");
    } else {gb.display.println(cents(timetodraw));}
}

//FAKE IR with D-PAD
void fake_IR() {  
  if (gb.buttons.repeat(BUTTON_UP, 0)) {
    IR[0] = 0;
  } else {  IR[0] = 1;}
  if (gb.buttons.repeat(BUTTON_DOWN, 0)) {
    IR[1] = 0;
  } else {  IR[1] = 1;}
  if (gb.buttons.repeat(BUTTON_LEFT, 0)) {
    IR[2] = 0;
  } else {  IR[2] = 1;}
  if (gb.buttons.repeat(BUTTON_RIGHT, 0)) {
    IR[3] = 0;
  } else {  IR[3] = 1;}
  }

//SETUP
void setup(){
 //Desactivate this line for fake_IR
 //for (i=0; i<=3; i++) {pinMode(IRPin[i],INPUT);}
 gb.begin();
 mode = gb.gui.menu("Mode", main_menu);
}

//LOOP
void loop(){
  //Time update
  race_time = (millis() - prev_race_time);
  for (i=0; i<=3; i++) {slot_time[i] = race_time - prev_slot_time[i];}


  //DETECTION IR and NEW LAP
  for (i=0; i<=3; i++) {
    fake_IR();
    //Desactivate this line for fake_IR
    //IR[i] = digitalRead(IRPin[i]);
    if (IR[i] == 1 && newlap[i] == true && slot_time[i] > 500) {newlap[i] = false;}
    if (IR[i] == 0 && newlap[i] == true) {continue;}
    if (IR[i] == 0 && newlap[i] == false) {
      newlap[i] = true;
      for (byte n=4; n>0; n--) {last_time[i][n] = last_time[i][n-1];}   //SAVE LAST TIMES
      last_time[i][0] = slot_time[i];
      prev_slot_time[i] = race_time;
    }
  }

  while(!gb.update());

  //UPDATE BEST LAP
  for (i=0; i<=3; i++) {
    if (last_time[i][0] > 0 && last_time[i][0] < best_slot_time[i]) {
      best_slot_time[i] = last_time[i][0];
      gb.gui.popup("Best Lap !", 25);
      }
  }
  
  gb.display.clear();
  
  //DRAW TITLE
  gb.display.setColor(WHITE);
  gb.display.println(" Meta Slot Lap-Timer");

  switch (mode) {
    case 0:
      //DRAW TIMES - player 1 training
      gb.display.println(" Training Mode");
      drawTime(slot_time[0],false);
      gb.display.setColor(RED);
      drawTime(best_slot_time[0],true);
      gb.display.setColor(WHITE);
      gb.display.println(" Last :");
      for (i=0; i<=4; i++) {
      drawTime(last_time[0][i],true);
      }
      break;
    case 1:
      //DRAW TIMES - 2 players
      gb.display.println(" 2 Drivers Mode");
      gb.display.println("");
      drawTime(race_time, true);
      gb.display.println("");
      gb.display.println("  TIME      BEST ");
      for (i=0; i<=1; i++) {
      drawTime(slot_time[i],false);
      drawTime(best_slot_time[i],true);
      gb.display.println("");
      }
      break;
    case 2:
      //DRAW TIMES - 4 players
      gb.display.println(" 4 Drivers Mode");
      gb.display.println("");
      drawTime(race_time, true);
      gb.display.println("  TIME      BEST ");
      for (i=0; i<=3; i++) {
      drawTime(slot_time[i],false);
      drawTime(best_slot_time[i],true);
      }
      break; 
  }
  

  

}
