/**************************************************************************/
/*!
    @file     BLEClientHidGamepad.h
    @author   Aaron Pendley

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2021, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef BLECLIENTHIDGAMEPAD_H_
#define BLECLIENTHIDGAMEPAD_H_

#include "bluefruit_common.h"
#include "BLEClientCharacteristic.h"
#include "BLEClientService.h"

class BLEClientHidGamepad : public BLEClientService
{
  public:
    // Callback Signature
    typedef void (*gamepad_callback_t ) (hid_gamepad_report_t* report);

    BLEClientHidGamepad(void);

    virtual bool  begin(void);
    virtual bool  discover(uint16_t conn_handle);

    bool     getHidInfo(uint8_t info[4]);    

    // Gamepad API
    bool gamepadPresent(void);
    bool enableGamepad();
    bool disableGamepad();

    void getGamepadReport(hid_gamepad_report_t* report);

    // Report callback
    void setGamepadReportCallback(gamepad_callback_t fp);

  protected:
    gamepad_callback_t _gamepad_cb;

    hid_gamepad_report_t _last_gamepad_report;

    BLEClientCharacteristic _hid_info;
    BLEClientCharacteristic _hid_report;

    void _handle_gamepad_input(uint8_t* data, uint16_t len);

    friend void gamepad_client_notify_cb(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len);
};

#endif /* BLECLIENTHIDGAMEPAD_H_ */
