#include "MainTask.h"
#include "Arduino.h"
#include "MsgService.h"
#include "Button.h"
#include "Potentiometer.h"
#include "config.h"
#include <avr/sleep.h>
#include <avr/power.h>


MainTask :: MainTask(int pinPot, int pinBut) {
  this->pinPot = pinPot;
  this->pinButton = pinBut;
  this->firstTimeReady = 0;
}

void MainTask :: init(int period) {
  Task::init(period);
  pot = new Potentiometer(pinPot, 0, 5);
  button = new Button(pinButton);
  state = MainState::STANDBY;
  MsgService.init();
  prevPotValue = pot->getValue();
}

void MainTask :: tick() {

  if (state == MainState::STANDBY) {
#ifdef __DEBUG__
    Serial.println("[MainTask]STANDBY");
#endif
    reStart = false;
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
    Serial.println("READY");
#endif
    int newValue = pot->getValue();
    if (prevPotValue != newValue) {
      prevPotValue = newValue;
      MsgService.sendMsg(String(prevPotValue));
    }
    if (button->isPressed()) {
#ifdef __DEBUG__
      Serial.println("[MAINTASK]Bottone premuto, fai il caffe");
#endif
      newCoffe = true;
      state = MainState::MAKECOFFE;
      MsgService.sendMsg("m");
    }
    if (!correctDistance) { //potrebbe verificarsi un errore
#ifdef __DEBUG__
      Serial.println("[MAINTASK]La distanza non è corretta, torno ad ON");
#endif
      state = MainState::ON;
      MsgService.sendMsg("t");
    }
    return;
  }

  if (state == MainState::MAKECOFFE) {
#ifdef __DEBUG__
    Serial.println("MAKECOFFE");
#endif
    if (coffeReady) {
#ifdef __DEBUG__
      Serial.println("[MAINTASK]il caffè è pronto, puoi prenderlo");
#endif
      MsgService.sendMsg("r");
      numCoffe--;
      state = MainState::TAKECOFFE;
    }
    return;
  }

  if (state == MainState::TAKECOFFE) {
#ifdef __DEBUG__
    Serial.println("TAKECOFFE");
    Serial.println("[MAINTASK]puoi prendere il caffe");
#endif
    if (numCoffe <= 0) {
#ifdef __DEBUG__
      Serial.println("[MAINTASK]non ci sono più caffe->mant");
#endif
      state = MainState::MAINTENANCE;
      MsgService.sendMsg("n");
      maintenanceActive = true;
      coffeTaked = false;
      return;
    }
    else if (coffeTaked) {
#ifdef __DEBUG__
      Serial.println("[MAINTASK]Hai preso il caffe");
#endif
      state = MainState::READY;
      MsgService.sendMsg("w");
      coffeTaked = false;
    }
  }

  if (state == MainState::MAINTENANCE) {
#ifdef __DEBUG__
    Serial.println("MAINTENANCE");
#endif
    if (MsgService.isMsgAvailable()) {
      Msg* msg = MsgService.receiveMsg();
      if (msg->getContent() == "j") {
        numCoffe = 10;
      }
      delete msg;
    }
    if (numCoffe > 0) {
#ifdef __DEBUG__
      Serial.println("[MAINTASK]Hai fatto fillup, stato->ready");
#endif
      state = MainState::STANDBY;
      maintenanceActive = false;
      reStart = true;
    }
  }


}
