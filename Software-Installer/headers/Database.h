#pragma once

#include <string>
#include <vector>
#include <mysql/mysql.h>
#include <sqlite3.h>
#include "Structs.h"

/**
 * @class Database
 *
 * Handles synchronization between a local SQLite resource database
 * and a remote MySQL database for software entries and associated URLs.
 * If the specified databases (SQLite file or MySQL schema) do not exist,
 * they will be created automatically.
 */
class Database {
    public:
        /**
         * @brief Construct a Database object
         *
         * @param sqlitePath     Path to the local SQLite database file.
         * @param mysqlHost      Hostname or IP of the MySQL server.
         * @param mysqlUser      Username for MySQL authentication.
         * @param mysqlPassword  Password for MySQL authentication.
         * @param mysqlDB        Name of the MySQL database/schema.
         * @param mysqlPort      Port number for MySQL (default 3306).
         *
         * Upon construction, attempts to open or create the SQLite database,
         * then connect to MySQL and create the target schema if it does not exist.
         */
        Database(const std::string& sqlitePath,
            const std::string& mysqlHost,
            const std::string& mysqlUser,
            const std::string& mysqlPassword,
            const std::string& mysqlDB,
            unsigned int mysqlPort = 3306);

        /**
         * @brief Destructor closes any open database connections.
         */
        ~Database();

        /**
         * @brief Establishes connection to the MySQL server and creates the schema if missing.
         * @return true on success, false on failure.
         */
        bool connectMySQL();

        /**
         * @brief Opens the SQLite database file, creating it if it does not exist.
         * @return true on success, false on failure.
         */
        bool openSQLite();

        /**
         * @brief Closes both SQLite and MySQL connections.
         */
        void closeConnections();

        /**
         * @brief Loads all software entries from the SQLite database.
         * @return Vector of Software structs read from SQLite.
         */
        std::vector<Software> loadSoftwareFromSQLite();

        /**
         * @brief Loads all URL entries from the SQLite database.
         * @return Vector of UrlEntry structs read from SQLite.
         */
        std::vector<UrlEntry> loadUrlsFromSQLite();

        /**
         * @brief Migrates software entries from SQLite into the MySQL database.
         * @return true on full success, false if any insertion fails.
         */
        bool migrateSoftwareToMySQL();

        /**
         * @brief Migrates URL entries from SQLite into the MySQL database.
         * @return true on full success, false if any insertion fails.
         */
        bool migrateUrlsToMySQL();

        /**
         * @brief Inserts a single Software record into MySQL.
         * @param software  Software struct to insert.
         * @return true on success.
         */
        bool insertSoftware(const Software& software);

        /**
         * @brief Inserts a single URL record into MySQL.
         * @param urlEntry  UrlEntry struct to insert.
         * @return true on success.
         */
        bool insertUrl(const UrlEntry& urlEntry);

    private:
        // Path to local SQLite resource database (file created if missing)
        std::string sqlitePath_;
        sqlite3* sqliteDb_ = nullptr;

        // MySQL connection parameters (schema created if missing)
        std::string mysqlHost_;
        std::string mysqlUser_;
        std::string mysqlPassword_;
        std::string mysqlDB_;
        unsigned int mysqlPort_;
        MYSQL* mysqlConn_ = nullptr;

        /**
         * @brief Creates the SQLite database file if it doesn't exist.
         * @return true on success.
         */
        bool createSQLiteDb();

        /**
         * @brief Creates the MySQL database/schema if it doesn't exist.
         * @return true on success.
         */
        bool createMySQLDatabase();
    };
