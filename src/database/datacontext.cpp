/*
 *  SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert
 * <juergen.muelbert@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QStandardPaths>
#include <QTextStream>
#include <QtSql/QSqlDatabase>
#include <memory>
#include <unordered_map>

Q_LOGGING_CATEGORY(dbLog, "jmbde.database")

#include "jmbdemodels/database/datacontext.h"

// Anonymous namespace for internal constants and utility functions
namespace {
    constexpr auto DEFAULT_CONNECTION = "default_connection";
    constexpr auto SQLITE_DRIVER = "QSQLITE";
    constexpr auto ODBC_DRIVER = "QODBC";
    constexpr auto PGSQL_DRIVER = "QPSQL";
    constexpr auto DB_EXTENSION = ".sqlite3";

    const std::unordered_map<QString, DBTypes> DB_TYPE_MAP = {
        {"SQLITE", DBTypes::SQLITE},
        {"PGSQL", DBTypes::PGSQL},
        {"ODBC", DBTypes::ODBC}
    };

    constexpr auto ERROR_DB_OPEN = "Failed to open database: %1";
    constexpr auto ERROR_DB_INIT = "Failed to initialize database: %1";
    constexpr auto ERROR_DB_VERSION = "Database version mismatch. Expected: %1.%2";
} // namespace

namespace jmbde {
namespace database {

// RAII-based transaction management for QSqlDatabase
class DatabaseTransaction {
public:
    explicit DatabaseTransaction(QSqlDatabase& db) : m_db(db), m_active(db.transaction()) {}

    ~DatabaseTransaction() {
        if (m_active) {
            m_db.rollback();
            qCWarning(dbLog) << "Transaction rolled back.";
        }
    }

    bool commit() {
        if (m_active) {
            m_active = false;
            return m_db.commit();
        }
        return false;
    }

private:
    QSqlDatabase& m_db;
    bool m_active{false};
};

DataContext::DataContext(QObject* parent, const QString& dbName)
    : QObject(parent)
    , m_dbName(dbName.isEmpty() ? QCoreApplication::applicationName() : dbName)
    , m_dbType(DBTypes::SQLITE) {
    initializeDatabase();
}

void DataContext::initializeDatabase() {
    try {
        m_database = QSqlDatabase::addDatabase(SQLITE_DRIVER, DEFAULT_CONNECTION);
        m_database.setDatabaseName(m_dbName);

        if (!m_database.open()) {
            throw std::runtime_error(m_database.lastError().text().toStdString());
        }

        setupDatabase();
        qCInfo(dbLog) << "Database initialized successfully.";
    } catch (const std::exception& e) {
        const QString errorMsg = tr(ERROR_DB_INIT).arg(e.what());
        qCCritical(dbLog) << errorMsg;
        emit databaseError(errorMsg);
    }
}

void DataContext::setupDatabase() {
    switch (m_dbType) {
        case DBTypes::SQLITE:
            setupSQLiteDatabase();
            break;
        case DBTypes::ODBC:
            setupODBCDatabase();
            break;
        case DBTypes::PGSQL:
            setupPostgresDatabase();
            break;
        default:
            throw std::runtime_error("Unsupported database type.");
    }
}

void DataContext::setupSQLiteDatabase() {
    setDatabaseConnection();
    QFile dbFile(m_connectionString);

    if (!dbFile.exists()) {
        qCInfo(dbLog) << "Creating new SQLite database at:" << m_connectionString;
        createNewDatabase();
    } else {
        qCInfo(dbLog) << "Opening existing SQLite database at:" << m_connectionString;
        openDatabase();
    }
}

void DataContext::createNewDatabase() {
    DatabaseTransaction transaction(m_database);

    try {
        executeSQLScript(":/data/script.sql");
        if (transaction.commit()) {
            qCInfo(dbLog) << "New SQLite database created successfully.";
        } else {
            throw std::runtime_error("Failed to commit database creation.");
        }
    } catch (const std::exception& e) {
        const QString errorMsg = tr("Database creation failed: %1").arg(e.what());
        qCCritical(dbLog) << errorMsg;
        emit databaseError(errorMsg);
    }
}

void DataContext::executeSQLScript(const QString& scriptPath) {
    QFile scriptFile(scriptPath);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to open SQL script file: " + scriptPath.toStdString());
    }

    QTextStream scriptStream(&scriptFile);
    QSqlQuery query(m_database);

    while (!scriptStream.atEnd()) {
        QString line = scriptStream.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("--")) {
            continue; // Skip comments and empty lines
        }

        if (!query.exec(line)) {
            throw std::runtime_error(
                QString("SQL Error: %1. Query: %2")
                    .arg(query.lastError().text(), line)
                    .toStdString()
            );
        }
    }
    qCInfo(dbLog) << "SQL script executed successfully.";
}

void DataContext::setDatabaseConnection() {
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir writeDir(dataPath);

    if (!writeDir.exists() && !writeDir.mkpath(dataPath)) {
        const QString errorMsg = tr("Failed to create database directory: %1").arg(dataPath);
        qCCritical(dbLog) << errorMsg;
        throw std::runtime_error(errorMsg.toStdString());
    }

    m_connectionString = dataPath + QDir::separator() + m_dbName + DB_EXTENSION;
}

} // namespace database
} // namespace jmbde
