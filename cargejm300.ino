#include <Gamebuino-Meta.h>

//position x and y
const byte y = 48;
byte x = 36;

//car parameters
const byte carHeight = 8;
const byte carWidth = 6;
const byte lineLeftX = 15;
const byte lineRightX = 65;

//car speed na acceleration
byte carSpeed = 0;
byte acceleration = 0;

//road draw line
short lineStartingPos = -20;

//enemy's car parameters
short enemyX = random(lineLeftX + 1, lineRightX - carWidth);
short enemy2X = random(lineLeftX + 1, lineRightX - carWidth);
short enemyY = - 16;
short enemy2Y = - 16;
short enemy2Direction = random(0, 2) ? -1 : 1;

//fuel
byte carFuel = 100;
const byte fuelDropMax = 20;
byte fuelDrop = fuelDropMax;
const byte fuelHeight = 5;
const byte fuelWidth = 5;
const byte fuelSpawn = 0;
short fuelY = - 200;
short fuelX = random(lineLeftX + 1, lineRightX - fuelWidth);

//car life
byte carLife = 3;

//enemy time spawn
const byte enemyTimeMax = 150;
byte enemyTime = enemyTimeMax;

//score
unsigned int score = 0;

//main menu
bool mainMenu = true;
const String menuOptions [] = { "NEW GAME", "HIGH SCORES", "CREDITS" };
const byte menuPositions [][2] = { {25, 20}, {25, 30}, {25, 40} };
byte selectedOption = 0;

//HIGH SCORES

//colors
int cred = 200;
int cgreen = 0;
int cblue = 0;

//car's
const uint8_t PlayerCarData[] = {
  6, 8,
  1, 0,
  0,
  0xFF,
  1,

  0x00, 0x88,0x00, 0x08,
  0x88, 0x80,0x88, 0x00,
  0x88, 0x80,0x00, 0x08,
  0x08, 0x88,0x80, 0x08,
  0x88, 0x80,0x80, 0x00,
  0x08, 0x80,0x88, 0x08,
};

Image PlayerCar = Image(PlayerCarData);

const uint8_t EnemyCarData[] = {
  6, 8,
  1, 0,
  0,
  0xFF,
  1,

  0x00, 0xdd,0x00, 0x0d,
  0xdd, 0xd0,0xdd, 0x00,
  0xdd, 0xd0,0xdd, 0x0d,
  0xdd, 0xdd,0xdd, 0xd0,
  0xdd, 0x0d,0xdd, 0xdd,
  0xdd, 0xd0,0x00, 0x0d,
};

Image EnemyCar = Image(EnemyCarData);

const uint8_t FuelData[] = {
  5, 5,
  1, 0,
  1,
  0xFF,
  1,

  0x90, 0x08,0x80, 0x08,
  0x08, 0x08,0x88, 0x80,
  0x88, 0x88,0x00, 0x88,0x0080,
};

Image Fuel = Image(FuelData);

// funkcja
byte subtract(byte value, byte sub) {
  if (byte(value - sub) > value) {
    return 0;
  } else {
    return value - sub;
  }
}

byte addition(byte value, byte add) {
  if (value + add <= 255) {
    return value + add;
  } else {
    return 255;
  }
}
short points(int value, int pts){
  return value + pts;
}

bool does_colide(short x1, short y1, short width1, short height1, short x2, short y2, short width2, short height2){
    return ((x1 >= x2 && x1 <= x2+width2) || (x1 + width1 >= x2 && x1 + width1 <= x2 + width2))
      && ((y1 >= y2 && y1 <= y2 + height2) || (y1 + height1 >= y2 && y1 + height1 <= y2 + height2));
}


void setup() {
  // put your setup code here, to run once:
  gb.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  while(!gb.update());
  gb.display.clear();

//MAIN MENU
if(mainMenu){
  while (1) {
    while(!gb.update());
    gb.display.clear();
    gb.lights.clear();

    if ((gb.frameCount % 15) == 0){
      gb.lights.drawPixel(0,0, BLUE);
      gb.lights.drawPixel(1,3, BLUE);
      gb.lights.drawPixel(0,3, RED);
      gb.lights.drawPixel(1,0, RED);
    }

    for(short i = 0 ; i < 3 ; i++){
      gb.display.setCursor(menuPositions[i][0], menuPositions[i][1]);
      if(selectedOption == i){
        gb.display.setColor(BLUE);
      }
      else{
        gb.display.setColor(WHITE);
      }
      gb.display.print(menuOptions [i]);

    }
    if(gb.buttons.pressed(BUTTON_DOWN)){
      selectedOption ++;
      if(selectedOption > 2){
        selectedOption = 0;
      }
    }
    if(gb.buttons.pressed(BUTTON_UP)){
      selectedOption --;
       if(selectedOption == 255){
        selectedOption = 2;
       }
    }
    if(gb.buttons.pressed(BUTTON_A)){
      if(selectedOption == 0){
        mainMenu = false;
        break;
      }
    }
    if(gb.buttons.repeat(BUTTON_A, 0)){
      if(selectedOption == 1){
        gb.display.setColor(BLACK);
        gb.display.fillRect(5, 5, 80, 50);
        gb.display.setColor(WHITE);
        gb.display.setCursor(20, 2);
        gb.display.print("HIGH SCORES");
        gb.display.setCursor(10, 15);
        gb.display.setColor(YELLOW);
        gb.display.print("1.---");
        gb.display.setCursor(10, 25);
        gb.display.setColor(GRAY);
        gb.display.print("2.---");
        gb.display.setCursor(10, 35);
        gb.display.setColor(BROWN);
        gb.display.print("3.---");
        gb.display.setCursor(10, 45);
        gb.display.setColor(WHITE);
        gb.display.print("4.---");
        gb.display.setCursor(10, 55);
        gb.display.print("5.---");
      }
    }
  }
}

 //ROAD AND LINES COLOR
  if (cred = 200){
    cgreen += 8;
  }
  if (cgreen > 255){
    cblue += 10;
  }
  if (cblue > 255){
    cblue = 0;
    cgreen = 0;
    cred = 150;
  }
  Color road = gb.createColor(cred,cgreen,cblue);
  gb.display.setColor(road);
  gb.display.drawLine(lineLeftX, 0, lineLeftX, 64);
  gb.display.drawLine(lineRightX, 0, lineRightX, 64);
  gb.lights.drawPixel(0, 0, road);
  gb.lights.drawPixel(0, 1, road);
  gb.lights.drawPixel(0, 2, road);
  gb.lights.drawPixel(0, 3, road);
  gb.lights.drawPixel(1, 0, road);
  gb.lights.drawPixel(1, 1, road);
  gb.lights.drawPixel(1, 2, road);
  gb.lights.drawPixel(1, 3, road);
  gb.display.drawImage(x, y, PlayerCar);
  gb.display.setColor(WHITE);
  gb.display.print("SPD\n");
  gb.display.printf("%d\n", carSpeed);
  gb.display.printf("FUEL\n");
  gb.display.printf("%d\n", carFuel);
  gb.display.print("LIFE\n");
  gb.display.printf("%d\n", carLife);
  gb.display.print("SCR\n");
  gb.display.printf("%d\n", score);
  gb.display.drawImage(enemyX, enemyY, EnemyCar);
  gb.display.setColor(DARKBLUE);
  gb.display.fillRect(enemy2X, enemy2Y, carWidth, carHeight);
  gb.display.drawImage(fuelX, fuelY, Fuel);

  // przerywana linia
  for(byte i = 0; i <= 5; i++){
    gb.display.setColor(road);
    gb.display.drawLine(40, (i * 16) + lineStartingPos, 40, (i * 16) + (lineStartingPos + 8));
  }
  //ruch lini
  if(carSpeed > 0){
    acceleration = carSpeed / 50 + 1;
    lineStartingPos+= acceleration;
    if(lineStartingPos >= -4){
      lineStartingPos = -20;
    }
  }
  else{
    acceleration = 0;
  }

  if(gb.buttons.repeat(BUTTON_LEFT, 0)) {
    if(x > lineLeftX + 1){
      x = subtract(x, 1);
    }
  } else if(gb.buttons.repeat(BUTTON_RIGHT, 0)) {
    if(x < lineRightX - carWidth){
      x = addition(x, 1);
    }
  }

  if(carFuel > 0){
    if(gb.buttons.repeat(BUTTON_A, 0)){
      carSpeed = addition(carSpeed, 4);
    }
  }

  if(carSpeed > 0){
    carSpeed = subtract(carSpeed, 1);

    if(fuelDrop > 0){
      fuelDrop = subtract(fuelDrop, carSpeed / 150 + 1);
    }
    else {
      if(carFuel > 0){
        carFuel = subtract(carFuel, 1);
        fuelDrop = fuelDropMax;
      }
    }
    if(enemyTime > 0){
      enemyTime = subtract(enemyTime, carSpeed / 150 + 1);
    }
  }

  // WARLORDS OF THE STREET
  if(acceleration == 0) {
    enemyY += -2;
    fuelY += -2;
  }
  else{
    enemyY+= acceleration / 5 + 2;
    fuelY += acceleration / 5 + 2;
  }
  if (enemyY >= 63){
    enemyY = -16;
    enemyX = random(lineLeftX + 1, lineRightX - carWidth);
  }
  //FUEL
  if((gb.frameCount % 150) == 0){
    fuelY = - 100;
    fuelX = random(lineLeftX + 1, lineRightX - fuelWidth);
  }
  if((does_colide(x+1, y+1, carWidth -1, carHeight -1, fuelX +1, fuelY +1, carWidth -1, carHeight -1))){
    carFuel = addition(carFuel, 10);
    fuelY = - 100;
    fuelX = random(lineLeftX + 1, lineRightX - fuelWidth);
  }

  if(enemyTime == 0){
     if(acceleration == 0) {
       enemy2Y += -2;
     }
     else{
        enemy2Y+= acceleration / 5 + 2;
     }
     if(enemy2Y >= 65){
       enemy2Y = -16;
       enemy2X = random(lineLeftX + 1, lineRightX - carWidth);
       enemyTime = enemyTimeMax;
       enemy2Direction = random(0, 2) ? -1 : 1;
     }
     if(carSpeed > 0){
       enemy2X += enemy2Direction;
     }
  }
  if(enemy2X == lineLeftX + 1 || enemy2X == lineRightX - carWidth){
    enemy2Direction *= -1;
  }

  // HIT'n'DESTROY
  if((does_colide(x+1, y+1, carWidth-1, carHeight-1, enemyX+1, enemyY+1, carWidth-1, carHeight-1))
    ||(does_colide(x+1, y+1, carWidth-1, carHeight-1, enemy2X+1, enemy2Y+1, carWidth-1, carHeight-1)) ) {
    carLife = subtract(carLife, 1);
    carSpeed = 0;
    carFuel = subtract(carFuel, 20);
    enemyTime = enemyTimeMax;
    enemyY = -16;
    enemyX = random(lineLeftX + 1, lineRightX - carWidth);
    enemy2Y = -16;
    enemy2X = random(lineLeftX + 1, lineRightX - carWidth);
    if(score > 14){
      score -= 14;
    }
    else{
      score = 0;
    }

  }
  if(carLife == 0 || (carFuel == 0 && carSpeed == 0)){
    gb.display.clear();
    gb.display.setCursor(20, 20);
    gb.display.print("GAME OVAH!\n");
    gb.display.setCursor(25, 30);
    gb.display.print("u succ");
    gb.display.setCursor(0, 50);
    gb.display.print("Press 'menu' to RUN!");
    if(gb.buttons.pressed(BUTTON_MENU)){
      carSpeed = 0;
      x = 36;
      carFuel = 100;
      fuelDrop = fuelDropMax;
      carLife = 3;
      enemyY = -16;
      enemyX = random(lineLeftX + 1, lineRightX - carWidth);
      enemy2Y = -16;
      enemy2X = random(lineLeftX + 1, lineRightX - carWidth);
      score = 0;
    }
  }
  if((enemyY> y && enemyX <= x + 20 && x <= enemyX + 20) || (enemy2Y> y && enemy2X <= x + 20 && x <= enemy2X + 20) ){
    score = points(score, 1);
  }

}
