#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <Button.h>

#include <StandardCplusplus.h>
#include <vector>


class SmartBlaster {
  private:
    //flags to know which modes are enabled
    uint8_t _isChrono;
    uint8_t _isVoltmeter;
    uint8_t _isSelectFire;

    //stuff to keep track of what to print for each different value
    char _ammoToPrint[2];
    char _chronoToPrint[6];
    String _voltageToPrint[4];
    String _fireModeToPrint[2];

    //keep track of ammo
    std::vector<int> _magSizes;
        // uint8_t _magSizes[];
    uint8_t _numOfMagSizes;
    uint8_t _currentMagSize;
    int _maxAmmo;
    int _currentAmmo;

    U8G2_SSD1306_128X64_NONAME_F_SW_I2C _u8g2;

    Button _triggerBtnArg;
    Button _magInsDetBtn;
    Button _magSzTogBtn;

    //init methods
    SmartBlaster initMagSizes(int magSizes[], int numOfMagSizes);

    //display methods
    SmartBlaster initDisplay();     //initialize display right when it's created
    SmartBlaster initAmmoForDisplay(bool toPrint);
    SmartBlaster printVals();   //print all values at the same time, including ammo, chrono vals, etc.

    //smart blaster funcitonality methods
    SmartBlaster reload ();  //reload
    SmartBlaster toggleMagSizes ();

  public:
    SmartBlaster(int magSizes[], int numOfMagSizes);
    SmartBlaster init ();
    SmartBlaster smartMyBlaster();

};

#endif
