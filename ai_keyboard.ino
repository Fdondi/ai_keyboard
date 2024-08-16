#define HID_CUSTOM_LAYOUT
#define LAYOUT_GERMAN_SWISS

#include <HID-Project.h>

class Debouncer {
  const int _debounceDelay;  // Debounce delay in milliseconds
  int _lastActivation;
public:
  Debouncer(int debounceDelay)
    : _debounceDelay(debounceDelay), _lastActivation(0) {}
  bool tooSoon() const {
    return millis() - _lastActivation < _debounceDelay;
  }
  void reset() {
    _lastActivation = millis();
  }
} debouncer(5000);

int stage = 0;

class Button {
  const int _pin_num;
public:
  const String content;
  Button(int pin_num, const String&& content)
    : _pin_num(pin_num), content(content) {
    pinMode(pin_num, INPUT_PULLUP);
  }
  void read(Button*& out) {
    if (digitalRead(_pin_num) == LOW) {
      Serial.print("Pressed: ");
      Serial.println(_pin_num);
      out = this;
    }
  }
};

Button prompt_buttons[] = { { 2, "Explain this in detail:" },
                            { 3, "Summarize to one or two paragraphs:" },
                            { 5, "Rewrite this to be more professional:" },
                            { 6, "Translate this to English:" },
                            { 9, "Write a poem about this:" } };

Button LLM_buttons[] = { { 11, "https://chat.openai.com/" },
                         { 12, "https://gemini.google.com/app" },
                         { 13, "https://claude.ai/new" } };

Button* active_prompt = nullptr;
Button* active_LLM = &LLM_buttons[0];  // There must always be a valid LLM activated.

void setup() {
  Keyboard.begin();
}

void loop() {
  // Check every time if the user switched the LLM to use, if so overwrite
  for (const Button& button : LLM_buttons)
    button.read(active_LLM);

  if (stage > 20) stage = 0;  // Failsafe. Sometimes "default" doesn't trigger.

  // We need to do it in stages becasue doing everything in the same loop was causing glitches.
  switch (stage) {
    case 0:  // nothing yet, check if we should activate
      if (debouncer.tooSoon()) return;
      active_prompt = nullptr;
      for (const Button& button : prompt_buttons)
        button.read(active_prompt);
      if (!active_prompt) return;
      Serial.println(F("valid keypress"));
      debouncer.reset();
      break;
    // Case > 0 -> Send the instructions
    case 1:
      Serial.println(F("Copy selected text"));
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('c');
      break;
    case 2:
      Serial.println(F("Open new browser tab"));
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('t');
      break;
    case 3:
      Serial.println(F("Navigate to the selected LLM"));
      Keyboard.print(active_LLM->content);
      Keyboard.press(KEY_RETURN);
      break;
    case 4:
      Serial.println(F("Add the prompt"));
      delay(2000);  // Wait for the page to load, adjust this delay as needed
      Keyboard.releaseAll();
      const String& prompt = active_prompt ? active_prompt->content : "ERROR no prompt selected";
      Keyboard.print(prompt);
      active_prompt = nullptr;
      break;
    case 5:
      Serial.println(F("add newline without sending message"));
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_ENTER);
      break;
    case 6:
      Serial.println(F("Paste the copied text"));
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('v');
      break;
    case 7:
      Serial.println(F("Send message to LLM"));
      Keyboard.press(KEY_RETURN);
      break;
    default:
      Serial.println(F("We ran out of valid stages; reset"));
      stage = 0;
      Serial.print(F("Reset done, stage is now: "));
      Serial.println(stage);
      return;
  }
  delay(100);
  Keyboard.releaseAll();
  Serial.print(F("Done stage:"));
  Serial.println(stage);
  stage += 1;
}
