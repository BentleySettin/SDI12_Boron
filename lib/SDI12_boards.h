/**
 * @file SDI12_boards.h
 * @copyright (c) 2013-2020 Stroud Water Research Center (SWRC)
 *                          and the EnviroDIY Development Team
 * @author Sara Geleskie Damiano (sdamiano@stroudcenter.org)
 *
 * @brief This file defines the timing units needed for various Arduino boards.
 *
 */

/* ======================== Arduino SDI-12 =================================
An Arduino library for SDI-12 communication with a wide variety of environmental
sensors. This library provides a general software solution, without requiring
   ======================== Arduino SDI-12 =================================*/

   #ifndef SRC_SDI12_BOARDS_H_
   #define SRC_SDI12_BOARDS_H_
   
   #include <Arduino.h>
   
   #if defined(ESP32) || defined(ESP8266) || defined(PARTICLE)
   /** The interger type (size) of the timer return value */
   typedef uint32_t sdi12timer_t;
   #else
   /** The interger type (size) of the timer return value */
   typedef uint8_t sdi12timer_t;
   #endif
   
   /**
    * @brief The class used to define the processor timer for the SDI-12 serial emulation.
    */
   class SDI12Timer {
    public:
     /**
      * @brief Construct a new SDI12Timer
      */
     SDI12Timer();
     /**
      * @brief Set the processor timer prescaler such that the 10 bits of an SDI-12
      * character are divided into the rollover time of the timer.
      *
      * @note  The ESP32 and ESP8266 are fast enough processors that they can take the
      * time to read the core 'micros()' function still complete the other processing
      * needed on the serial bits.  All of the other processors using the Arduino core also
      * have the micros function, but the rest are not fast enough to waste the processor
      * cycles to use the micros function and must manually configure the processor timer
      * and use the faster assembly macros to read that processor timer directly.
      */
     void configSDI12TimerPrescale(void);
     /**
      * @brief Reset the processor timer prescaler to whatever it was prior to being
      * adjusted for this library.
      *
      * @note The prescaler is *NOT* set back to initial values for SAMD boards!  On those
      * processors, generic clock generator 4 will remain configured for SDI-12 until it is
      * reset outside of this library.
      */
     void resetSDI12TimerPrescale(void);
   
   // Most 'standard' AVR boards
   //
   #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || \
     defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) ||  \
     defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) ||   \
     defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega1284__)
   
   /**
    * @brief A string description of the timer to use
    *
    * Timer/Counter2 (TC2) is a general purpose, single channel, 8-bit Timer/Counter
    * module.
    *
    * Features
    * - Single Channel Counter
    * - Clear Timer on Compare Match (Auto Reload)
    * - Glitch-free, Phase Correct Pulse Width Modulator (PWM)
    * - Frequency Generator
    * - 10-bit Clock Prescaler
    * - Overflow and Compare Match Interrupt Sources (TOV2, OCF2A, and OCF2B)
    * - Allows Clocking from External 32kHz Watch Crystal Independent of the I/O Clock
    */
   #define TIMER_IN_USE_STR "TCNT2"
   /**
    * @brief The c macro name for the assembly timer to use
    *
    * The register used to access the timer/counter value is TCNT2
    */
   #define TCNTX TCNT2  // Using Timer 2
   
   #if F_CPU == 16000000L
   /**
    * @brief A string description of the prescaler in use.
    */
   #define PRESCALE_IN_USE_STR "1024"
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit at
    * the SDI-12 baud rate of 1200 bits/second.
    *
    * 16MHz / 1024 prescaler = 15624 'ticks'/sec = 64 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/64 µs) = 13.0208 ticks/bit
    *
    * The 8-bit timer rolls over after 256 ticks, 19.66085 bits, or 16.38505 ms
    * (256 ticks/roll-over) * (1 bit/13.0208 ticks) = 19.66085 bits
    * (256 ticks/roll-over) * (1 sec/15624 ticks) = 16.38505 milliseconds
    */
   #define TICKS_PER_BIT 13
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(13.0208 ticks/bit) * 2^10 = 78.6432
    */
   #define BITS_PER_TICK_Q10 79
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 2
   
   #elif F_CPU == 12000000L
   /**
    * @brief A string description of the prescaler in use.
    */
   #define PRESCALE_IN_USE_STR "1024"
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit at
    * the SDI-12 baud rate of 1200 bits/second.
    *
    * 12MHz / 1024 prescaler = 11719 'ticks'/sec = 85 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/85 µs) = 9.765625 ticks/bit
    *
    * The 8-bit timer rolls over after 256 ticks, 26.2144 bits, or 21.84487 ms
    * (256 ticks/roll-over) * (1 bit/9.765625 ticks) = 26.2144 bits
    * (256 ticks/roll-over) * (1 sec/11719 ticks) = 21.84487 milliseconds
    */
   #define TICKS_PER_BIT 10
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(9.765625 ticks/bit) * 2^10 = 104.8576
    */
   #define BITS_PER_TICK_Q10 105
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 2
   
   #elif F_CPU == 8000000L
   /**
    * @brief A string description of the prescaler in use.
    */
   #define PRESCALE_IN_USE_STR "256"
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit at
    * the SDI-12 baud rate of 1200 bits/second.
    *
    * 8MHz / 256 prescaler = 31250 'ticks'/sec = 32 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/32 µs) = 26.04166667 ticks/bit
    *
    * The 8-bit timer rolls over after 256 ticks, 9.8304 bits, or 8.192 ms
    * (256 ticks/roll-over) * (1 bit/26.04166667 ticks) = 9.8304 bits
    * (256 ticks/roll-over) * (1 sec/31250 ticks) = 8.192 milliseconds
    * @note The timer will roll-over with each character!
    */
   #define TICKS_PER_BIT 26
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(26.04166667 ticks/bit) * 2^10 = 39.3216
    */
   #define BITS_PER_TICK_Q10 39
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 10
   
     // #define PRESCALE_IN_USE_STR "1024"
     // #define TICKS_PER_BIT 6
     //     // 8MHz / 1024 prescaler = 31250 'ticks'/sec = 128 µs / 'tick'
     //     // (1 sec/1200 bits) * (1 tick/128 µs) = 6.5104166667 ticks/bit
     // #define BITS_PER_TICK_Q10 157
     //     // 1/(6.5104166667 ticks/bit) * 2^10 = 157.2864
     // #define RX_WINDOW_FUDGE 5
   
   #endif
   
   
   // ATtiny boards (ie, adafruit trinket)
   //
   #elif defined(__AVR_ATtiny25__) | defined(__AVR_ATtiny45__) | defined(__AVR_ATtiny85__)
   
   /**
    * @brief A string description of the timer to use
    *
    * The Timer/Counter1 features a high resolution and a high accuracy usage with the
    * lower prescaling opportunities. It can also support two accurate, high speed, 8-bit
    * pulse width modulators using clock speeds up to 64MHz (or 32MHz in low speedmode).
    */
   #define TIMER_IN_USE_STR "TCNT1"
   /**
    * @brief The c macro name for the assembly timer to use
    *
    * The register used to access the timer/counter value is TCNT1.
    */
   #define TCNTX TCNT1  // Using Timer 1
   
   #if F_CPU == 16000000L
   /**
    * @brief A string description of the prescaler in use.
    */
   #define PRESCALE_IN_USE_STR "1024"
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit at
    * the SDI-12 baud rate of 1200 bits/second.
    *
    * 16MHz / 1024 prescaler = 15624 'ticks'/sec = 64 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/64 µs) = 13.0208 ticks/bit
    *
    * The 8-bit timer rolls over after 256 ticks, 19.66 bits, or 16.38505 ms
    * (256 ticks/roll-over) * (1 bit/13.0208 ticks) = 19.66 bits
    * (256 ticks/roll-over) * (1 sec/15624 ticks) = 16.38505 milliseconds
    */
   #define TICKS_PER_BIT 13
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(13.0208 ticks/bit) * 2^10 = 78.6432
    */
   #define BITS_PER_TICK_Q10 79
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 2
   
   #elif F_CPU == 8000000L
   #define PRESCALE_IN_USE_STR "512"
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit at
    * the SDI-12 baud rate of 1200 bits/second.
    *
    * 8MHz / 512 prescaler = 15624 'ticks'/sec = 64 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/64 µs) = 13.0208 ticks/bit
    *
    * The 8-bit timer rolls over after 256 ticks, 19.66 bits, or 16.38505 ms
    * (256 ticks/roll-over) * (1 bit/13.0208 ticks) = 19.66 bits
    * (256 ticks/roll-over) * (1 sec/15624 ticks) = 16.38505 milliseconds
    */
   #define TICKS_PER_BIT 13
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(13.0208 ticks/bit) * 2^10 = 78.6432
    */
   #define BITS_PER_TICK_Q10 79
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 5
   
   #endif
   
   
   // Arduino Leonardo & Yun and other 32U4 boards
   //
   #elif defined(ARDUINO_AVR_YUN) || defined(ARDUINO_AVR_LEONARDO) || \
     defined(__AVR_ATmega32U4__)
   
   /**
    * @brief A string description of the timer to use
    *
    * Timer/Counter4 is a general purpose high speed Timer/Counter module, with three
    * independent Output Compare Units, and with enhanced PWM support.
    *
    * Features
    * - Up to 10-Bit Accuracy
    * - Three Independent Output Compare Units
    * - Clear Timer on Compare Match (Auto Reload)
    * - Glitch Free, Phase and Frequency Correct Pulse Width Modulator (PWM)
    * - Enhanced PWM mode: one optional additional accuracy bit without effect on output
    * frequency
    * - Variable PWM Period
    * - Independent Dead Time Generators for each PWM channels
    * - Synchronous update of PWM registers
    * - Five Independent Interrupt Sources (TOV4, OCF4A, OCF4B, OCF4D, FPF4)
    * - High Speed Asynchronous and Synchronous Clocking Modes
    * - Separate Prescaler Unit
    */
   #define TIMER_IN_USE_STR "TCNT4"
   /**
    * @brief The c macro name for the assembly timer to use
    *
    * The register used to access the low byte timer/counter value is TCNT4.
    * @note We only utilize the low byte register, effectively using the 10-bit timer as an
    * 8-bit timer.
    */
   #define TCNTX TCNT4  // Using Timer 4
   
   #if F_CPU == 16000000L
   /**
    * @brief A string description of the prescaler in use.
    */
   #define PRESCALE_IN_USE_STR "1024"
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit at
    * the SDI-12 baud rate of 1200 bits/second.
    *
    * 16MHz / 1024 prescaler = 15624 'ticks'/sec = 64 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/64 µs) = 13.0208 ticks/bit
    *
    * The first 8-bits of the 10-bit timer roll over after 256 ticks, 19.66 bits,
    * or 16.38505 ms
    * (256 ticks/roll-over) * (1 bit/13.0208 ticks) = 19.66 bits
    * (256 ticks/roll-over) * (1 sec/15624 ticks) = 16.38505 milliseconds
    */
   #define TICKS_PER_BIT 13
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(13.0208 ticks/bit) * 2^10 = 78.6432
    */
   #define BITS_PER_TICK_Q10 79
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 2
   
   #elif F_CPU == 8000000L
   /**
    * @brief A string description of the prescaler in use.
    */
   #define PRESCALE_IN_USE_STR "512"
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit
    * at the SDI-12 baud rate of 1200 bits/second.
    *
    * 8MHz / 512 prescaler = 15624 'ticks'/sec = 64 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/64 µs) = 13.0208 ticks/bit
    *
    * The first 8-bits of the 10-bit timer roll over after 256 ticks, 19.66 bits,
    * or 16.38505 ms
    * (256 ticks/roll-over) * (1 bit/13.0208 ticks) = 19.66 bits
    * (256 ticks/roll-over) * (1 sec/15624 ticks) = 16.38505 milliseconds
    */
   #define TICKS_PER_BIT 13
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(13.0208 ticks/bit) * 2^10 = 78.6432
    */
   #define BITS_PER_TICK_Q10 79
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 5
   
   #endif
   
   
   // Arduino Zero other SAMD21 boards
   //
   #elif defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_ARCH_SAMD) || \
     defined(__SAMD21G18A__) || defined(__SAMD21J18A__) || defined(__SAMD21E18A__)
   
   /**
    * @brief A string description of the timer to use
    *
    * The Generic Clock controller GCLK provides nine Generic Clock Generators that can
    * provide a wide range of clock frequencies.
    *
    * Generators can be set to use different external and internal oscillators as source.
    * The clock of each Generator can be divided.  The outputs from the Generators are used
    * as sources for the Generic Clock Multiplexers, which provide the Generic Clock
    * (GCLK_PERIPHERAL) to the peripheral modules, as shown in Generic Clock Controller
    * Block Diagram.
    *
    * Features
    * - Provides Generic Clocks
    * - Wide frequency range
    * - Clock source for the generator can be changed on the fly
    *
    * The TC consists of a counter, a prescaler, compare/capture channels and control
    * logic. The counter can be set to count events, or it can be configured to count clock
    * pulses. The counter, together with the compare/capture channels, can be configured to
    * timestamp input events, allowing capture of frequency and pulse width. It can also
    * perform waveform generation, such as frequency generation and pulse-width modulation
    * (PWM).
    *
    * Features
    * - Selectable configuration
    *   – Up to five 16-bit Timer/Counters (TC) including one low-power TC, each
    * configurable as:
    *     - 8-bit TC with two compare/capture channels
    *     - 16-bit TC with two compare/capture channels
    *     - 32-bit TC with two compare/capture channels, by using two TCs
    * - Waveform generation
    *     – Frequency generation
    *     – Single-slope pulse-width modulation
    * - Input capture
    *     – Event capture
    *     – Frequency capture
    *     – Pulse-width capture
    * - One input event
    * - Interrupts/output events on:
    *     – Counter overflow/underflow
    *     – Compare match or capture
    * - Internal prescaler
    * - Can be used with DMA and to trigger DMA transactions
    */
   #define TIMER_IN_USE_STR "GCLK4-TC3"
   /**
    * @brief The c macro name for the assembly timer to use
    *
    * This signifies the register of timer/counter 3, the 8-bit count, the count value
    */
   #define TCNTX REG_TC3_COUNT8_COUNT  // Using Timer 3 with generic clock 4
   
   /**
    * @brief A string description of the prescaler in use.
    */
   #define PRESCALE_IN_USE_STR "3x1024"
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit at
    * the SDI-12 baud rate of 1200 bits/second.
    *
    * 48MHz / 3 pre-prescaler = 16MHz
    * 16MHz / 1024 prescaler = 15624 'ticks'/sec = 64 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/64 µs) = 13.0208 ticks/bit
    *
    * The 8-bit count rolls over after 256 ticks, 19.66 bits, or 16.38505 ms
    * (256 ticks/roll-over) * (1 bit/13.0208 ticks) = 19.66 bits
    * (256 ticks/roll-over) * (1 sec/15624 ticks) = 16.38505 milliseconds
    */
   #define TICKS_PER_BIT 13
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(13.0208 ticks/bit) * 2^10 = 78.6432
    */
   #define BITS_PER_TICK_Q10 79
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 2
   
   // Espressif ESP32/ESP8266 boards
   //
   #elif defined(ESP32) || defined(ESP8266) || defined(PARTICLE)
     /**
      * @brief Read the processor micros and right shift 6 bits (ie, divide by 64) to get a
      * 64µs tick.
      *
      * @note  The ESP32 and ESP8266 are fast enough processors that they can take the time
      * to read the core 'micros()' function still complete the other processing needed on
      * the serial bits.  All of the other processors using the Arduino core also have the
      * micros function, but the rest are not fast enough to waste the processor cycles to
      * use the micros function and must use the faster assembly macros to read the
      * processor timer directly.
      *
      * @return **sdi12timer_t** The current processor micros
      */
     sdi12timer_t SDI12TimerRead(void);
   
   /**
    * @brief The number of "ticks" of the timer that occur within the timing of one bit
    * at the SDI-12 baud rate of 1200 bits/second.
    *
    * 48MHz / 3 pre-prescaler = 16MHz
    * 16MHz / 1024 prescaler = 15624 'ticks'/sec = 64 µs / 'tick'
    * (1 sec/1200 bits) * (1 tick/64 µs) = 13.0208 ticks/bit
    *
    * The 8-bit count rolls over after 256 ticks, 19.66 bits, or 16.38505 ms
    * (256 ticks/roll-over) * (1 bit/13.0208 ticks) = 19.66 bits
    * (256 ticks/roll-over) * (1 sec/15624 ticks) = 16.38505 microseconds
    */
   #define TICKS_PER_BIT 13
   /**
    * @brief The number of "ticks" of the timer per SDI-12 bit, shifted by 2^10.
    *
    * 1/(13.0208 ticks/bit) * 2^10 = 78.6432
    */
   #define BITS_PER_TICK_Q10 79
   /**
    * @brief A "fudge factor" to get the Rx to work well.   It mostly works to ensure that
    * uneven tick increments get rounded up.
    *
    * @see https://github.com/SlashDevin/NeoSWSerial/pull/13
    */
   #define RX_WINDOW_FUDGE 2
   
   // Unknown board
   #else
   #endif
   };
   
   #endif  // SRC_SDI12_BOARDS_H_