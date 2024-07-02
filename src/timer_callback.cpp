#include "timer_callback.hpp"
#include "DJI_motor.hpp"
#include "LPF.hpp"
#include "ch110.hpp"
#include "clamp.hpp"
#include "error_handle.hpp"
#include "init.hpp"
#include "pid.hpp"
#include "vision.hpp"

extern float yaw_target_euler;       //YAW轴目标欧拉角
extern float pitch_target_euler;     //PITCH轴目标欧拉角
extern float yaw_target_vision;      //YAW轴视觉目标
extern float pitch_target_vision;    //PITCH轴视觉目标
extern int16_t friction_target_rpm;  //摩擦轮目标转速
extern int32_t trigger_target_pos;   //拨盘目标位置

/**
 * @brief 1KHz的定时器回调
 * 
 */
void TimerPeriodCallback() {

    static auto tick = 0;

    if (tick == 1000) {
        ErrorDetection();  //错误检测
        tick = 0;
    } else {
        tick++;
    }

    /*  =========================== 摩擦轮、拨盘电机控制 ===========================  */

    //摩擦轮转速PID
    auto measure_rpm_201 = lpf_rpm_201.Compute(dji_motor_201.get_actual_rpm());
    auto dji_motor_201_input = pid_vel_201.Compute(friction_target_rpm, measure_rpm_201);
    dji_motor_201.set_input(dji_motor_201_input);  //设置电机输出

    //摩擦轮转速PID
    auto measure_rpm_202 = lpf_rpm_202.Compute(dji_motor_202.get_actual_rpm());
    auto dji_motor_202_input = pid_vel_202.Compute(friction_target_rpm, measure_rpm_202);
    dji_motor_202.set_input(dji_motor_202_input);  //设置电机输出

    //拨盘外环PID
    auto measure_pos_203 = dji_motor_203.get_encoder_integral();
    auto temp_203 = pid_pos_203.Compute(trigger_target_pos, measure_pos_203);

    //拨盘内环PID
    auto measure_rpm203 = lpf_rpm_203.Compute(dji_motor_203.get_actual_rpm());
    auto dji_motor_203_input = pid_vel_203.Compute(temp_203, measure_rpm203);
    dji_motor_203.set_input(dji_motor_203_input);  //设置电机输出

    /*  =========================== YAW、PITCH电机控制 ===========================  */

    //视觉启动
    if (vision.get_use() == true) {

        //YAW外环PID
        auto vision_temp_205 = vision_pid_pos_205.Compute(yaw_target_vision);

        //YAW内环PID
        //@develop 第一次调试时需检查对应的角速度轴是否正确
        auto vision_measure_z_velocity = ch110.get_z_velocity();
        auto vision_dji_motor_205_input =
            vision_pid_vel_205.Compute(vision_temp_205, vision_measure_z_velocity);
        dji_motor_205.set_input(vision_dji_motor_205_input);

        //PITCH外环PID
        auto vision_temp_206 = vision_pid_pos_206.Compute(pitch_target_vision);

        //PITCH内环PID
        //@develop 第一次调试时需检查对应的角速度轴是否正确
        auto vision_measure_y_velocity = ch110.get_y_velocity();
        auto vision_dji_motor_206_input =
            vision_pid_vel_206.Compute(vision_temp_206, vision_measure_y_velocity);
        vision_dji_motor_206_input += EGC.Compute();  //加入重力补偿
        dji_motor_206.set_input(vision_dji_motor_206_input);

        //视觉使用期间基于欧拉角控制的目标值得等于实际值，防止解除视觉模式时云台回到原目标值导致失控假象
        yaw_target_euler = ch110.get_yaw_integral();
        pitch_target_euler = clamp(ch110.get_pitch(), -5.0f, 30.0f);
    }
    //视觉未启动
    else {

        //YAW外环PID
        auto euler_error_205 = yaw_target_euler - ch110.get_yaw_integral();
        euler_error_205 = clamp(euler_error_205, -180.0f, 180.0f);  //限幅目标值和实际值差值最多一圈
        auto temp_205 = pid_pos_205.Compute(euler_error_205);

        //YAW内环PID
        //@develop 第一次调试时需检查对应的角速度轴是否正确
        auto measure_z_velocity = ch110.get_z_velocity();
        auto dji_motor_205_input = pid_vel_205.Compute(temp_205, measure_z_velocity);
        dji_motor_205.set_input(dji_motor_205_input);

        //PITCH外环PID
        auto euler_error_206 =
            pitch_target_euler - clamp(ch110.get_pitch(), -5.0f, 30.0f);  //俯仰角限制
        auto temp_206 = pid_pos_206.Compute(euler_error_206);

        //PITCH内环PID
        //@develop 第一次调试时需检查对应的角速度轴是否正确
        auto measure_y_velocity = ch110.get_y_velocity();
        auto dji_motor_206_input = pid_vel_206.Compute(temp_206, measure_y_velocity);
        dji_motor_206_input += EGC.Compute();  //加入重力补偿
        dji_motor_206.set_input(dji_motor_206_input);
    }

    DjiMotorSend();  //电机控制报文发送
}