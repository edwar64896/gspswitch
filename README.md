# arduinolibs

# gspswitch
## Introduction
This class handles two-way toggle switches and momentary pushbutton switches in arduino and embedded systems with minimal code requirements.

## Purpose
The purpose of the class is to simplify the use of callback function or serial communications when switches or buttons are used.
A "Callback function" is simply a block of code that is executed when an action occurs within the code.

## How to use this
To use this class, you instantiate it using one of four constructor options depending on whether you wish to use callbacks or strings,
and whether you are using pushbuttons or toggle switches.


This is the format of the constructor when using serial strings on a two-way toggle switch.
When the switch is turned off, the string "Switch is Off" will be printed on the serial comms interface.
When the switch is turned on, the string "Switch is On" will be printed on the serial comms interface.
the first parameter indicates the pin that will be used for the switch. It is assumed that the internal pullup resistor will be used.
```
gspSwitch myToggleSwitch(1,"Switch is Off","Switch is On");
```
another option is to use callbacks. Callbacks give you more flexibility in the actions that can be performed.
Here is an example of exactly the same output, but using callbacks:

```
void myToggleSwitchGoesOff() {
  Serial.println("Switch is Off");
}
void myToggleSwitchGoesOn() {
  Serial.println("Switch is On");
}

gspSwitch myToggleSwitch(1,myToggleSwitchGoesOff,myToggleSwitchGoesOn);
```

The constructors for momentary pushbutton operation are similar, only taking a single string or callback function as a parameter.

```
gspSwitch myPushButton(2,"Button was pushed");
```
... or with a callback:
```
void myButtonWasPushed() {
  Serial.println("You pushed my button.");
}
gspSwitch myPushButton(2,myButtonWasPushed);
```

The only requirements for using this library are to 
   * include the library header file, 
   * to start up the Serial comms interface and to 
   * register the instance in setup()
   * put a line of code somewhere in the "loop()" function:
```
#include "gspswitch.h"

gspSwitch myToggleSwitch(1,"Switch is Off","Switch is On");
gspSwitch myOtherToggleSwitch(1,"Switch is Off","Switch is On");

void setup() {
  Serial.begin(115200);
  gspGrouped::register_instance(&myToggleSwitch); // you need one of these for each gspSwitch
  gspGrouped::register_instance(&myOtherToggleSwitch); // you need one of these for each gspSwitch
}

void loop() {

...
  
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
