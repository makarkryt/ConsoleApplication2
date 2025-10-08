#pragma once
#include <iostream>
#include <string>
class StoreException : public std::runtime_error
{

public:
    enum ExceptionType {
        ZERO_PARAM,         // ������� ���������
        TIME_EXCEEDED,      // ��������� ����� �����������  
        SOLD_OUT,           // ����� ������
        INVALID_PRICE,      // �������� ����
        PRICE_SURGE,        // ������ ������ ����
        LOW_STOCK_ALERT,     // ���������� ������ �������
        INVALID_URL,
        LITTLE_DATA
    };
    ExceptionType type;
    StoreException(const ExceptionType t, const std::string& msg);
};

