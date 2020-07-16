#ifndef GSP_SWITCH
#define GSP_SWITCH

#include "Arduino.h"
#include "nonstd.h"
#include "gspgrouped.h"

#define gspSwitch_MODE_UNCONFIGURED 0
#define gspSwitch_MODE_SWITCH_CB 1
#define gspSwitch_MODE_SWITCH_STR 2
#define gspSwitch_MODE_PUSHBUTTON_PUSH_CB 3
#define gspSwitch_MODE_PUSHBUTTON_PUSH_STR 4
#define gspSwitch_MODE_PUSHBUTTON_RELEASE_CB 5
#define gspSwitch_MODE_PUSHBUTTON_RELEASE_STR 6
#define gspSwitch_MODE_PUSHBUTTON_CONTINUOUS_CB 7
#define gspSwitch_MODE_PUSHBUTTON_CONTINUOUS_STR 8
#define gspSwitch_MODE_PUSHBUTTON_LATCH_CB 9
#define gspSwitch_MODE_PUSHBUTTON_LATCH_STR 10
#define gspSwitch_MODE_PUSHBUTTON_TIMEBASED_CB 11
#define gspSwitch_MODE_PUSHBUTTON_TIMEBASED_STR 12
#define gspSwitch_MODE_PUSHBUTTON_TIMEBASED2_CB 13
#define gspSwitch_MODE_PUSHBUTTON_TIMEBASED2_STR 14
#define gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_MANUALRESET_CB 15
#define gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB 16

#define _SW_BAND_1  1024
#define _SW_BAND_2  16384
#define _SW_BAND_3  32768

#define gspSwitch_AUTORESET_CAP 1024

#define gspSwitch_DEBOUNCE_COUNT 1024
    
class gspSwitch:public gspGrouped
{
  public:

    // constructors for 2-position toggle switches or latching pushbuttons
    // mode = 0 (default) -> two position switch
    // mode = 1 -> latching pushbutton
    //
    // params: 
    //    1 - switch pin
    //    2 - on action (fn (void) callback)
    //    3 - off action (fn (void) callback)
    //    4 - mode (default 0)
    gspSwitch(uint8_t, nonstd::function<void ()>, nonstd::function<void ()>, uint8_t mode = 0 );
    gspSwitch(uint8_t, const char *, const char *,uint8_t mode = 0);

    // constructors for momentary pushbutton switches
    // mode = 0 (default) for "activate once on release"
    // mode = 1 for "activate once on push"
    // mode = 2 for "activate continuously on push"
    //
    // params: 
    //    1 - switch pin
    //    2 - activation action (fn (void) callback)
    //    3 - mode (default 0)
    gspSwitch(uint8_t, nonstd::function<void ()>,uint8_t mode = 0);
    gspSwitch(uint8_t, const char *,uint8_t mode = 0);

    // mode 3 pushbutton - this is an "activate on release" mode that allows 
    // use of a pushbutton based on how long the button was down for.
    //
    // on/off - very short momentary pushbutton action
    // alternate mode 1 - button is depressed for 1 second or longer
    //
    // params: 
    //    1 - switch pin
    //    2 - on action (fn (void) callback)
    //    3 - off action (fn (void) callback)
    //    4 - long depress action
    gspSwitch(uint8_t, nonstd::function<void ()>, nonstd::function<void ()>, nonstd::function<void ()>);
    gspSwitch(uint8_t, const char *, const char *, const char *);    


    // mode 4 pushbutton - this is an "activate on release" mode that allows 
    // use of a pushbutton based on how long the button was down for.
    //
    // on/off - very short momentary pushbutton action
    // alternate action 1 - button is depressed for 1 second or longer
    // alternate action 2 - button is depressed for 2 second or longer
    //
    // params: 
    //    1 - switch pin
    //    2 - on action (fn (void) callback)
    //    3 - off action (fn (void) callback)
    //    4 - long depress action (fn (void) callback)
    //    5 - very long depress action (fn (void) callback)
    gspSwitch(uint8_t, nonstd::function<void ()>, nonstd::function<void ()>, nonstd::function<void ()>, nonstd::function<void ()>);
    gspSwitch(uint8_t, const char *, const char *, const char *, const char *);    


    // constructors for momentary pushbutton switches with internal counter
    // mode = 0 (default) for "activate once on release"
    // mode = 1 for "activate once on push"
    //
    // params: 
    //    1 - switch pin
    //    2 - max counter states
    //    2 - counter change action (counter sent as parameter)  (fn (uint8_t) callback)
    //    3 - mode (default gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_MANUALRESET_CB or gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB)
    gspSwitch(uint8_t, uint8_t, nonstd::function<void (uint8_t)>, uint8_t = 0);
    
    //resets the counter - only valid with this mode/constructor
    void counterReset();
    uint8_t getCounterValue();
    void counterAutoResetHoldoff();


    // default constructor
    gspSwitch();
    ~gspSwitch();

    bool check();

    uint8_t getState();

    enum state {
      Off=0,
      On,
      Mode1,
      Mode2
    };

    static void checkAll() {
      gspGrouped::checkAll(gspSwitch::firstInstance);
    }

  protected:
    void setFirstInstance(gspGrouped * fi) {gspSwitch::firstInstance = fi;}
    gspGrouped * getFirstInstance() {return gspSwitch::firstInstance;}
     static gspGrouped * firstInstance;

  private:

    void debugPrint(int pin);

    nonstd::function<void ()> _callback_off; /*callback to invoke upon successful parse*/
    nonstd::function<void ()> _callback_on; /*callback to invoke upon successful parse*/
    nonstd::function<void ()> _callback_depress; /*callback to invoke upon successful parse*/
    nonstd::function<void ()> _callback_depress2; /*callback to invoke upon successful parse*/
    nonstd::function<void (uint8_t)> _callback_changestate; /*callback to invoke upon successful parse*/
    const char* _strOff = nullptr;
    const char* _strOn = nullptr;
    const char* _strDepress = nullptr;
    const char* _strDepress2 = nullptr;
    uint8_t _pin=0;
    int _old_s1 = -1;
    int _s1=0;
    int _s1count=0;
    int _s2=0;
    uint32_t _s3=0;
    int _switchMode=gspSwitch_MODE_UNCONFIGURED;

    uint8_t _switchState=gspSwitch::Off;

    int switchInstance=0;

    int dbug=0;

    void counterIncrement();
    uint8_t _maxCounter;
    uint8_t _counter=0;
    uint64_t _autoResetCounter = 0;

};

#endif
