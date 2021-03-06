# arduinolibs

# gspswitch
## Introduction
This class handles two-way toggle switches and momentary pushbutton switches in arduino and embedded systems with minimal code requirements.

## Purpose
The aim of this series of libraries is to massively simplify the coding requirements for arduino implementations.

## How to use this
To use this class, you instantiate it using one of a number of constructor options depending on whether you wish to use function callbacks or strings,
and whether you are using pushbuttons or toggle switches.

### Callbacks
A "Callback" function is a stand-alone function that is only called when a particular action or event happens in your code. For instance, the function:

```
void turnOn() {
  // actions to take if your switch state is changed to "on"
}

void turnOff() {
  // actions to take if your switch state is changed to "off"
}
```
This can be implemented simply by defining a "gspswitch", identifying the digital pin associated with the switch, and specifying the callback functions you are going to use. 
Aside from this, there are minimal additional coding requirements...

```
gspSwitch mySwitch(12,turnOff,turnOn);

```

### Strings

If you define your switches with "strings" (instead of callback functions) the defined strings will be sent across the serial communications interface when the defined events occur.

```
gspSwitch mySwitchWithStrings(12,"I was turned Off","I was turned On");
```
This is the format of the constructor when using serial strings on a two-way toggle switch.
When the switch is turned off, the string "I was turned Off" will be printed on the serial comms interface.
When the switch is turned on, the string "I was turned On" will be printed on the serial comms interface.
the first parameter indicates the pin that will be used for the switch. It is assumed that the internal pullup resistor will be used.
```
gspSwitch myToggleSwitch(1,"Switch is Off","Switch is On");
```



### Components.

gspswitch supports two types of devices : Two-position switches and momentary pushbuttons.
The type of constructor you will use depends on a number of things:

1. what device you are using
2. what you want it to do


#### Two-Position Toggle Switch

```
    // callback mode:
    gspSwitch  {instancename}({pin}, on_callback_function, off_callback_function );

    // on_callback_function and off_callback_function must follow the following pattern:

    void on_callback_function() {
      // on actions here
    }

    void off_callback_function() {
      // off actions here
    }


    // string mode
    gspSwitch  {instancename}({pin}, "Turn on Send This", "Turn Off Send This");

```

### Momentary Pushbutton

#### Two State Latching Pushbutton operation:

The two-state latching mechanism is:

1. push once to turn on
2. push again to turn off.

```
    // callback mode:
    gspSwitch  {instancename}({pin}, on_callback_function, off_callback_function , 1 ); /// note mode indicator at the end of the constructor

    // on_callback_function and off_callback_function must follow the following pattern:

    void on_callback_function() {
      // on actions here
    }

    void off_callback_function() {
      // off actions here
    }


    // string mode
    gspSwitch  {instancename}({pin}, "Turn on Send This", "Turn Off Send This" , 1); /// note mode indicator at the end of the constructor

```

#### Push-To-Activate
##### Activate-On-Release (Default)
Function is called (or string is sent) when the button is released after being depressed.
```
    // callback mode
    gspSwitch  {instancename}({pin}, callback_function );

    // string mode
    gspSwitch  {instancename}({pin}, "Send on Activate" );

```
##### Activate-On-Push
Function is called (or string is sent) when the button is depressed.
```
    // callback mode
    gspSwitch  {instancename}({pin}, callback_function, 1 );

    // string mode
    gspSwitch  {instancename}({pin}, "Send on Activate", 1 );

```
##### Activate-Continuously-On-Push
Function is called continuously while the button is pushed.
```
    // callback mode
    gspSwitch  {instancename}({pin}, callback_function, 2 );

    // string mode
    gspSwitch  {instancename}({pin}, "Send on Activate", 2 );

```
#### Long-Push Activation
Short push of the button toggles state on and off.
Long push of the button activates the 3rd callback/string.

```
    // callback mode
    gspSwitch   {instancename}({pin}, callback_function_on, callback_function_off, callback_function_long );

    // string mode
    gspSwitch   {instancename}({pin}, "On message","Off Message","Long Push Message" );

```
#### Long-And-Longer-Push Activation
Short push of the button toggles state on and off.
Long push of the button activates the 3rd callback/string.
Very long push of the button (2s or longer) activates the 4th callback/string

```
    // callback mode
    gspSwitch   {instancename}({pin}, callback_function_on, callback_function_off, callback_function_long, callback_function_longer ); 

    // string mode
    gspSwitch   {instancename}({pin}, "On message","Off Message","Long Push Message","Longest Push Message" );

```
#### multi-state activation
each push of the pushbutton activates a new 'state'.
you enumerate the number of states you want when instantiating the component.
You can either have manual reset of the device or automatic reset.
Resetting takes the state back to '0' which is the first state.

```
    // callback mode - Auto Reset
    gspSwitch   {instancename}({pin}, {nstates}, state_change_callback); 

    // callback mode - Manual Reset
    gspSwitch   {instancename}({pin}, {nstates}, state_change_callback,1); 


    void state_change_callback(uint8_t state) {
      switch (state) {
        case 0:
        break;
        case 1...
        case 2..
        case n...
      }
    }

```
In "AutoReset" mode, call 'counterAutoResetHoldoff();' on the switch instance in order to hold-off the auto-reset operation.
This can be useful when this device is being used to alter multiple data instances using an encoder.

To reset the state in "Manual" mode, call 'counterReset();' on the switch instance.

To recover the state of the activation, call 'getCounterValue();' on the switch instance.


### Coding Requirements

The only requirements for using this library are to 
   * include the library header file #include <gspswitch.h>
   * to start up the Serial comms interface and to 
   * register the instance in setup()
   * put a line of code somewhere in the "loop()" function:
```

/*
 * This is the requried header file
 */ 

#include "gspswitch.h"

/*
 * Here is where we define the switch instances.
 *
 * gspswitch <variablename>(<pin>,<off_callback>,<on_callback>);
 *   ...or...
 * gspswitch <variablename>(<pin>,"Off Event STring To Send","On Event String to Send");
 */

gspSwitch myToggleSwitch(1,"Switch is Off","Switch is On");
gspSwitch myOtherToggleSwitch(1,"Switch is Off","Switch is On");

void setup() {
  Serial.begin(115200); // turn on the serial interface

  /*
   * Register the switch instances with gspGrouped
   */
  gspGrouped::register_instance(&myToggleSwitch); // you need one of these for each gspSwitch
  gspGrouped::register_instance(&myOtherToggleSwitch); // you need one of these for each gspSwitch
}

void loop() {

...

  /*
   * Check all the switches. Only one line required.
   */
  gspSwitch.checkAll(); //<< This is the only other line of code required in the app.

...

}

```
## Debugging
if you define DEBUG=1 in your build options, the class will provide some basic debugging output on the serial interface with each action.

## Example code 
```
#include "gspswitch.h"

// Set up the on/off switch
// Classname (gspSwitch)
// Instance name (we will call it switch1)
// Parameters:
// 1:pin number
// 2:Function to call when we turn the switch off
// 3:Function to call when we turn the switch on
gspSwitch swLandingLight(2,"C430","C431");
gspSwitch swApuGenerator(3,"E500","E501");
gspSwitch swFuelValveE1(4,"F32","F33");
gspSwitch swFuelValveE2(5,"F35","F36");
gspSwitch swPanelLights(6,"C460","C461");
gspSwitch swNavLights(7,"C410","C411");
gspSwitch swBeacons(8,"C420","C421");
gspSwitch pbNoSmoke(9,"Y12");
gspSwitch pbSeatBelt(10,"Y16");
gspSwitch swStartValveE1(11,"E42","E43");
gspSwitch swStartValveE2(12,"E45","E46");
gspSwitch pbPushBack(A0,"Y01");
gspSwitch pbJetway(A1,"Y02");
gspSwitch swApuStarter(A2,"E47","E48");
gspSwitch swMasterBattery(A3,"E17","E18");
gspSwitch swStrobeLights(A4,"C450","C451");




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gspGrouped::register_instance(&swLandingLight);
  gspGrouped::register_instance(&swApuGenerator);
  gspGrouped::register_instance...etc..
  etc...
}

void loop() {
  // put your main code here, to run repeatedly:

  // this must go in loop/
  // allows us to check what's happening with the switch.

  gspSwitch::checkAll();
}
```
