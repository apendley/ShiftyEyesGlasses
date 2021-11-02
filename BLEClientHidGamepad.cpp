/**************************************************************************/
/*!
    @file     BLEClientHidGamepad.cpp
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

#include "bluefruit.h"
#include "BLEClientHidGamepad.h"

BLEClientHidGamepad::BLEClientHidGamepad(void)
 : BLEClientService(UUID16_SVC_HUMAN_INTERFACE_DEVICE),
   _hid_info(UUID16_CHR_HID_INFORMATION),
   _hid_report(UUID16_CHR_REPORT)

{
  _gamepad_cb = NULL;
  varclr(&_last_gamepad_report);
}

void gamepad_client_notify_cb(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  BLEClientHidGamepad& svc = (BLEClientHidGamepad&) chr->parentService();
  svc._handle_gamepad_input(data, len);
}

bool BLEClientHidGamepad::begin(void)
{
  // Invoke base class begin()
  BLEClientService::begin();

  _hid_info.begin(this);
  _hid_report.begin(this);

  // set notify callback
  _hid_report.setNotifyCallback(gamepad_client_notify_cb);  

  return true;
}

void BLEClientHidGamepad::setGamepadReportCallback(gamepad_callback_t fp)
{
  _gamepad_cb = fp;
}

bool BLEClientHidGamepad::discover(uint16_t conn_handle)
{
  // Call Base class discover
  VERIFY( BLEClientService::discover(conn_handle) );
  _conn_hdl = BLE_CONN_HANDLE_INVALID; // make as invalid until we found all chars

  // Discover all characteristics
  Bluefruit.Discovery.discoverCharacteristic(conn_handle, _hid_info, _hid_report);

  VERIFY( _hid_info.discovered(), _hid_report.discovered() );

  _conn_hdl = conn_handle;
  return true;
}

/*------------------------------------------------------------------*/
/* Info
 *------------------------------------------------------------------*/
bool BLEClientHidGamepad::getHidInfo(uint8_t info[4])
{
  return 4 == _hid_info.read(info, 4);
}

/*------------------------------------------------------------------*/
/* Gamepad
 *------------------------------------------------------------------*/
bool BLEClientHidGamepad::gamepadPresent() 
{
  return _hid_report.discovered();
}

bool BLEClientHidGamepad::enableGamepad() 
{
  return _hid_report.enableNotify();
}

bool BLEClientHidGamepad::disableGamepad() 
{
  return _hid_report.disableNotify();
}

void BLEClientHidGamepad::_handle_gamepad_input(uint8_t* data, uint16_t len)
{
  varclr(&_last_gamepad_report);
  memcpy(&_last_gamepad_report, data, len);

  if ( _gamepad_cb ) _gamepad_cb(&_last_gamepad_report);
}

void BLEClientHidGamepad::getGamepadReport(hid_gamepad_report_t* report)
{
  memcpy(report, &_last_gamepad_report, sizeof(hid_gamepad_report_t));
}
