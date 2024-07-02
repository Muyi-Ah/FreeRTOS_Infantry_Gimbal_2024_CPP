#pragma once
#include "stdint.h"

class Communication {
   private:
   bool is_reply_;
    //@develop 根据需求来写
   public:
    Communication(/* args */);
    ~Communication();

    void DataUpdate(volatile const uint8_t* rx_buf);
    void Send();

    auto get_is_reply(){return is_reply_;};

    void set_is_reply(bool flag){is_reply_ = flag;};
};