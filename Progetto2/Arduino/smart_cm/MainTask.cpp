#include "MainTask.h"
#include "Arduino.h"
#include "MsgService.h"
#include "Button.h"
#include "Potentiometer.h"
#include <avr/sleep.h>
#include <avr/power.h>

MainTask :: MainTask(int pinPot, int pinBut) {
  this->pinPot = pinPot;
  this->pinButton = pinBut;
}

void MainTask :: init(int period) {
  Task::init(period);
  /* pot = new Potentiometer(pinPot, 0, 5);*/
  button = new Button(pinButton);
  state = MainState::STANDBY;
  MsgService.init();
  prevPotValue = pot->getValue();
}

void MainTask :: tick() {

  if (state == MainState::STANDBY) {
    Serial.println("STANDBY");
    reStart = false;
    if (!isPresent) {
      Serial.println("[MAINTASK]Entro in modalità riposo");
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
      Serial.println("[MAINTASK]Cambio stato perchè ho rilevato la presenza");
      state = MainState::ON;
    }
    return;
  }

  if (state == MainState::ON) {
    Serial.println("ON");
    if (correctDistance) {
      Serial.println("[MAINTASK]La distanza è corretta, cambio stato");
      state = MainState::READY;
      MsgService.sendMsg("w");
    } else if (!isPresent) {
      Serial.println("[MAINTASK]non c'è più nessuno");
      state = MainState::STANDBY;
    }
    return;
  }

  if (state == MainState::READY) {
    Serial.println("READY");
    /* if (prevPotValue != pot->getValue()) {
       prevPotValue = pot->getValue();
       Serial.println("Valore zucchero:" + String(prevPotValue));
      }*/
    if (button->isPressed()) {
      Serial.println("[MAINTASK]Bottone premuto, fai il caffe");
      newCoffe = true;
      state = MainState::MAKECOFFE;
      MsgService.sendMsg("m");
    }
    if (!correctDistance) {
      Serial.println("[MAINTASK]La distanza non è corretta, torno ad ON");
      state = MainState::ON;
      MsgService.sendMsg("t");
    }
    return;
  }

  if (state == MainState::MAKECOFFE) {
    Serial.println("MAKECOFFE");
    if (coffeReady) {
      MsgService.sendMsg("r");
      Serial.println("[MAINTASK]il caffè è pronto, puoi prenderlo");
      numCoffe--;
      state = MainState::TAKECOFFE;
      MsgService.sendMsg("t");
    }
    return;
  }

  if (state == MainState::TAKECOFFE) {
    Serial.println("TAKECOFFE");
    Serial.println("[MAINTASK]puoi prendere il caffe");
    if (numCoffe <= 0) {
      Serial.println("[MAINTASK]non ci sono più caffe->mant");
      state = MainState::MAINTENANCE;
      MsgService.sendMsg("n");
      maintenanceActive = true;
      return;
    }
    else if (coffeTaked) {
      Serial.println("[MAINTASK]Hai preso il caffe");
      state = MainState::READY;
      MsgService.sendMsg("w");
      coffeTaked = false;
    }
  }

  if (state == MainState::MAINTENANCE) {
    Serial.println("MAINTENANCE");
    if (MsgService.isMsgAvailable()) {
      Msg* msg = MsgService.receiveMsg();
      if (msg->getContent() == "j") {
        numCoffe = 10;
      }
      delete msg;
    }
    if (numCoffe > 0) {
      Serial.println("[MAINTASK]Hai fatto fillup, stato->ready");
      state = MainState::STANDBY;
      maintenanceActive = false;
      reStart = true;
    }
  }


}
