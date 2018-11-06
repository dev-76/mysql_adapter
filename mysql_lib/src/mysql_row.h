#ifndef MYSQL_ROW_H
#define MYSQL_ROW_H

#include <memory>
#include <vector>

#include "common_mysql.h"
#include "mysql_field.h"

class MySQLRow {
public:
    MySQLRow(MYSQL_ROW row, unsigned long* fieldLenghts, unsigned int fieldsCount);
    ~MySQLRow();

    void Reset();
    MySQLField* operator[](size_t index);

private:
    std::vector<MySQLField*> m_Fields;
    unsigned int m_FieldsCount;
};

#endif // !MYSQL_ROW_H
