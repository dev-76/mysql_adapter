#include "mysql_field.h"

#include <cstring>

MySQLField::MySQLField(char* data, size_t length)	{
    m_RawData = nullptr;
    if(data && length > 0) {
        m_RawData = new char[length + 1];
        memcpy(m_RawData, data, length);
        m_RawData[length] = '\0';
        m_Null = false;
    }
    else {
        m_Null = true;
    }
}

MySQLField::~MySQLField()	{
    if(m_RawData) {
        delete[] m_RawData;
        m_RawData = nullptr;
    }
}

unsigned char MySQLField::ToUInt8() const {
    return (unsigned char)*m_RawData;
}

unsigned short MySQLField::ToUInt16() const {
    return (unsigned short)atoi(m_RawData);
}

unsigned int MySQLField::ToUInt32() const {
    return (unsigned int)atoi(m_RawData);
}

unsigned long long MySQLField::ToUInt64() const {
    return (unsigned long long)atoll(m_RawData);
}

char MySQLField::ToInt8() const {
    return (char)*m_RawData;
}

short MySQLField::ToInt16() const {
    return (short)atoi(m_RawData);
}

int MySQLField::ToInt32() const {
    return atoi(m_RawData);
}

long long MySQLField::ToInt64() const {
    return atoll(m_RawData);
}

float MySQLField::ToFloat() const {
    return strtof(m_RawData, nullptr);
}

double MySQLField::ToDouble() const {
    return strtod(m_RawData, nullptr);
}

const char * MySQLField::ToCStr() const {
    return m_RawData;
}

std::string MySQLField::ToStr() const {
    return std::string(m_RawData);
}

bool MySQLField::IsNull() {
    return m_Null;
}
