#include "UserHandler.h"
#include "Arduino.h"
#include "MsgService.h"
#include "Button.h"
#include "Potentiometer.h"
#include "config.h"
#include <avr/sleep.h>
#include <avr/power.h>


UserHandler :: UserHandler(int pinPot, int pinBut) {
  this->pinPot = pinPot;
  this->pinButton = pinBut;
  this->firstTimeReady = 0;
}

void UserHandler :: init(int period) {
  Task::init(period);
  pot = new Potentiometer(pinPot, 0, 5);
  button = new Button(pinButton);
  state = MainState::STANDBY;
  MsgService.init();
  prevPotValue = pot->getValue();
}

void UserHandler :: tick() {

  if (state == MainState::STANDBY) {
    reStart = false;
#ifdef __DEBUG__
    Serial.println("[MainTask]STANDBY");
#endif
    if (!isPresent) {
      set_sleep_mode(SLEEP_MODE_IDLE);
      sleep_enable();
      power_adc_disable();
      power_spi_disable();
      power_timer0_disable();
      power_timer2_disable();
      power_twi_disable();
      sleep_mode();
      sleep_disable();
      power_all_enable();
    } else {
      state = MainState::ON;
    }
    return;
  }

  if (state == MainState::ON) {
#ifdef __DEBUG__
    Serial.println("[MainTask]ON");
#endif
    if (correctDistance) {
      state = MainState::READY;
      MsgService.sendMsg("w");
    } else if (!isPresent) {
      MsgService.sendMsg("t");
      state = MainState::STANDBY;
    }
    return;
  }

  if (state == MainState::READY) {
#ifdef __DEBUG__
    Serial.println("[MainTask]READY");
#endif
    int newValue = pot->getValue();
    if (prevPotValue != newValue) {
      prevPotValue = newValue;
      MsgService.sendMsg(String(prevPotValue));
    } else if(firstTimeReady == 0){
      MsgService.sendMsg(String(prevPotValue));
      firstTimeReady = 1;
    }
    if (button->isPressed()) {
      newCoffe = true;
      state = MainState::MAKECOFFE;
      MsgService.sendMsg("m");
      firstTimeReady = 0;
    }else if (!correctDistance) { //potrebbe verificarsi un errore
      state = MainState::ON;
      firstTimeReady = 0;
      MsgService.sendMsg("t");
    }
    return;
  }

  if (state == MainState::MAKECOFFE) {
#ifdef __DEBUG__
    Serial.println("[MainTask]MAKECOFFE");
#endif
    if (coffeReady) {
      MsgService.sendMsg("r");
      numCoffe--;
      state = MainState::TAKECOFFE;
    }
    return;
  }

  if (state == MainState::TAKECOFFE) {
#ifdef __DEBUG__
    Serial.println("[MainTask]TAKECOFFE");
#endif
    if (numCoffe <= 0) {
      state = MainState::MAINTENANCE;
      MsgService.sendMsg("n");
      maintenanceActive = true;
      coffeTaked = false;
      return;
    }
    else if (coffeTaked) {
      state = MainState::READY;
      MsgService.sendMsg("w");
      coffeTaked = false;
#ifdef __DEBUG__
      Serial.println("[MAINTASK]Coffe taked");
#endif
    }
  }

  if (state == MainState::MAINTENANCE) {
    if (MsgService.isMsgAvailable()) {
      Msg* msg = MsgService.receiveMsg();
      if (msg->getContent() == "j") {
        numCoffe = 10;
#ifdef __DEBUG__
    Serial.println("[MainTask]MAINTENANCE");
#endif
      }
      delete msg;
    }
    if (numCoffe > 0) {
      state = MainState::STANDBY;
      maintenanceActive = false;
      reStart = true;
    }
  }


}
