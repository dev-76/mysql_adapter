#include "mysql_result_set.h"

MySQLResultSet::MySQLResultSet() : m_RowsCount(0), m_FieldsCount(0), m_CurrentRowIndex(0) {
    m_Rows.clear();
}

MySQLResultSet::MySQLResultSet(MYSQL_RES * pResult) : m_RowsCount(0), m_FieldsCount(0), m_CurrentRowIndex(0) {
    m_RowsCount = mysql_num_rows(pResult);
    m_FieldsCount = mysql_num_fields(pResult);
    MYSQL_ROW row;
    unsigned long* fieldLenghts;
    while(row = mysql_fetch_row(pResult)) {
        fieldLenghts = mysql_fetch_lengths(pResult);
        m_Rows.push_back(std::make_shared<MySQLRow>(*(new MySQLRow(row, fieldLenghts, m_FieldsCount))));
    }
    m_CurrentRow = nullptr;
}

MySQLResultSet::~MySQLResultSet() {
    if(m_RowsCount) {
        for(auto it = m_Rows.begin(); it != m_Rows.end(); it++)
            (*it)->~MySQLRow();
        m_Rows.clear();
    }
}

std::shared_ptr<MySQLRow> MySQLResultSet::Fetch() {
    if(m_CurrentRowIndex >= m_RowsCount) {
        std::shared_ptr<MySQLRow> empty = nullptr;
        return empty;
    }
    return m_Rows[m_CurrentRowIndex++];
}

bool MySQLResultSet::Empty() {
    return m_RowsCount == 0;
}

void MySQLResultSet::Reset() {
    m_RowsCount = m_FieldsCount = m_CurrentRowIndex = 0;
    m_Rows.clear();
}

void MySQLResultSet::ResetIter() {
    m_CurrentRowIndex = 0;
}
