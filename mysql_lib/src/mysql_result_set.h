#ifndef MYSQL_RESULT_SET_H
#define MYSQL_RESULT_SET_H

#include <memory>
#include <vector>
#include "common_mysql.h"
#include "mysql_row.h"

class MySQLResultSet {
public:
    MySQLResultSet();
    MySQLResultSet(MYSQL_RES* pResult);
    ~MySQLResultSet();

    std::shared_ptr<MySQLRow> Fetch();
    bool Empty();

    void Reset();
    void ResetIter();

private:
    std::vector<std::shared_ptr<MySQLRow>> m_Rows;
    std::shared_ptr<MySQLRow> m_CurrentRow;
    unsigned long m_CurrentRowIndex;
    unsigned long long m_RowsCount;
    unsigned long m_FieldsCount;
};

#endif // !MYSQL_RESULT_SET_H
