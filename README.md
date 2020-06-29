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

This is the format of the constructor when using serial strings on a two-way toggle switch.
When the switch is turned off, the string "I was turned Off" will be printed on the serial comms interface.
When the switch is turned on, the string "I was turned On" will be printed on the serial comms interface.
the first parameter indicates the pin that will be used for the switch. It is assumed that the internal pullup resistor will be used.
```
gspSwitch myToggleSwitch(1,"Switch is Off","Switch is On");
```

### Constructors
```

    // constructors for 2-position toggle switches
    // mode = 0 (default) -> two position switch
    // mode = 1 -> latching pushbutton
    //
    // params: 
    //    1 - on action
    //    2 - off action
    //    3 - mode
    gspSwitch(uint8_t, nonstd::function<void ()>, nonstd::function<void ()>, uint8_t mode = 0 );
    gspSwitch(uint8_t, const char *, const char *,uint8_t mode = 0);

    // constructors for momentary pushbutton switches
    // mode = 0 (default) for "activate once on release"
    // mode = 1 for "activate once on push"
    // mode = 2 for "activate continuously on push"
    //
    // params: 
    //    1 - activation action
    //    2 - mode
    gspSwitch(uint8_t, nonstd::function<void ()>,uint8_t mode = 0);
    gspSwitch(uint8_t, const char *,uint8_t mode = 0);

    // mode 3 pushbutton - this is an "activate on release" mode that allows 
    // use of a pushbutton based on how long the button was down for.
    //
    // on/off - very short momentary pushbutton action
    // alternate mode 1 - button is depressed for 1 second or longer
    //
    // params: 
    //    1 - on action
    //    2 - off action
    //    3 - long depress action
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
    //    1 - on action
    //    2 - off action
    //    3 - long depress action
    //    4 - very long depress action
    gspSwitch(uint8_t, nonstd::function<void ()>, nonstd::function<void ()>, nonstd::function<void ()>, nonstd::function<void ()>);
    gspSwitch(uint8_t, const char *, const char *, const char *, const char *);  


```

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
## Pushbutton modes

The pushbutton operational modes are configured in the last parameter of the constructor.
The default mode is "Activate on release". Other optional modes are "Activate once on push" or "Activate continuously on push".

### Activate on release (default)
When pushed, the button does nothing, however once you release it the callback is invoked once - or the "string" is sent once. This mode is best used to avoid accidental double-taps on the switch.

```
gspSwitch myPushButton(2 /* pin */,myButtonWasReleased /* callback function */,0 /* mode */);
gspSwitch myPushButton(2 /* pin */,myButtonWasReleased /* callback function */); //same as above
```

### Activate once on push (mode param=1)
When pushed, the callback is invoked once. In order for the callback to be invoked again, the pushbutton must be released.
```
gspSwitch myPushButton(2 /* pin */,myButtonWasPushed /* callback function */,1 /* mode */);
```
### Activate continuously on push
When pushed the callback is invoked repeatedly until the pushbutton is released.
```
void myButtonIsStillBeingPushed() {
Serial.println("we will keep on seeing this while the button is being pushed");
}
gspSwitch myPushButton(2 /* pin */,myButtonIsStillBeingPushed /* callback function */,2 /* mode */);
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
