#ifndef MYSQL_FIELD_H
#define MYSQL_FIELD_H

#include <string>

class MySQLField {
public:
    MySQLField(char* data, size_t length);
    ~MySQLField();

    unsigned char ToUInt8() const;
    unsigned short ToUInt16() const;
    unsigned int ToUInt32() const;
    unsigned long long ToUInt64() const;
    char ToInt8() const;
    short ToInt16() const;
    int ToInt32() const;
    long long ToInt64() const;
    float ToFloat() const;
    double ToDouble() const;
    const char* ToCStr() const;
    std::string ToStr() const;

    bool IsNull();

private:
    char* m_RawData;
    bool m_Null;
};

#endif // !MYSQL_FIELD_H
