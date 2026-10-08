#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const int I2C_SDA_PIN = 6;
const int I2C_SCL_PIN = 7;
const int DHT_PIN = 3;
const int BUTTON_ONE_PIN = 4;
const int BUTTON_TWO_PIN = 5;
const bool USE_DHT11 = true;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_MPU6050 mpu;
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

bool mpuReady = false;

int petJoy = 50;
int petEnergy = 50;
int petFullness = 50;

enum ReactionType {
  NONE,
  NAP_REACTION,
  RUN_REACTION,
  JUMP_REACTION,
  HEART_REACTION
};

struct MenuItem {
  const char* label;
  int joyChange;
  int energyChange;
  int fullnessChange;
  ReactionType reaction;
};

const MenuItem MENU_ITEMS[] = {
  {"NAP",   1,  18, -4, NAP_REACTION},
  {"PLAY", 12,  -9, -5, RUN_REACTION},
  {"FEED",  3,   2, 18, JUMP_REACTION},
  {"PET",   7,   0,  0, HEART_REACTION}
};

const int MENU_BOX_X[] = {48, 94, 48, 2};
const int MENU_BOX_Y[] = {2, 26, 50, 26};
const int MENU_BOX_W = 32;
const int MENU_BOX_H = 12;

bool menuOpen = false;
bool showStats = false;
int selectedSector = 0;
unsigned long reactionStartTime = 0;
ReactionType currentReaction = NONE;

const int PET_HOME_Y = 24;
int petX = 48;
int petY = PET_HOME_Y;
unsigned long lastMoveTime = 0;

const int PET_SPRITE_WIDTH = 32;
const int PET_SPRITE_HEIGHT = 32;
const uint8_t PROGMEM PET_SPRITE[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x40, 0x02, 0x00,
  0x00, 0x80, 0x01, 0x00, 0x01, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x40, 0x04, 0x09, 0x10, 0x20,
  0x08, 0x06, 0x60, 0x10, 0x08, 0x00, 0x00, 0x10, 0x10, 0x11, 0x88, 0x08, 0x10, 0x11, 0x88, 0x08,
  0x10, 0x00, 0x00, 0x08, 0x10, 0x44, 0x22, 0x08, 0x08, 0x22, 0x44, 0x10, 0x08, 0x11, 0x88, 0x10,
  0x04, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00,
  0x00, 0x40, 0x02, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x11, 0x88, 0x00,
  0x00, 0x11, 0x88, 0x00, 0x00, 0x11, 0x88, 0x00, 0x00, 0x11, 0x88, 0x00, 0x00, 0x1f, 0xf8, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void drawSparkle(int x, int y) {
  display.drawPixel(x, y - 2, SSD1306_WHITE);
  display.drawPixel(x - 1, y - 1, SSD1306_WHITE);
  display.drawPixel(x + 1, y - 1, SSD1306_WHITE);
  display.drawPixel(x - 2, y, SSD1306_WHITE);
  display.drawPixel(x, y, SSD1306_WHITE);
  display.drawPixel(x + 2, y, SSD1306_WHITE);
  display.drawPixel(x - 1, y + 1, SSD1306_WHITE);
  display.drawPixel(x + 1, y + 1, SSD1306_WHITE);
  display.drawPixel(x, y + 2, SSD1306_WHITE);
}

void drawHeart(int x, int y) {
  display.drawPixel(x, y, SSD1306_WHITE);
  display.drawPixel(x - 1, y - 1, SSD1306_WHITE);
  display.drawPixel(x + 1, y - 1, SSD1306_WHITE);
  display.drawPixel(x - 2, y - 2, SSD1306_WHITE);
  display.drawPixel(x, y - 2, SSD1306_WHITE);
  display.drawPixel(x + 2, y - 2, SSD1306_WHITE);
}

bool isNapping() {
  return (currentReaction == NAP_REACTION);
}

void startReaction(ReactionType reaction, unsigned long now) {
  currentReaction = reaction;
  reactionStartTime = now;
}

void drawPet() {
  unsigned long now = millis();
  display.drawBitmap(petX, petY, PET_SPRITE, PET_SPRITE_WIDTH, PET_SPRITE_HEIGHT, SSD1306_WHITE);

  if (!isNapping() && (now / 250) % 2 == 0) {
    drawSparkle(petX - 4, petY + 18);
  }
}

void drawMenu() {
  for (int i = 0; i < 4; i++) {
    int labelWidth = strlen(MENU_ITEMS[i].label) * 6;
    int textX = MENU_BOX_X[i] + (MENU_BOX_W - labelWidth) / 2;
    display.drawRect(MENU_BOX_X[i], MENU_BOX_Y[i], MENU_BOX_W, MENU_BOX_H, SSD1306_WHITE);
    display.setCursor(textX, MENU_BOX_Y[i] + 2);
    display.print(MENU_ITEMS[i].label);
  }
  display.fillRect(MENU_BOX_X[selectedSector], MENU_BOX_Y[selectedSector], MENU_BOX_W, MENU_BOX_H, SSD1306_INVERSE);
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_ONE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_TWO_PIN, INPUT_PULLUP);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found"));
    for (;;) delay(1000);
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.display();

  mpuReady = mpu.begin();
  if (mpuReady) {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  } else {
    Serial.println(F("MPU6050 not found"));
  }

  if (USE_DHT11) {
    dht.begin();
  }
}

void loop() {
  unsigned long now = millis();

  sensors_event_t a, g, temp;
  a.acceleration.x = 0;
  a.acceleration.y = 0;
  a.acceleration.z = 0;
  if (mpuReady) {
    mpu.getEvent(&a, &g, &temp);
  }

  static bool lastBtn1 = HIGH;
  static bool lastBtn2 = HIGH;
  bool btn1 = digitalRead(BUTTON_ONE_PIN);
  bool btn2 = digitalRead(BUTTON_TWO_PIN);

  float shakeMagnitude = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z);
  if (shakeMagnitude > 18.0 && (currentReaction == NONE || currentReaction == NAP_REACTION) && !menuOpen) {
    startReaction(JUMP_REACTION, now);
  }

  if (lastBtn1 == HIGH && btn1 == LOW) {
    if (!menuOpen && currentReaction == NONE) {
      menuOpen = true;
      showStats = false;
      selectedSector = 0;
    } else if (menuOpen) {
      MenuItem choice = MENU_ITEMS[selectedSector];
      petJoy = constrain(petJoy + choice.joyChange, 0, 100);
      petEnergy = constrain(petEnergy + choice.energyChange, 0, 100);
      petFullness = constrain(petFullness + choice.fullnessChange, 0, 100);

      startReaction(choice.reaction, now);
      menuOpen = false;
    }
    delay(50);
  }

  if (lastBtn2 == HIGH && btn2 == LOW) {
    if (!menuOpen) {
      showStats = !showStats;
    }
    delay(50);
  }

  lastBtn1 = btn1;
  lastBtn2 = btn2;

  if (menuOpen) {
    if (a.acceleration.y > 3.0)        selectedSector = 1;
    else if (a.acceleration.y < -3.0)  selectedSector = 3;
    else if (a.acceleration.x < -3.0)  selectedSector = 2;
    else if (a.acceleration.x > 3.0)   selectedSector = 0;
  }

  if (currentReaction == NONE && !menuOpen && (now - lastMoveTime > 2000)) {
    lastMoveTime = now;
    petX += random(-8, 9);
    if (petX < 0) petX = 0;
    if (petX > SCREEN_WIDTH - PET_SPRITE_WIDTH) petX = SCREEN_WIDTH - PET_SPRITE_WIDTH;
  }

  if (currentReaction != NONE && now - reactionStartTime > 4000) {
    currentReaction = NONE;
    petY = PET_HOME_Y;
  }

  display.clearDisplay();

  if (showStats) {
    display.setCursor(0, 0);
    display.println(F("--- STARBIE STATS ---"));
    display.print(F("JOY: ")); display.println(petJoy);
    display.print(F("ENG: ")); display.println(petEnergy);
    display.print(F("FUL: ")); display.println(petFullness);

    if (USE_DHT11) {
      float t = dht.readTemperature();
      display.print(F("TEMP: "));
      if (isnan(t)) {
        display.println(F("-- C"));
      } else {
        display.print(t);
        display.println(F(" C"));
      }
    }
  }
  else if (menuOpen) {
    drawMenu();
    drawPet();
  }
  else if (currentReaction != NONE) {
    unsigned long elapsed = now - reactionStartTime;

    if (currentReaction == NAP_REACTION) {
      if ((elapsed / 500) % 2 == 0) {
        display.setCursor(petX + 34, petY);
        display.print(F("z"));
        display.setCursor(petX + 40, petY - 6);
        display.print(F("Z"));
      }
    }
    else if (currentReaction == HEART_REACTION) {
      int heartY = petY - (elapsed / 100);
      if (heartY > 0) drawHeart(petX + 16, heartY);
    }
    else if (currentReaction == JUMP_REACTION) {
      petY = PET_HOME_Y - (int)(sin((elapsed % 1000) * 3.14159 / 1000.0) * 10);
    }
    else if (currentReaction == RUN_REACTION) {
      int cycle = elapsed % 2000;
      if (cycle < 1000) {
        petX = cycle * (SCREEN_WIDTH - PET_SPRITE_WIDTH) / 1000;
      } else {
        petX = (SCREEN_WIDTH - PET_SPRITE_WIDTH) - ((cycle - 1000) * (SCREEN_WIDTH - PET_SPRITE_WIDTH) / 1000);
      }
      if ((elapsed / 200) % 2 == 0) drawHeart(petX + random(-4, 36), petY - 4);
    }
    drawPet();
  }
  else {
    drawPet();
  }

  display.display();
}
