#pragma once
#include "stdint.h"

#include "DJI_motor.hpp"
#include "LPF.hpp"
#include "bsp_can.hpp"
#include "bsp_uart.hpp"
#include "ch110.hpp"
#include "communication.hpp"
#include "dr16.hpp"
#include "gravity_compensation.hpp"
#include "pid.hpp"
#include "state_machine.hpp"
#include "ude.hpp"
#include "vision.hpp"
#include "vofa.hpp"

extern PidController pid_vel_201;
extern PidController pid_vel_202;
extern PidController pid_pos_203;
extern PidController pid_vel_203;
extern PidController pid_pos_205;
extern PidController pid_vel_205;
extern PidController pid_pos_206;
extern PidController pid_vel_206;

extern PidController vision_pid_pos_205;
extern PidController vision_pid_vel_205;
extern PidController vision_pid_pos_206;
extern PidController vision_pid_vel_206;

extern DjiMotor dji_motor_201;
extern DjiMotor dji_motor_202;
extern DjiMotor dji_motor_203;
extern DjiMotor dji_motor_205;
extern DjiMotor dji_motor_206;

extern DjiMotor* dji_motor_list[];

extern CanManager can_motor;
extern CanManager can_ch110;

extern UartManager uart_remote;
extern UartManager uart_communication;
extern UartManager uart_vision;

extern StateMachine state_machine;
extern Communication communication;
extern Vofa vofa;
extern Vision vision;
extern CH110 ch110;
extern DR16 dr16;
extern LPF lpf_rpm_201;
extern LPF lpf_rpm_202;
extern LPF lpf_rpm_203;
extern EmpiricalGravityCompensator EGC;