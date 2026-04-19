#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define RESET_BTN  7
#define SAVE_BTN   8
#define TOGGLE_BTN 9
#define EXEC_BTN   10

int s1=0, s2=0, s3=0, s4=0, s5=0;

#define MAX_POSES 20
int saved[MAX_POSES][5];
int saveIndex = 0;

bool showSaved = false;
int  viewIndex = 0;

bool   saveBtnHeld  = false;
unsigned long saveStart = 0;
bool   saveDone     = false;
bool   deleteDone   = false;

bool   toggleHeld       = false;
unsigned long toggleStart = 0;
bool   toggleModeDone   = false;
bool   toggleClickArmed = false;

bool   execHeld     = false;
unsigned long execStart = 0;
bool   execDone     = false;

String statusMsg    = "";
unsigned long statusUntil = 0;
unsigned long statusDuration = 0;

// ── helpers ──────────────────────────────────────────
void showStatus(String msg, int ms = 1500) {
  statusMsg      = msg;
  statusUntil    = millis() + ms;
  statusDuration = ms;
}

void drawProgressBar(int x, int y, int w, int h, int val, int maxVal) {
  display.drawRect(x, y, w, h, WHITE);
  int fill = map(val, 0, maxVal, 0, w - 2);
  if (fill > 0)
    display.fillRect(x + 1, y + 1, fill, h - 2, WHITE);
}

void printVal(int x) {
  if (x < 100) display.print(" ");
  if (x < 10)  display.print(" ");
  display.print(x);
}

// ── EEPROM ───────────────────────────────────────────
void loadFromEEPROM() {
  saveIndex = EEPROM.read(0);
  if (saveIndex > MAX_POSES) saveIndex = 0;
  for (int i = 0; i < saveIndex; i++)
    for (int j = 0; j < 5; j++)
      saved[i][j] = EEPROM.read(1 + i*5 + j);
}

void savePose() {
  if (saveIndex < MAX_POSES) {
    saved[saveIndex][0] = s1;
    saved[saveIndex][1] = s2;
    saved[saveIndex][2] = s3;
    saved[saveIndex][3] = s4;
    saved[saveIndex][4] = s5;
    for (int j = 0; j < 5; j++)
      EEPROM.write(1 + saveIndex*5 + j, saved[saveIndex][j]);
    saveIndex++;
    EEPROM.write(0, saveIndex);
    showStatus("Saved!  ID:" + String(saveIndex), 1500);
  } else {
    showStatus("Memory Full!", 1500);
  }
}

void deleteAll() {
  saveIndex = 0; viewIndex = 0;
  EEPROM.write(0, 0);
  showSaved = false;
  showStatus("All Deleted!", 2000);
}

// ── Execute with per-servo display ───────────────────
void executeID(int id) {
  int vals[5] = {
    saved[id][0], saved[id][1], saved[id][2],
    saved[id][3], saved[id][4]
  };

  for (int s = 0; s < 5; s++) {

    // Draw executing screen
    display.clearDisplay();
    display.setTextSize(1);

    // Header
    display.setCursor(0, 0);
    display.print("EXECUTING  ");
    display.print("ID:");
    display.print(id + 1);
    display.print("/");
    display.println(saveIndex);
    display.drawLine(0, 10, 128, 10, WHITE);

    // Current servo highlight
    display.setCursor(0, 14);
    display.print("Moving: S");
    display.println(s + 1);

    display.setCursor(0, 24);
    display.print("Angle: ");
    display.println(vals[s]);

    // Progress bar
    drawProgressBar(0, 34, 128, 6, s, 5);

    // Step tracker at bottom
    display.setCursor(0, 46);
    for (int i = 0; i < 5; i++) {
      display.print("S"); display.print(i + 1);
      if      (i < s)  display.print("+ ");
      else if (i == s) display.print("> ");
      else             display.print("- ");
    }

    display.display();

    // Send this servo
    Serial1.print("EXEC,");
    Serial1.print(vals[0]); Serial1.print(",");
    Serial1.print(vals[1]); Serial1.print(",");
    Serial1.print(vals[2]); Serial1.print(",");
    Serial1.print(vals[3]); Serial1.print(",");
    Serial1.println(vals[4]);

    delay(1200);
  }

  showStatus("Done  ID:" + String(id + 1), 1500);
}

// ============================================================
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(RESET_BTN,  INPUT_PULLUP);
  pinMode(SAVE_BTN,   INPUT_PULLUP);
  pinMode(TOGGLE_BTN, INPUT_PULLUP);
  pinMode(EXEC_BTN,   INPUT_PULLUP);

  loadFromEEPROM();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.println("SYSTEM READY");
  display.display();
  delay(1500);
}

// ============================================================
void loop() {

  unsigned long now = millis();

  // ── RESET ─────────────────────────────────────────
  if (digitalRead(RESET_BTN) == LOW) {
    Serial1.println("RESET");
    showStatus("Reset!", 1000);
    delay(300);
    return;
  }

  // ── SAVE BTN: 2s = save, 10s = delete all ─────────
  if (digitalRead(SAVE_BTN) == LOW) {
    if (!saveBtnHeld) {
      saveBtnHeld = true; saveStart = now;
      saveDone = false; deleteDone = false;
    }
    unsigned long held = now - saveStart;
    if (held >= 10000 && !deleteDone) {
      deleteAll(); deleteDone = true; saveDone = true;
    } else if (held >= 2000 && !saveDone) {
      savePose(); saveDone = true;
    }
  } else {
    saveBtnHeld = false; saveDone = false; deleteDone = false;
  }

  // ── TOGGLE BTN: 5s = mode, tap = scroll IDs ───────
  if (digitalRead(TOGGLE_BTN) == LOW) {
    if (!toggleHeld) {
      toggleHeld = true; toggleStart = now;
      toggleModeDone = false; toggleClickArmed = true;
    }
    if ((now - toggleStart) >= 5000 && !toggleModeDone) {
      showSaved = !showSaved;
      toggleModeDone = true; toggleClickArmed = false;
      showStatus(showSaved ? "Saved Mode" : "Live Mode", 1200);
    }
  } else {
    if (toggleHeld) {
      unsigned long heldFor = now - toggleStart;
      if (toggleClickArmed && heldFor < 500 && showSaved && saveIndex > 0) {
        viewIndex = (viewIndex + 1) % saveIndex;
        showStatus("ID: " + String(viewIndex + 1) + " / " + String(saveIndex), 800);
      }
    }
    toggleHeld = false; toggleModeDone = false; toggleClickArmed = false;
  }

  // ── EXEC BTN: 2s hold in saved mode ───────────────
  if (digitalRead(EXEC_BTN) == LOW) {
    if (!execHeld) { execHeld = true; execStart = now; execDone = false; }
    if ((now - execStart) >= 2000 && !execDone && showSaved && saveIndex > 0) {
      execDone = true;
      executeID(viewIndex);
    }
  } else {
    execHeld = false; execDone = false;
  }

  // ── JOYSTICK ──────────────────────────────────────
  int j1x = analogRead(A0), j1y = analogRead(A1);
  int j2x = analogRead(A2), j2y = analogRead(A3);
  int j3  = analogRead(A4);

  Serial1.print(j1x); Serial1.print(",");
  Serial1.print(j1y); Serial1.print(",");
  Serial1.print(j2x); Serial1.print(",");
  Serial1.print(j2y); Serial1.print(",");
  Serial1.println(j3);

  // ── FEEDBACK ──────────────────────────────────────
  if (Serial1.available()) {
    String fb = Serial1.readStringUntil('\n');
    fb.trim();
    if (fb.startsWith("FB"))
      sscanf(fb.c_str(), "FB,%d,%d,%d,%d,%d", &s1,&s2,&s3,&s4,&s5);
  }

  // ── DISPLAY ───────────────────────────────────────
  display.clearDisplay();
  display.setTextSize(1);

  // Status overlay with draining bar
  if (now < statusUntil) {
    display.setCursor(20, 18);
    display.setTextSize(1);
    display.println(statusMsg);
    int remaining = (int)(statusUntil - now);
    int barW = map(remaining, 0, statusDuration, 0, 110);
    drawProgressBar(9, 36, 110, 5, barW, 110);
    display.display();
    delay(50);
    return;
  }

  // ── Live Mode ─────────────────────────────────────
  if (!showSaved) {
    display.setCursor(0, 0);
    display.print("LIVE");

    // joystick active hint
    bool moving = abs(j1x-512)>60||abs(j1y-512)>60||abs(j2x-512)>60||abs(j2y-512)>60||abs(j3-512)>60;
    display.setCursor(50, 0);
    display.print(moving ? "moving" : "idle  ");

    display.drawLine(0, 10, 128, 10, WHITE);

    display.setCursor(0,  13); display.print("S1:"); printVal(s1);
    drawProgressBar(42, 13, 34, 4, s1, 180);

    display.setCursor(80, 13); display.print("S2:"); printVal(s2);

    display.setCursor(0,  26); display.print("S3:"); printVal(s3);
    drawProgressBar(42, 26, 34, 4, s3, 180);

    display.setCursor(80, 26); display.print("S4:"); printVal(s4);

    display.setCursor(0,  39); display.print("S5:"); printVal(s5);
    drawProgressBar(42, 39, 34, 4, s5, 180);

  // ── Saved Mode ────────────────────────────────────
  } else {
    display.setCursor(0, 0);
    display.print("SAVED ");

    // ID badge  [3/7]
    String idStr = "[" + String(viewIndex+1) + "/" + String(saveIndex) + "]";
    display.setCursor(128 - idStr.length()*6, 0);
    display.print(idStr);

    display.drawLine(0, 10, 128, 10, WHITE);

    int d[5] = {
      saved[viewIndex][0], saved[viewIndex][1], saved[viewIndex][2],
      saved[viewIndex][3], saved[viewIndex][4]
    };

    display.setCursor(0,  13); display.print("S1:"); printVal(d[0]);
    drawProgressBar(42, 13, 34, 4, d[0], 180);

    display.setCursor(80, 13); display.print("S2:"); printVal(d[1]);

    display.setCursor(0,  26); display.print("S3:"); printVal(d[2]);
    drawProgressBar(42, 26, 34, 4, d[2], 180);

    display.setCursor(80, 26); display.print("S4:"); printVal(d[3]);

    display.setCursor(0,  39); display.print("S5:"); printVal(d[4]);
    drawProgressBar(42, 39, 34, 4, d[4], 180);

    // hint at bottom
    display.setCursor(0, 52);
    display.print("tap=next  hold=exec");
  }

  display.display();
  delay(50);
}
