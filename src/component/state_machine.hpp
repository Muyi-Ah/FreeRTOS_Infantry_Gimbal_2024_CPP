#pragma once
#include "stdint.h"

enum MainState {
    kMainStateNone,
    kOperate,
    kHalt,
};

enum SubState {
    kSubStateNone,
    kSubMode11,
    kSubMode12,
    kSubMode13,
    kSubMode21,
    kSubMode22,
    kSubMode23,
    kSubMode31,
    kSubMode32,
    kSubMode33,
};

enum Event {
    kEventEnterOperate,
    kEventEnterHalt,
    kEventSwitchSubMode11,
    kEventSwitchSubMode12,
    kEventSwitchSubMode13,
    kEventSwitchSubMode21,
    kEventSwitchSubMode22,
    kEventSwitchSubMode23,
    kEventSwitchSubMode31,
    kEventSwitchSubMode32,
    kEventSwitchSubMode33,
};

class StateMachine {
   private:
    enum MainState main_state_ = kMainStateNone;
    enum SubState sub_state_ = kSubStateNone;

   public:
    StateMachine(/* args */);
    ~StateMachine();

    auto get_main_state(){return main_state_;};
    auto get_sub_state(){return sub_state_;};

    void HandleEvent(enum Event event);
};