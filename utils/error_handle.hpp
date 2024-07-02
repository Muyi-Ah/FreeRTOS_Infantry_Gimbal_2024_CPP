#ifndef ERROR_HANDLE_H
#define ERROR_HANDLE_H

enum ErrorType {
    kPointerError,
    kHalLibError,
    kSwitchError,
    kMotorError,
    kDr16Error,
    kImuError,
    kComuError,
    kVisionError,
};

void ErrorHandle(enum ErrorType error_type);
void CheckPointer(void* ptr);
void ErrorDetection();

#endif  // ERROR_HANDLE_H