#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Keyboard.h>
#include <HID-Project.h>
#include <Encoder.h>

const int SW_4 = 2;
const int SW_3 = 3;
const int SW_2 = 4;
const int SW_1 = 5;

const int ENC_SWITCH = 6;

const int SCL_PIN = 8;
const int SDA_PIN = 20;

Encoder encoder(7, 21);

int layer = 0;
long position;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void showMessage(String msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(msg);
  display.display();
}

void setup() {
  Serial.begin(9600);

  delay(500);

  Wire.setSCL(SCL_PIN);
  Wire.setSDA(SDA_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Failed to start");
    for(;;);
  }

  display.display();
  delay(2000);

  display.clearDisplay();

  Keyboard.begin();
  Consumer.begin();

  pinMode(SW_4, INPUT_PULLUP);
  pinMode(SW_3, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
  pinMode(SW_1, INPUT_PULLUP);

  pinMode(ENC_SWITCH, INPUT_PULLUP);

  // Initialize encoder position to avoid volume changes on startup
  position = encoder.read() / 4;
}

void loop() {

  // Encoder Volume Control
  long newPosition = encoder.read() / 4;

  if (newPosition != position) {

    if (newPosition > position) {
      Consumer.write(MEDIA_VOLUME_UP);
    }
    else if (newPosition < position) {
      Consumer.write(MEDIA_VOLUME_DOWN);
    }

    showMessage("Volume: " + String(newPosition));

    position = newPosition;
  }

  // SW_4 - Change Layer
  if (digitalRead(SW_4) == LOW) {

    while (digitalRead(SW_4) == LOW) {
      // Wait for release
    }

    if (layer != 2) {
      layer++;
    }
    else {
      layer = 0;
    }

    showMessage("Layer: " + String(layer));

    delay(20);
  }

  // SW_3
  if (digitalRead(SW_3) == LOW) {

    while (digitalRead(SW_3) == LOW) {
      // Wait for release
    }

    if (layer == 0) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('c');
      delay(100);
      Keyboard.releaseAll();

      showMessage("Copied!");
    }
    else if (layer == 1) {

    }
    else if (layer == 2) {

    }

    delay(20);
  }

  // SW_2
  if (digitalRead(SW_2) == LOW) {

    while (digitalRead(SW_2) == LOW) {
      // Wait for release
    }

    if (layer == 0) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('v');
      delay(100);
      Keyboard.releaseAll();

      showMessage("Pasted!");
    }
    else if (layer == 1) {

    }
    else if (layer == 2) {

    }

    delay(20);
  }

  // SW_1
  if (digitalRead(SW_1) == LOW) {

    while (digitalRead(SW_1) == LOW) {
      // Wait for release
    }

    if (layer == 0) {
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_TAB);
      delay(100);
      Keyboard.releaseAll();

      showMessage("Switched Tab!");
    }
    else if (layer == 1) {

    }
    else if (layer == 2) {

    }

    delay(20);
  }

  // Encoder Button - Mute
  if (digitalRead(ENC_SWITCH) == LOW) {

    while (digitalRead(ENC_SWITCH) == LOW) {
      // Wait for release
    }

    Consumer.write(MEDIA_VOLUME_MUTE);

    showMessage("Muted Audio!");

    delay(20);
  }
}

