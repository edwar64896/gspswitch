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
    
class gspSwitch:public gspGrouped
{
  public:

    static gspSwitch * makeOne(uint8_t _pin, 
        nonstd::function<void ()> cb1, /*callback to invoke upon successful parse*/
        nonstd::function<void ()> cb2) /*callback to invoke upon successful parse*/
	{
        gspSwitch * instance = new gspSwitch(_pin,cb1,cb2);
        gspGrouped::register_instance(instance);
        return instance;
    }

    static gspSwitch * makeOne(uint8_t _pin, 
	const char * s1, 
	const char * s2) 
	{
        gspSwitch * instance = new gspSwitch(_pin,s1,s2);
        gspGrouped::register_instance(instance);
        return instance;
    }

    static gspSwitch * makeOne(uint8_t _pin, 
        nonstd::function<void ()> cb1, /*callback to invoke upon successful parse*/
	uint8_t mode=0) 
	{
        gspSwitch * instance = new gspSwitch(_pin,cb1,mode);
        gspGrouped::register_instance(instance);
        return instance;
    }

    static gspSwitch * makeOne(uint8_t _pin, 
	const char * s1,
	uint8_t mode=0) 
	{
        gspSwitch * instance = new gspSwitch(_pin,s1,mode);
        gspGrouped::register_instance(instance);
        return instance;
    }

    // constructors for 2-position toggle switches
    gspSwitch(uint8_t , void (*)(), void (*)());
    gspSwitch(uint8_t , const char *, const char *);

    // constructors for momentary pushbutton switches
    // mode = 0 (default) for "activate once on release"
    // mode = 1 for "activate once on push"
    // mode = 2 for "activate continuously on push"
    gspSwitch(uint8_t , void (*)(),uint8_t mode = 0);
    gspSwitch(uint8_t , const char *,uint8_t mode = 0);

    // default constructor
    gspSwitch();
    ~gspSwitch();

    bool check();

    uint8_t getState();

    enum state {
      Off=0,
      On
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
    const char* _strOff = nullptr;
    const char* _strOn = nullptr;
    uint8_t _pin=0;
    int _old_s1 = -1;
    int _s1=0;
    int _s1count=0;
    int _s2=0;
    int _switchMode=gspSwitch_MODE_UNCONFIGURED;

    uint8_t _switchState=gspSwitch::Off;

    int switchInstance=0;

    int dbug=0;

};

#endif
