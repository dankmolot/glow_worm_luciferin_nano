#pragma once

/*
  Non-blocking replacements for delay().
  Several convenient ways to use them.
  Every will tell you _at_ every n millis().
  Every::Timer will tell you (once) when n millis() has gone by (or after, or until)
  Every::Toggle will keep track of a toggling boolean for you.

  * Every n millis

  
    static Every t1(100);  // every 100 msec

    void setup() {
      ... 
      
      // considerable time can pass from global construction to now
      t1.reset();
      }
    
    if ( t1() ) { do it; }

    * Details
      The "every" object has to be static/global, obviously because it needs to remember the "last" time.
      Note the `.reset()` to start timing just before the first loop()!
      
      The object + "()" is magic: returns a boolean meaning "just expired?" (and restarts for the next interval.

      The initial event does not happen immediately, it happens in n msec. If you want an immediate first
      event, supply "true" for the "now" argument in the constructors: Every(100, true). 

      The interval can be up to 2^32 msecs (full range of millis()). Sadly, that's 4 bytes.

      Takes 8 bytes of RAM for "Every" object.
      
      Resists drift by "re-aligning" when it detects that the interval has expired. E.g. if it should
      happen every 100 msec, but you don't test till 30msec late (i.e. at 130msec), it will fire, and
      re-align to fire at 200msec. Thus, it's not an interval, it's "_on_ every n msec". For small
      amounts of drift, this is probably nice. For larger amounts, might be confusing (see Every::Timer). 

      There's no suspend/stop. That would add at least 1 boolean more memory!

      See examples/Every

# Example Toggle every n millis

    // "blink" example using EveryToggle
    Every::Toggle t1(200); // adds the .state() method
    void setup() {
      pinMode(LED_BUILTIN, OUTPUT); }

      // considerable time can pass from global construction to now
      t1.reset();
      }

    void loop() {
      if ( t1() ) {
        digitalWrite( LED_BUILTIN, t1.state ); // state goes true,false,true,false...
        }
    }

  * Count from 0..n-1, 0..n-1, etc
    static EveryCount(100, 10); // 0..9
    
    if ( t1() ) { Serial.println( t1.state ); } // prints 0,1,2..9, with delay of 100 between

  * Tell when an interval has passed
    static Every::Timer(1000);

    if ( t1() ) { Serial.println("1 second"); } // once

    * Is timer running?

      if (t1.running) { it's running now }

  * Cycle through a sequence
    const char abcd[] = { 'a', 'b', 'c', 'd' }; // the sequence
    // adds the .sequence() method
    static EverySequence t1(150, 4,abcd); // have to say "4" to say how long the sequence is

    void setup() {
      ... 
      
      // considerable time can pass from global construction to now
      t1.reset();
      }

    // Prints a,b,c,d,a,b,c,d, with a delay of 100 between
    if ( t1() ) { Serial.println( t1.sequence() ) };

  * Use lambda, or a function, or functor
 
    boolean happened = t1( &doit } );
    boolean happened = t1( someobject } ); // if it has a: void operator()()
    boolean happened = t1( []() { do it; } ); // "inline" function
    boolean happened = t1( [somevar]() { do it with somevar; } ); // use vars in the body

    * Details
      You can use a lambda that has a capture. But, you can refer to global/static objects.
      Note the `.reset()` to start timing just before the first loop()!

      functions/lambdas/functors will be called with no arguments.
      
  * Resetting
    if (tmetoreset) t1.reset(); // next in 100 msec from this call
    // t1.reset(true); // the next t1() will be true ("immediate")
    // t1.reset(150, true); // resets the interval too!

  * Changing interval
 
    t1.interval = 1000; // I lied, change it

  * How long ago was last
    ...
    millis() - t1.last;
    

  ???

  A timeline: n1,n2,n3 => event1,event2,event3
  Special case of pred1,pred2,... => event1,event2
*/

//#include <Streaming.h>
//#define DEBUG Serial << '[' << millis() << "] "

#include <Arduino.h>

class Every {
  public:
    // everthing public
    unsigned long last; // last time we fired
    unsigned long interval = 1000; // "delay" till next firing

    // set interval at x() time, default 1000
    Every(bool now = false) : Every( 1000, now) {}

    Every(int interval, bool now = false) : Every( (unsigned long) interval, now) {}
    Every(unsigned long interval, bool now = false) : interval(interval) {
      last = millis(); // so, would wait for interval

      if (now) {
        last -= interval; // adjust to "already expired"
      }
    }

    virtual boolean operator()() {
      // lots of this class means lots of calls to millis()
      unsigned long now = millis(); // minimize drift due to this fn
      unsigned long diff = now - last;
      
      if (diff >= interval) {
        unsigned long drift = diff % interval;
        // DEBUG << "drift " << last << " now " << now << " d: " << drift << endl;
        last = now;
        last -= drift;
        return true;
      }
      else {
        return false;
      }
    }
    // so you can use "bare" numbers like 100 for 100msec
    boolean operator()(int x_interval) { return (*this)( (unsigned long) x_interval); }
    boolean operator()(unsigned long x_interval) {
      unsigned long now = millis(); // minimize drift due to this fn
      unsigned long diff = now - last;

      if (diff >= x_interval) {
        unsigned long drift = diff % x_interval;
        //Serial << "drift " << last << " now " << now << " d: " << drift << endl;
        last = now;
        last -= drift;
        return true;
      }
      else {
        return false;
      }
  
    }

    template <typename T>
    boolean operator()(T lambdaF ) {
      // simple lambda: []() { do something };
      boolean hit = (*this)();
      if (hit) lambdaF();
      return hit;
    }

    // sadly, the 'virtual' also prevents optimizing away an unused 'interval' instance-var
    virtual void reset(boolean now=false) {
      last = millis();
      if (now) last -= interval;
    }
    void reset(unsigned long interval, boolean now=false) { 
      this->interval=interval; 
      reset(now); 
      }

    class Toggle;
    class Pattern;
    class Timer;
};
