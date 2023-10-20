# Proteus_32Channel_IO_MCP23017_OLED
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <Adafruit_MCP23X17.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4 // Set your OLED reset pin
#define SCREEN_ADDRESS 0x3D // Use the correct address for your SSD1309 display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LED_PIN[] = {0, 1, 2, 3, 4, 5, 6, 7};
const int BUTTON_PIN[] = {8, 9, 10, 11, 12, 13, 14, 15};
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;

int channel = 0;  // Variable for channel (0-125)
int address = 0; // Variable for address (0-999)

void setup() {
  Serial.begin(9600);

  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();

  // Initialize the first MCP23X17 instance with address 0x20
  if (!mcp1.begin_I2C(0x20)) {
    Serial.println("Error initializing MCP1.");
    while (1);
  }

  // Initialize the second MCP23X17 instance with address 0x27
  if (!mcp2.begin_I2C(0x27)) {
    Serial.println("Error initializing MCP2.");
    while (1);
  }

  for (int i = 0; i < 8; i++) {
    mcp1.pinMode(LED_PIN[i], OUTPUT);
    mcp2.pinMode(LED_PIN[i], OUTPUT);
    mcp1.pinMode(BUTTON_PIN[i], INPUT_PULLUP);
    mcp2.pinMode(BUTTON_PIN[i], INPUT_PULLUP);
  }

  // Retrieve values from EEPROM
  channel = EEPROM.read(0);  // Read the channel value from EEPROM
  address = EEPROM.read(1);  // Read the address value from EEPROM

  // Your OLED display code here
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Channel: " + String(channel));
  display.println("Address: " + String(address));
  display.display();
  delay(2000);
}

void loop() {
  // Your main loop code here
  display.println("Channel: " + String(channel));
  display.println("Address: " + String(address));
  // Update the display as needed

  for (int i = 0; i < 8; i++) {
    if (mcp1.digitalRead(BUTTON_PIN[i]) == LOW) {
      mcp1.digitalWrite(LED_PIN[i], HIGH);
    } else {
      mcp1.digitalWrite(LED_PIN[i], LOW);
    }

    if (mcp2.digitalRead(BUTTON_PIN[i]) == LOW) {
      mcp2.digitalWrite(LED_PIN[i], HIGH);
    } else {
      mcp2.digitalWrite(LED_PIN[i], LOW);
    }
  }
}

void saveToEEPROM() {
  // Save the channel and address values to EEPROM
  EEPROM.write(0, channel);  // Write the channel value to EEPROM
  EEPROM.write(1, address);  // Write the address value to EEPROM
}
