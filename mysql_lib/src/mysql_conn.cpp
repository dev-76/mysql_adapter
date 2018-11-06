#include "mysql_conn.h"
#include "utils.h"

MySQLConnection::MySQLConnection(const std::string& dbHost, const std::string& dbPort, const std::string& dbUser, const std::string& dbPassword, const std::string& dbName, bool logQueries,
	std::function<void(const std::string&)> debugTrace, std::function<void(const std::string&)> warningTrace, std::function<void(const std::string&)> errorTrace)
    : m_DatabaseHost(dbHost), m_DatabasePort(dbPort), m_DatabaseUser(dbUser), m_DatabasePassword(dbPassword), m_DatabaseName(dbName), m_Connected(false),
        m_LogQueries(logQueries), m_DebugTrace(debugTrace), m_WarningTrace(warningTrace), m_ErrorTrace(errorTrace) {}

MYSQL* MySQLConnection::m_GetConnectionNative() {
    return m_pMYSQL;
}

MySQLConnectionSync::MySQLConnectionSync(const std::string& dbHost, const std::string& dbPort, const std::string& dbUser, const std::string& dbPassword, const std::string& dbName, bool logQueries,
	std::function<void(const std::string&)> debugTrace, std::function<void(const std::string&)> warningTrace, std::function<void(const std::string&)> errorTrace)
    : MySQLConnection(dbHost, dbPort, dbUser, dbPassword, dbName, logQueries, debugTrace, warningTrace, errorTrace) {}

bool MySQLConnectionSync::Connect() {
    m_pMYSQL = mysql_init(NULL);
    mysql_options(m_pMYSQL, MYSQL_SET_CHARSET_NAME, "utf8");
    if(!mysql_real_connect(m_pMYSQL, m_DatabaseHost.c_str(), m_DatabaseUser.c_str(), m_DatabasePassword.c_str(), m_DatabaseName.c_str(),
                            m_DatabasePort == "" ? 0 : std::atoi(m_DatabasePort.c_str()), NULL, 0)) {
        m_ErrorTrace(FmtStr("Error when connect to mysql: %s", mysql_error(m_pMYSQL)));
        return false;
    }
    mysql_query(m_pMYSQL, "SET CHARACTER SET utf8");
    m_Connected = true;

    return true;
}

void MySQLConnectionSync::Disconnect() {
    Lock();
    mysql_close(m_pMYSQL);
    Unlock();
}

bool MySQLConnectionSync::Ping() {
    if(!mysql_query(m_pMYSQL, "SELECT 1"))
        return true;
    else
        return false;
}

std::shared_ptr<MySQLResultSet> MySQLConnectionSync::Query(const std::string & sql)
{
    if(sql.empty())
        return std::make_shared<MySQLResultSet>(*(new MySQLResultSet()));
    MYSQL_RES* result;
    if(mysql_real_query(m_pMYSQL, sql.c_str(), sql.size())) {
        m_ErrorTrace(FmtStr("MySQLConnectionSync::ExecuteQuery: Error %s when execute query %s", mysql_error(m_pMYSQL), sql));
        return std::make_shared<MySQLResultSet>(*(new MySQLResultSet()));
    }
    if(!(result = mysql_store_result(m_pMYSQL))) {
        m_ErrorTrace(FmtStr("MySQLConnectionSync::ExecuteQuery: Error when store query result %s", mysql_error(m_pMYSQL)));
        return std::make_shared<MySQLResultSet>(*(new MySQLResultSet()));
    }
    std::shared_ptr<MySQLResultSet> resSet = nullptr;
    resSet = std::make_shared<MySQLResultSet>(*(new MySQLResultSet(result)));
    mysql_free_result(result);
    if(m_LogQueries)
        m_DebugTrace(FmtStr("SQL QUERY: %s", sql));
    return resSet;
}

unsigned long long MySQLConnectionSync::Execute(const std::string& sql) {
    if(sql.empty())
        return 0ULL;
    if(mysql_real_query(m_pMYSQL, sql.c_str(), sql.size())) {
        m_ErrorTrace(FmtStr("CMySQLConnectionSync::Execute: Error %s when execute query %s", mysql_error(m_pMYSQL), sql));
        return 0ULL;
    }
    if(m_LogQueries)
        m_DebugTrace(FmtStr("SQL QUERY: %s", sql));
    return (unsigned long long)mysql_affected_rows(m_pMYSQL);
}

void MySQLConnectionSync::Lock() {
	m_Mutex.lock();
}

bool MySQLConnectionSync::TryLock() {
	return m_Mutex.try_lock();
}

void MySQLConnectionSync::Unlock() {
	m_Mutex.unlock();
}

unsigned long MySQLConnectionSync::ThreadID() {
    return mysql_thread_id(m_pMYSQL);
}

void MySQLConnectionSync::EscapeString(char *dest, const char *source, size_t length) {
    mysql_real_escape_string(m_pMYSQL, dest, source, length);
}

MySQLConnectionAsync::MySQLConnectionAsync(const std::string& dbHost, const std::string& dbPort, const std::string& dbUser, const std::string& dbPassword, const std::string& dbName, bool logQueries,
	std::function<void(const std::string&)> debugTrace, std::function<void(const std::string&)> warningTrace, std::function<void(const std::string&)> errorTrace)
    : MySQLConnection(dbHost, dbPort, dbUser, dbPassword, dbName, logQueries, debugTrace, warningTrace, errorTrace) {}
