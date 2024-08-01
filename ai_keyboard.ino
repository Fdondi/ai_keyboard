#define HID_CUSTOM_LAYOUT
#define LAYOUT_GERMAN_SWISS

#include <HID-Project.h>

const int explainKey = 2;

const int debounceDelay = 5000; // Debounce delay in milliseconds

unsigned long lastActivation = 0;

void setup() {
  pinMode(explainKey, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  // if it's the first time we see the button pressed
  if ((digitalRead(explainKey) == LOW) && (millis() - lastActivation > debounceDelay)) {
    lastActivation = millis(); // reset debounce 
    // Send the keypresses
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('c'); // Copy selected text
    delay(100);
    Keyboard.releaseAll();
    delay(100);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('t'); // Open new browser tab
    delay(100);
    Keyboard.releaseAll();
    delay(100);
    Keyboard.print("https://chat.openai.com/"); // Navigate to ChatGPT
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
    delay(2000); // Wait for the page to load, adjust this delay as needed
    Keyboard.releaseAll();
    Keyboard.print("Explain this text in detail."); // Add the prompt
    delay(100);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('v'); // Paste the copied text
  }
}
