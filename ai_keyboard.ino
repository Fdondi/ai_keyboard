#define HID_CUSTOM_LAYOUT
#define LAYOUT_GERMAN_SWISS

#include <HID-Project.h>

const int explainKey = 2;

const int debounceDelay = 5000; // Debounce delay in milliseconds

int stage = 0;

unsigned long lastActivation = 0;

void setup() {
  pinMode(explainKey, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  switch (stage) {
    case 0:  // nothing yet, check if we should activate
      // if button not pressed, or too soon, leave
      if ((digitalRead(explainKey) != LOW) || (millis() - lastActivation < debounceDelay)) return;
      // If we didn't leave, we have a valid keypress
      lastActivation = millis();  // reset debounce
      break;
    // Case > 0 -> Send the instructions
    case 1:  // Copy selected text
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('c');
      break;
    case 2:  // Open new browser tab
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('t');
      break;
    case 3:  // Navigate to ChatGPT
      Keyboard.print("https://chat.openai.com/");
      Keyboard.press(KEY_RETURN);
      break;
    case 4:         // Add the prompt
      delay(2000);  // Wait for the page to load, adjust this delay as needed
      Keyboard.releaseAll();
      Keyboard.print("Explain this text in detail:");
    case 5: // add newline without sending message
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_ENTER);
      break;
    case 6:  // Paste the copied text
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('v');
      Keyboard.press(KEY_RETURN);
      break;
    default:
      // We ran out of valid stages; reset
      stage = 0;
      return;
  }
  delay(100);
  Keyboard.releaseAll();
  Serial.print("Done stage:");
  Serial.println(stage);
  stage += 1;
}
