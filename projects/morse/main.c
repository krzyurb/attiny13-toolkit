#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include <attiny13.h>

uint8_t letter;

bool button_pressed;
bool button_pressed_prev = 0;

#define INPUT_BUTTON PB4
#define MODE_INPUT_BUTTON PB3

#define INPUT_LED_1 PB0
#define OUTPUT_LED PB1

#define COUNTER TCNT0
#define DEBOUNCE_DELAY 20

// TODO: can we use register directly instead of this variable?
volatile int8_t i_overflowed = 0;
int8_t overflowed;
int8_t maxOverflowed;

// 0 -> input
// 1 -> output
volatile int8_t mode = 0;

volatile uint8_t message[40];
volatile uint8_t messageLength = 0;

void addLetterToMessage(uint8_t letter) {
  message[messageLength] = letter;
  messageLength++;
}


uint8_t getLastLetterOfMessage()
{
  return message[messageLength - 1];
}

int8_t currentLetterIndex = 0;
int8_t currentLetterMaskIndex = 0;

ISR(TIM0_OVF_vect)
{

  // Input overflow
  if(i_overflowed > 12) {
    i_overflowed = 0;
  } else {
    i_overflowed++;
  }

  // Skip for input mode
  if (mode == 0) {
    return;
  }

  // Skip for empty message
  if (messageLength == 0)
  {
    digitalWrite(OUTPUT_LED, 0);
    return;
  }

  // Check if we finished message
  if (currentLetterIndex >= messageLength)
  {
    currentLetterIndex = 0;
    currentLetterMaskIndex = 0;
  }

  // Take letter
  uint8_t letter = message[currentLetterIndex];
  uint8_t letterLength = (letter >> 5) & 0b00000111;

  if (letterLength == 0 && overflowed < maxOverflowed)
  {
    maxOverflowed = 8;
    overflowed++;
    return;
  }

  // Take current bit of letter
  uint8_t bit = (letter >> currentLetterMaskIndex) & 1;

  if (currentLetterMaskIndex == 5 || currentLetterMaskIndex == letterLength)
  {
    maxOverflowed = 5;
  } else

    // Ustaw LED
    if (bit)
    {
      if (overflowed == 0)
      {
        maxOverflowed = 7;
      }

      if (overflowed <= 5)
      {
        digitalWrite(OUTPUT_LED, 1);
      }
      else
      {
        digitalWrite(OUTPUT_LED, 0);
      }
    }
    else
    {
      if (overflowed == 0)
      {
        maxOverflowed = 3;
      }

      if (overflowed <= 1)
      {
        digitalWrite(OUTPUT_LED, 1);
      }
      else
      {
        digitalWrite(OUTPUT_LED, 0);
      }
    }

    // Przejdź do następnego bitu
    if (overflowed >= maxOverflowed)
    {
      currentLetterMaskIndex++;
      overflowed = 0;
    } else
  {
    overflowed++;
  }

  if (currentLetterMaskIndex > 5 || currentLetterMaskIndex > letterLength)
  {
    currentLetterMaskIndex = 0;
    currentLetterIndex++;
  }
}

void init() {
  addLetterToMessage(0b00000000); // Empty char for beginning

  // init overvlow to zero
  overflowed = 0;
  i_overflowed = 0;

  button_pressed = 0;
  button_pressed_prev = 0;

  // set input button
  pinMode(INPUT_BUTTON, 1);
  pinMode(MODE_INPUT_BUTTON, 1);

  // set output LED
  pinMode(INPUT_LED_1, 0);
  pinMode(OUTPUT_LED, 0);

  // set timer
  TCNT0 = 0;

  // enable timer with prescaler 1024
  TCCR0B |= (1 << CS02) | (1 << CS00);

  // Włącz przerwanie od przepełnienia
  TIMSK0 |= (1 << TOIE0);

  // enable interrupts
  sei();
}

int main(void) {
  init();

  uint8_t newLetter = 0b00000000;
  uint8_t newLetterIndex = 0;
  uint8_t long_press = 0;

  while(1) {
    _delay_ms(DEBOUNCE_DELAY);

    // go into OUTPUT mode
    if (digitalRead(MODE_INPUT_BUTTON))
    {
      mode = 1;
    }

    if (mode == 0) {
      // INPUT button handler

      // Maximum length of letter
      if (newLetterIndex > 4)
      {
        newLetter |= ((newLetterIndex) & 0b00000111) << 5;
        addLetterToMessage(newLetter);
        newLetter = 0b00000000;
        newLetterIndex = 0;
        digitalWrite(OUTPUT_LED, 1);
      }

      if (digitalRead(INPUT_BUTTON))
      {
        // Button is pressed
        button_pressed = 1;

        digitalWrite(OUTPUT_LED, 0);

        if (button_pressed_prev == 0)
        {
          TCNT0 = 0;
          i_overflowed = 0;
          button_pressed_prev = 1;
          digitalWrite(INPUT_LED_1, 0);
        }

        if (i_overflowed > 0)
        {
          long_press = 1;
          digitalWrite(INPUT_LED_1, 1);
        }
        }
      else
      {
        // button non pressed
        button_pressed = 0;

          if (i_overflowed >= 4)
          {
            uint8_t lastLetterLength = (getLastLetterOfMessage() >> 5) & 0b00000111;

            if (newLetterIndex != 0)
            {
              newLetter |= ((newLetterIndex) & 0b00000111) << 5;
              addLetterToMessage(newLetter);
              newLetter = 0b00000000;
              newLetterIndex = 0;
              digitalWrite(OUTPUT_LED, 1);
            }
            else if (lastLetterLength != 0 && i_overflowed >= 12)
            {
              newLetter = 0b00000000;
              addLetterToMessage(newLetter);
              newLetterIndex = 0;
              digitalWrite(OUTPUT_LED, 0);
            }
        }

        if (button_pressed_prev == 1)
        {
          if (long_press == 1)
          {
            // long press
            newLetter |= (1 << newLetterIndex);
            digitalWrite(INPUT_LED_1, 0);
            long_press = 0;
          }
          else
          {
            // short press
            newLetter &= ~(1 << newLetterIndex);
          }

          newLetterIndex++;
          i_overflowed = 0;
          button_pressed_prev = 0;
        }
      }
    }
  }
}
