#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

#define PULLUP false
#define INVERT false
#define DEBOUNCE 20

SmartBlaster::SmartBlaster (uint8_t magSizes[]) :
  _display(OLED_RESET),
  _swCntBtn(4, PULLUP, INVERT, DEBOUNCE),
  _reloadBtn(7, PULLUP, INVERT, DEBOUNCE),
  _magSzTogBtn(8, PULLUP, INVERT, DEBOUNCE)  {

    _isChrono = false;
    _isVoltmeter = false;
    _isSelectFire = false;

    initMagSizes(magSizes);
}

void SmartBlaster::init(bool isAmmoCounter, bool isIRGate bool isChrono, bool isVoltMeter) {
  _isAmmoCounter = isAmmoCounter;
  _isIRGate = isIRGate;
  _isChrono = isChrono;
  _isVoltmeter = isVoltMeter;

	initDisplay();
	initAmmoForDisplay(true);
}

void SmartBlaster::smartMyBlaster() {
  countAmmo();
  reload();
  toggleMagSizes();
  printVals();
}

uint8_t SmartBlaster::reload () {
  _reloadBtn.read();    //read button, using Button library

  //if button pressed, reload
  if (_reloadBtn.wasPressed()) {
    _currentAmmo = _maxAmmo;
    initAmmoForDisplay(true);      //display new ammo
  }

  return _currentAmmo;
}

//toggle between magazine sizes
uint8_t SmartBlaster::countAmmo () {
    if (!_isChrono && !isIRGate && _isAmmoCounter) {
      _swCntBtn.read();
      if (_swCntBtn.wasPressed()) {
        if (_maxAmmo != 0 && _currentAmmo < 99) {  //make sure that the ammo is less than 99 so it doesnt overflow the display and not in count-up mode
          currentAmmo--;    //increment ammo
        } else if (_maxAmmo == 0 && _currentAmmo > 0) { //make sure that the ammo is more than 0 so no negative numbers are displayed and in count-up mode
          _currentAmmo++;    //decrement ammo
        }
      }
    }

    return _currentAmmo;
}

//toggle between magazine sizes
uint8_t SmartBlaster::toggleMagSizes () {
    _magSzTogBtn.read(); //read button, using Button library

    //if button was pressed, toggle size
    if (_magSzTogBtn.wasPressed()) {
      //make sure mag sizes dont overflow
      //if it does, it goes back to the first mag size
      //if it doesn't, then goes to next mag size
      //if it doesn't, then goes to next mag size
      _currentMagSize = (_currentMagSize == _numOfMagSizes - 1) ? 0 : _currentMagSize + 1;
      _maxAmmo = _magSizes[_currentMagSize];
      _currentAmmo = _maxAmmo;
      initAmmoForDisplay(true);

    }

    return _maxAmmo;
}




void SmartBlaster::initMagSizes (uint8_t magSizes[]) {
    _magSizes = magSizes;

	_currentMagSize = 0;
	_maxAmmo = _magSizes[_currentMagSize];
	_currentAmmo = _maxAmmo;
}

void SmartBlaster::initDisplay () {
    _display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    _display.clearDisplay();
}

//helper function to display ammo. Initializes value to be passed displayed on display
void SmartBlaster::initAmmoForDisplay (bool toPrint) {
    String ammoToPrintBuffer = (_currentAmmo < 10 ? "0" : "") + (String)_currentAmmo;    //determine whether to insert 0 at the beginning of ammo

    Serial.print("ammo is ");
    Serial.println(ammoToPrintBuffer);

    ammoToPrintBuffer.toCharArray(_ammoToPrint, 3);	 //string to char arr

    if (toPrint) printVals();   //print vals based on whether to print them from this method 
}

void SmartBlaster::printVals() {
    String toPrint = ((String)(8) + "");

    _display.clearDisplay(); //clear the display, so the stuff that was here before is no longer here
    _display.setTextSize(6);  //set the size of the text
    _display.setTextColor(WHITE);    //set the color of text text
    _display.setCursor(0, 0);  //center text
    _display.print(toPrint);    //print the text

    _display.display(); //display the text

}
