#include "mysql_row.h"

MySQLRow::MySQLRow(MYSQL_ROW row, unsigned long * fieldLenghts, unsigned int fieldsCount) {
    for(size_t i = 0; i < fieldsCount; i++) {
        m_Fields.push_back(new MySQLField(row[i], fieldLenghts[i]));
    }
    m_FieldsCount = fieldsCount;
}

MySQLRow::~MySQLRow()	{
    for(auto it = m_Fields.begin(); it != m_Fields.end(); it++) {
        if(*it)
            delete *it;
    }
}

void MySQLRow::Reset() {
    for(auto it = m_Fields.begin(); it != m_Fields.end(); it++) {
        if(*it)
            delete *it;
    }
    m_Fields.clear();
    m_FieldsCount = 0;
}

MySQLField* MySQLRow::operator[](size_t index) {
    if(index > m_FieldsCount - 1)
        return nullptr;
    return m_Fields[index];
}
