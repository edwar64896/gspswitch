#include "Arduino.h"
#include "gspswitch.h"

gspGrouped* gspSwitch::firstInstance=nullptr;

//default constructor.
//nothing will happen with this.
gspSwitch::gspSwitch()
:gspGrouped() {
	//_callback_off = nullptr;
//	_callback_on = nullptr;
	_strOn = nullptr;
	_strOff = nullptr;
	_switchMode = gspSwitch_MODE_UNCONFIGURED;
}

//2-way toggle switch constructor
//with callbacks
gspSwitch::gspSwitch(uint8_t pin, 
nonstd::function<void ()> cb_on, /*callback to invoke upon successful parse*/
nonstd::function<void ()> cb_off, /*callback to invoke upon successful parse*/
uint8_t mode=0)
:gspGrouped() {
	_callback_off = cb_off;
	_callback_on = cb_on;
	_pin = pin;
	pinMode(_pin, INPUT_PULLUP);
	switch (mode) {
		case 0:
			_switchMode = gspSwitch_MODE_SWITCH_CB;
			break;
		case 1:
			_switchMode = gspSwitch_MODE_PUSHBUTTON_LATCH_CB;
			break;
		default:
			_switchMode = gspSwitch_MODE_UNCONFIGURED;
		break;
	}
}

//2-way toggle switch constructor with strings
gspSwitch::gspSwitch(uint8_t pin, const char * strOn, const char * strOff,uint8_t mode = 0)
:gspGrouped() {
	_strOff = strOff;
	_strOn = strOn;
	_pin = pin;
	pinMode(_pin, INPUT_PULLUP);
	switch (mode) {
		case 0:
			_switchMode = gspSwitch_MODE_SWITCH_STR;
			break;
		case 1:
			_switchMode = gspSwitch_MODE_PUSHBUTTON_LATCH_STR;
			break;
		default:
			_switchMode = gspSwitch_MODE_UNCONFIGURED;
		break;
	}
}

//pushbutton constructor
//with callback
gspSwitch::gspSwitch(uint8_t pin, 
nonstd::function<void ()> cb_on, /*callback to invoke upon successful parse*/
uint8_t mode)
:gspGrouped() {
	_callback_on = cb_on;
//	_callback_off = nullptr;
	_pin = pin;
	pinMode(_pin, INPUT_PULLUP);
	switch (mode) {
		case 0:
			_switchMode = gspSwitch_MODE_PUSHBUTTON_RELEASE_CB;
			break;
		case 1:
			_switchMode = gspSwitch_MODE_PUSHBUTTON_PUSH_CB;
			break;
		case 2:
			_switchMode = gspSwitch_MODE_PUSHBUTTON_CONTINUOUS_CB;
			break;
		default:
			_switchMode = gspSwitch_MODE_UNCONFIGURED;
			break;
	}
}

//pushbutton constructor with string
gspSwitch::gspSwitch(uint8_t pin, const char * strOn, uint8_t mode)
:gspGrouped() {
	_strOn = strOn;
	_strOff = nullptr;
	_pin = pin;
	pinMode(_pin, INPUT_PULLUP);
	switch (mode) {
		case 0:
			_switchMode = gspSwitch_MODE_PUSHBUTTON_RELEASE_STR;
			break;
		case 1:
			_switchMode = gspSwitch_MODE_PUSHBUTTON_PUSH_STR;
			break;
		case 2:
			_switchMode = gspSwitch_MODE_PUSHBUTTON_CONTINUOUS_STR;
			break;
		default:
			_switchMode = gspSwitch_MODE_UNCONFIGURED;
			break;
	}
}


gspSwitch::gspSwitch(uint8_t pin, 
	nonstd::function<void ()> cb_on, 
	nonstd::function<void ()> cb_off, 
	nonstd::function<void ()> cb_depress)
	:gspGrouped()  {
		_pin = pin;
		pinMode(_pin, INPUT_PULLUP);
		_switchMode=gspSwitch_MODE_PUSHBUTTON_TIMEBASED_CB;
		_callback_on=cb_on;
		_callback_off=cb_off;
		_callback_depress=cb_depress;
}


gspSwitch::gspSwitch(uint8_t pin, 
	const char * strOn, 
	const char * strOff, 
	const char * strDepress)
	:gspGrouped() {
		_pin = pin;
		pinMode(_pin, INPUT_PULLUP);
		_switchMode=gspSwitch_MODE_PUSHBUTTON_TIMEBASED_STR;
		_strOn=strOn;
		_strOff=strOff;
		_strDepress=strDepress;

}

gspSwitch::gspSwitch(uint8_t pin, 
	nonstd::function<void ()> cb_on, 
	nonstd::function<void ()> cb_off, 
	nonstd::function<void ()> cb_depress, 
	nonstd::function<void ()> cb_depress2)
	:gspGrouped()  {
		_pin = pin;
		pinMode(_pin, INPUT_PULLUP);
		_switchMode=gspSwitch_MODE_PUSHBUTTON_TIMEBASED2_CB;
		_callback_on=cb_on;
		_callback_off=cb_off;
		_callback_depress=cb_depress;
		_callback_depress2=cb_depress2;
}


gspSwitch::gspSwitch(uint8_t pin, 
	const char * strOn, 
	const char * strOff, 
	const char * strDepress,
	const char * strDepress2)
	:gspGrouped() {
		_pin = pin;
		pinMode(_pin, INPUT_PULLUP);
		_switchMode=gspSwitch_MODE_PUSHBUTTON_TIMEBASED2_STR;
		_strOn=strOn;
		_strOff=strOff;
		_strDepress=strDepress;
		_strDepress2=strDepress2; 
}


gspSwitch::gspSwitch(uint8_t pin, 
	uint8_t maxStates,
	nonstd::function<void (uint8_t)> cb_changestate,
	uint8_t mode = 0) {
		_pin = pin;
		pinMode(_pin, INPUT_PULLUP);
		_maxCounter = maxStates;
		_callback_changestate = cb_changestate;
		switch (mode) {
			case 0:
				_switchMode = gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB;
				break;
			case 1:
				_switchMode = gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_MANUALRESET_CB;
				break;
			default:
				_switchMode = gspSwitch_MODE_UNCONFIGURED;
				break;
	}
	
}
    
    //resets the counter - only valid with this mode/constructor
void gspSwitch::counterReset() {
	if (_switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_MANUALRESET_CB || _switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB ) {
		if (_counter!=0) {
			_autoResetCounter=0;
			_counter=0;
			_callback_changestate(_counter);
		}
	}
}

void gspSwitch::counterIncrement() {
	if (_switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_MANUALRESET_CB || _switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB ) {
		_autoResetCounter=0;
		_counter = (_counter + 1) % _maxCounter;
		_callback_changestate(_counter);
	}
}

uint8_t gspSwitch::getCounterValue() {
	if (_switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_MANUALRESET_CB || _switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB ) {
		return _counter;
	} else {
		return 0;
	}
}

void gspSwitch::counterAutoResetHoldoff() {
	_autoResetCounter=0;
}

gspSwitch::~gspSwitch() {}

uint8_t gspSwitch::getState() {
	return _switchState;
}

void gspSwitch::debugPrint(int pin) {
#ifdef DEBUG
	Serial.print("Switch:");
	Serial.print(getInstanceId());
	Serial.print(" Pin:");
	Serial.println(pin);
#endif
}

// instance method to check this instance (called from loop)
bool gspSwitch::check() {
	int drVal=digitalRead(_pin);
	switch (_switchMode) {
	case gspSwitch_MODE_SWITCH_CB:
		if (drVal != _old_s1) {
			debugPrint(_pin);	
			if (drVal) {
				_callback_off();
				_switchState=gspSwitch::Off;
			} else {
				_callback_on();
				_switchState=gspSwitch::On;
			}
			_old_s1 = drVal;
		}
		break;
	case gspSwitch_MODE_SWITCH_STR:
		if (drVal != _old_s1) {
			debugPrint(_pin);	
			if (drVal) {
				gspGrouped::gspStream.println(_strOff);
				_switchState=gspSwitch::Off;
			} else {
				gspGrouped::gspStream.println(_strOn);
				_switchState=gspSwitch::On;
			}
			_old_s1 = drVal;
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_MANUALRESET_CB:
	case gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB:
	case gspSwitch_MODE_PUSHBUTTON_RELEASE_CB:

		if (!drVal) {

			_s1 = 1; _s2 = 1;
			_switchState=gspSwitch::On;

		} else {

			_s1 = 0;
			_switchState=gspSwitch::Off;
			if ((_switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB) && _counter) {
				if (_autoResetCounter++ > gspSwitch_AUTORESET_CAP) {
					counterReset();
					_autoResetCounter=0;
				}
			}
		}

		if (_s1 == 0 && _s2) {

			debugPrint(_pin);	
			if (_switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_CB) {
				_callback_on();
			}
			if (_switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_MANUALRESET_CB || _switchMode == gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB) {
				counterIncrement();				
			}

			_s2 = 0;

		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_RELEASE_STR:
		if (!drVal) {
			_s1 = 1;
			_s2 = 1;
			_switchState=gspSwitch::On;
		} else {
			_s1 = 0;
			_switchState=gspSwitch::Off;
		}

		if (_s1 == 0 && _s2) {
			debugPrint(_pin);	
			gspGrouped::gspStream.println(_strOn);
			_s2 = 0;
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_PUSH_CB:
		if (!drVal) {
			_s1count=0;
			if (!_s1) {
				_switchState=gspSwitch::On;
				debugPrint(_pin);	
				_callback_on();
				_s1=1;
			}
		} else {
			_switchState=gspSwitch::Off;

			//debounce
			if (_s1count++>gspSwitch_DEBOUNCE_COUNT) {
				_s1 = 0;
				_s1count=0;
			}
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_PUSH_STR:
		if (!drVal) {
			_s1count=0;
			if (!_s1) {
				_switchState=gspSwitch::On;
				debugPrint(_pin);	
				gspGrouped::gspStream.println(_strOn);
				_s1=1;
			}
		} else {
			_switchState=gspSwitch::Off;

			//debounce
			if (_s1count++>gspSwitch_DEBOUNCE_COUNT) {
				_s1 = 0;
				_s1count=0;
			}
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_LATCH_CB:
		if (!drVal) {
			_s1count=0;
			if (!_s1) {
				if (_switchState==gspSwitch::On) {
					_switchState=gspSwitch::Off;
					_callback_off();
				} else {
					_switchState=gspSwitch::On;				
					_callback_on();
				}
				debugPrint(_pin);	
				_s1=1;
			}
		} else {
			//debounce
			if (_s1count++>gspSwitch_DEBOUNCE_COUNT) {
				_s1 = 0;
				_s1count=0;
			}
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_LATCH_STR:
		if (!drVal) {
			_s1count=0;
			if (!_s1) {
				if (_switchState==gspSwitch::On) {
					_switchState=gspSwitch::Off;
					gspGrouped::gspStream.println(_strOff);
				} else {
					_switchState=gspSwitch::On;				
					gspGrouped::gspStream.println(_strOn);
				}
				debugPrint(_pin);	
				_s1=1;
			}
		} else {
			//debounce
			if (_s1count++>gspSwitch_DEBOUNCE_COUNT) {
				_s1 = 0;
				_s1count=0;
			}
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_CONTINUOUS_CB:
		if (!drVal) {
			debugPrint(_pin);	
			_switchState=gspSwitch::On;
			_callback_on();
		} else {
			_switchState=gspSwitch::Off;			
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_CONTINUOUS_STR:
		if (!drVal) {
			debugPrint(_pin);	
			_switchState=gspSwitch::On;
			gspGrouped::gspStream.println(_strOn);
		} else {
			_switchState=gspSwitch::Off;
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_TIMEBASED_CB:
		if (!drVal) {
			_s1 = 1;
			_s2 = 1;
			_s3++;
		} else {
			_s1 = 0;
		}

		if (_s1 == 0 && _s2) {
			debugPrint(_pin);	
			if (_s3 >= _SW_BAND_2) {
					_switchState=gspSwitch::Mode1;
					_callback_depress();
			} else if (_s3 >= _SW_BAND_1) {
				if (_switchState!=gspSwitch::Off)
				{
					_switchState=gspSwitch::Off;
					_callback_off();
				}
				else if (_switchState==gspSwitch::Off)
				{
					_switchState=gspSwitch::On;
					_callback_on();
				}
			}
			_s3 = 0;
			_s2 = 0;
		}
		break;

	case gspSwitch_MODE_PUSHBUTTON_TIMEBASED_STR:
		if (!drVal) {
			_s1 = 1;
			_s2 = 1;
			_s3++;
		} else {
			_s1 = 0;
		}

		if (_s1 == 0 && _s2) {
			debugPrint(_pin);	
			if (_s3 >= _SW_BAND_2) {
					_switchState=gspSwitch::Mode1;
					gspGrouped::gspStream.println(_strDepress);
			} else if (_s3 >= _SW_BAND_1) {
				if (_switchState!=gspSwitch::Off)
				{
					_switchState=gspSwitch::Off;
					gspGrouped::gspStream.println(_strOff);
				}
				else if (_switchState==gspSwitch::Off)
				{
					_switchState=gspSwitch::On;
					gspGrouped::gspStream.println(_strOn);
				}
			}
			_s3 = 0;
			_s2 = 0;
		}
		break;
	case gspSwitch_MODE_PUSHBUTTON_TIMEBASED2_CB:
		if (!drVal) {
			_s1 = 1;
			_s2 = 1;
			_s3++;
		} else {
			_s1 = 0;
		}

		if (_s1 == 0 && _s2) {
			debugPrint(_pin);	
			if (_s3 >= _SW_BAND_3) {
					_switchState=gspSwitch::Mode2;
					_callback_depress2();
			} else if (_s3 >= _SW_BAND_2) {
					_switchState=gspSwitch::Mode1;
					_callback_depress();
			} else if (_s3 >= _SW_BAND_1) {
				if (_switchState!=gspSwitch::Off)
				{
					_switchState=gspSwitch::Off;
					_callback_off();
				}
				else if (_switchState==gspSwitch::Off)
				{
					_switchState=gspSwitch::On;
					_callback_on();
				}
			}
			_s3 = 0;
			_s2 = 0;
		}
		break;
	case gspSwitch_MODE_PUSHBUTTON_TIMEBASED2_STR:
		if (!drVal) {
			_s1 = 1;
			_s2 = 1;
			_s3++;
		} else {
			_s1 = 0;
		}

		if (_s1 == 0 && _s2) {
			debugPrint(_pin);	
			if (_s3 >= _SW_BAND_3) {
					_switchState=gspSwitch::Mode2;
					gspGrouped::gspStream.println(_strDepress2);
			} else if (_s3 >= _SW_BAND_2) {
					_switchState=gspSwitch::Mode1;
					gspGrouped::gspStream.println(_strDepress);
			} else if (_s3 >= _SW_BAND_1) {
				if (_switchState!=gspSwitch::Off)
				{
					_switchState=gspSwitch::Off;
					gspGrouped::gspStream.println(_strOff);
				}
				else if (_switchState==gspSwitch::Off)
				{
					_switchState=gspSwitch::On;
					gspGrouped::gspStream.println(_strOn);
				}
			}
			_s3 = 0;
			_s2 = 0;
		}
		break;
	default:
		break;
	}

	return true;
}

