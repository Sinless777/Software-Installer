#include "Database.h"
#include "Database.h"
#include <iostream>

// Constructor: open or create SQLite DB, then connect to MySQL and create schema
Database::Database(const std::string& sqlitePath,
    const std::string& mysqlHost,
    const std::string& mysqlUser,
    const std::string& mysqlPassword,
    const std::string& mysqlDB,
    unsigned int mysqlPort)
    : sqlitePath_(sqlitePath),
    mysqlHost_(mysqlHost),
    mysqlUser_(mysqlUser),
    mysqlPassword_(mysqlPassword),
    mysqlDB_(mysqlDB),
    mysqlPort_(mysqlPort)
{
    if (!openSQLite()) {
        std::cerr << "[Database] Failed to open or create SQLite DB at " << sqlitePath_ << std::endl;
    }
    if (!connectMySQL()) {
        std::cerr << "[Database] Failed to connect or create MySQL schema " << mysqlDB_ << std::endl;
    }
}

// Destructor: close both DB connections
Database::~Database() {
    closeConnections();
}

// Create or open SQLite database file
bool Database::createSQLiteDb() {
    int rc = sqlite3_open(sqlitePath_.c_str(), &sqliteDb_);
    return rc == SQLITE_OK;
}

bool Database::openSQLite() {
    return createSQLiteDb();
}

// Create MySQL database/schema if it doesn't exist
bool Database::createMySQLDatabase() {
    MYSQL* tempConn = mysql_init(nullptr);
    if (!tempConn) return false;
    if (!mysql_real_connect(tempConn,
        mysqlHost_.c_str(),
        mysqlUser_.c_str(),
        mysqlPassword_.c_str(),
        nullptr,
        mysqlPort_,
        nullptr,
        0)) {
        mysql_close(tempConn);
        return false;
    }
    std::string query = "CREATE DATABASE IF NOT EXISTS `" + mysqlDB_ + "`;";
    bool ok = (mysql_query(tempConn, query.c_str()) == 0);
    mysql_close(tempConn);
    return ok;
}

// Connect to MySQL and select the database
bool Database::connectMySQL() {
    mysqlConn_ = mysql_init(nullptr);
    if (!mysqlConn_) return false;
    if (!mysql_real_connect(mysqlConn_,
        mysqlHost_.c_str(),
        mysqlUser_.c_str(),
        mysqlPassword_.c_str(),
        nullptr,
        mysqlPort_,
        nullptr,
        0)) {
        mysql_close(mysqlConn_);
        mysqlConn_ = nullptr;
        return false;
    }
    if (!createMySQLDatabase()) return false;
    if (mysql_select_db(mysqlConn_, mysqlDB_.c_str()) != 0) {
        return false;
    }
    return true;
}

// Close SQLite and MySQL connections
void Database::closeConnections() {
    if (sqliteDb_) {
        sqlite3_close(sqliteDb_);
        sqliteDb_ = nullptr;
    }
    if (mysqlConn_) {
        mysql_close(mysqlConn_);
        mysqlConn_ = nullptr;
    }
}

// Load all Software entries from SQLite
std::vector<Software> Database::loadSoftwareFromSQLite() {
    std::vector<Software> list;
    const char* sql = "SELECT name, url, silentFlags, license FROM Software;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(sqliteDb_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return list;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Software s;
        s.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        s.url = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        s.silentFlags = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const unsigned char* lic = sqlite3_column_text(stmt, 3);
        s.license = lic ? reinterpret_cast<const char*>(lic) : std::string();
        list.push_back(std::move(s));
    }
    sqlite3_finalize(stmt);
    return list;
}

// Load all URL entries from SQLite
std::vector<UrlEntry> Database::loadUrlsFromSQLite() {
    // TODO: Implement based on UrlEntry struct
    return {};
}

// Migrate all Software from SQLite to MySQL
bool Database::migrateSoftwareToMySQL() {
    auto items = loadSoftwareFromSQLite();
    for (const auto& s : items) {
        if (!insertSoftware(s)) return false;
    }
    return true;
}

// Migrate all URLs from SQLite to MySQL
bool Database::migrateUrlsToMySQL() {
    // TODO: Use loadUrlsFromSQLite() and insertUrl()
    return true;
}

// Insert a single Software record into MySQL
bool Database::insertSoftware(const Software& s) {
    // Use prepared statements for safety
    MYSQL_STMT* stmt = mysql_stmt_init(mysqlConn_);
    const char* sql = "REPLACE INTO Software (name, url, silentFlags, license) VALUES (?, ?, ?, ?);";
    if (!stmt || mysql_stmt_prepare(stmt, sql, static_cast<unsigned long>(strlen(sql))) != 0) {
        return false;
    }
    MYSQL_BIND bind[4] = {};
    unsigned long nameLen = static_cast<unsigned long>(s.name.size());
    unsigned long urlLen = static_cast<unsigned long>(s.url.size());
    unsigned long flagsLen = static_cast<unsigned long>(s.silentFlags.size());
    unsigned long licLen = static_cast<unsigned long>(s.license.size());
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char*)s.name.c_str();
    bind[0].buffer_length = nameLen;
    bind[0].length = &nameLen;
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char*)s.url.c_str();
    bind[1].buffer_length = urlLen;
    bind[1].length = &urlLen;
    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (char*)s.silentFlags.c_str();
    bind[2].buffer_length = flagsLen;
    bind[2].length = &flagsLen;
    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = (char*)s.license.c_str();
    bind[3].buffer_length = licLen;
    bind[3].length = &licLen;
    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        mysql_stmt_close(stmt);
        return false;
    }
    bool ok = (mysql_stmt_execute(stmt) == 0);
    mysql_stmt_close(stmt);
    return ok;
}

// Insert a single URL record into MySQL
bool Database::insertUrl(const UrlEntry& urlEntry) {
    // TODO: Implement based on UrlEntry fields and table schema
    return false;
}
