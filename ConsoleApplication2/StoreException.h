#pragma once
#include <iostream>
#include <string>
class StoreException : public std::runtime_error
{

public:
    enum ExceptionType {
        ZERO_PARAM,         // нулевые параметры
        TIME_EXCEEDED,      // превышено время мониторинга  
        SOLD_OUT,           // товар продан
        INVALID_PRICE,      // неверная цена
        PRICE_SURGE,        // резкий скачок цены
        LOW_STOCK_ALERT,     // критически низкий остаток
        INVALID_URL,
        LITTLE_DATA
    };
    ExceptionType type;
    StoreException(const ExceptionType t, const std::string& msg);
};

