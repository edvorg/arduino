//update from SAnwandter

#define ROW_1 2
#define ROW_2 3
#define ROW_3 4
#define ROW_4 5
#define ROW_5 6
#define ROW_6 7
#define ROW_7 8
#define ROW_8 9

#define COL_1 10
#define COL_2 11
#define COL_3 12
#define COL_4 13
#define COL_5 A0
#define COL_6 A1
#define COL_7 A2
#define COL_8 A3

const byte rows[] = {
    ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
const byte columns[] = {
  COL_1,COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};

float timeCount = 0;
int id = 0;

void setup() 
{
    // Open serial port
    Serial.begin(9600);
    
    // Set all used pins to OUTPUT
    // This is very important! If the pins are set to input
    // the display will be very dim.
    for (byte i = 2; i <= 13; i++)
        pinMode(i, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
}

bool world_a[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};
bool world_b[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};
unsigned long last_time = millis();
bool (*source)[8] = world_a;
bool (*dest)[8] = world_b;

int safe(int from_index, int offset) {
  return (8 + from_index + offset) % 8;
}

void update_world(bool (*source)[8], bool (*dest)[8]) {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
          bool alive = source[i][j];    
          int neighbours = 0;

          int i_minus_one = safe(i, -1);
          int i_plus_one = safe(i, +1);
          int j_minus_one = safe(j, -1);
          int j_plus_one = safe(j, +1);
          
          if (source[i_minus_one][j_minus_one]) {
            neighbours += 1;      
          }
          if (source[i_minus_one][j]) {
            neighbours += 1;      
          }
          if (source[i_minus_one][j_plus_one]) {
            neighbours += 1;      
          }

          
          if (source[i_plus_one][j_minus_one]) {
            neighbours += 1;      
          }
          if (source[i_plus_one][j]) {
            neighbours += 1;      
          }
          if (source[i_plus_one][j_plus_one]) {
            neighbours += 1;      
          }


          if (source[i][j_plus_one]) {
            neighbours += 1;
          }
          if (source[i][j_minus_one]) {
            neighbours += 1;
          }

          if (alive) {
              if (neighbours == 2 || neighbours == 3) {
                  dest[i][j] = 1;
              } else {
                  dest[i][j] = 0;
              }
          } else {
              if (neighbours == 3) {
                  dest[i][j] = 1;
              } else {
                  dest[i][j] = 0;
              }
          }
      }
    }
}

void  drawScreen(bool (*world)[8]);

void loop() {
  unsigned new_time = millis();

  if (50 < (new_time - last_time)) {
    update_world(source, dest);
    last_time = new_time;
    bool (*tmp)[8] = source;
    source = dest;
    dest = tmp;
  }

  drawScreen(source);
}

 void  drawScreen(bool (*world)[8])
 { 
   // Turn on each row in series
    for (byte i = 0; i < 8; i++)        // count next row
     {
        digitalWrite(rows[i], HIGH);    //initiate whole row
        for (byte a = 0; a < 8; a++)    // count next row
        {
          
          // if You set (~buffer2[i] >> a) then You will have positive
          digitalWrite(columns[a], world[i][a] ? 0x00 : 0x01); // initiate whole column   
          
          delayMicroseconds(100);       // uncoment deley for diferent speed of display
          //delayMicroseconds(1000);
          //delayMicroseconds(16000);
          //delay(10);
          //delay(100);
          
          digitalWrite(columns[a], 1);      // reset whole column
        }
        digitalWrite(rows[i], LOW);     // reset whole row
        // otherwise last row will intersect with next row
    }
}
// 
  /* this is siplest resemplation how for loop is working with each row.
    digitalWrite(COL_1, (~b >> 0) & 0x01); // Get the 1st bit: 10000000
    digitalWrite(COL_2, (~b >> 1) & 0x01); // Get the 2nd bit: 01000000
    digitalWrite(COL_3, (~b >> 2) & 0x01); // Get the 3rd bit: 00100000
    digitalWrite(COL_4, (~b >> 3) & 0x01); // Get the 4th bit: 00010000
    digitalWrite(COL_5, (~b >> 4) & 0x01); // Get the 5th bit: 00001000
    digitalWrite(COL_6, (~b >> 5) & 0x01); // Get the 6th bit: 00000100
    digitalWrite(COL_7, (~b >> 6) & 0x01); // Get the 7th bit: 00000010
    digitalWrite(COL_8, (~b >> 7) & 0x01); // Get the 8th bit: 00000001
}*/
