/*
 * SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert
 * <juergen.muelbert@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariantMap>
#include <memory>
#include <functional>

#include "jmbdemodels_export.h"

Q_DECLARE_LOGGING_CATEGORY(dbLog)

namespace jmbde {
namespace database {

/**
 * @brief Enum for supported database types.
 */
enum class DBType {
    SQLite,
    PostgreSQL,
    ODBC,
    Unknown
};

/**
 * @brief Struct for configuring database connections.
 */
struct DBConfig {
    QString name;
    DBType type{DBType::SQLite};
    QString host;
    QString username;
    QString password;
    int port{0};

    /**
     * @brief Validates the database configuration.
     * @return True if the configuration is valid, otherwise false.
     */
    [[nodiscard]] bool isValid() const noexcept;
};

/**
 * @brief Manages database connections and operations.
 *
 * Provides a unified interface for interacting with various database types,
 * supporting connection handling, transactions, and CRUD operations.
 *
 * @author Jürgen
 * @version 0.8
 * @date 25.11.2024
 * @copyright GPL-3.0
 */
class JMBDEMODELS_EXPORT DataContext : public QObject {
    Q_OBJECT

public:
    using Ptr = std::shared_ptr<DataContext>;
    using QueryCallback = std::function<bool(QSqlQuery&)>;

    // ---- Constructors ----
    /**
     * @brief Constructor with minimal configuration.
     * @param parent Parent QObject.
     * @param dbName Database name.
     */
    explicit DataContext(QObject* parent = nullptr, const QString& dbName = {});

    /**
     * @brief Constructor with full configuration.
     * @param config Database configuration.
     * @param parent Parent QObject.
     */
    explicit DataContext(const DBConfig& config, QObject* parent = nullptr);

    /**
     * @brief Constructor with individual database parameters.
     * @param parent Parent QObject.
     * @param dbName Database name.
     * @param dbType Database type.
     * @param userName Username.
     * @param passWord Password.
     * @param hostName Hostname.
     * @param port Port number.
     */
    explicit DataContext(QObject* parent, const QString& dbName, const QString& dbType,
                         const QString& userName, const QString& passWord,
                         const QString& hostName, int port);

    /**
     * @brief Destructor.
     */
    ~DataContext() override;

    // ---- Deleted and Defaulted Functions ----
    DataContext(const DataContext&) = delete;
    DataContext& operator=(const DataContext&) = delete;
    DataContext(DataContext&&) noexcept = default;
    DataContext& operator=(DataContext&&) noexcept = default;

    // ---- Public Methods ----
    [[nodiscard]] bool isConnected() const noexcept;
    bool executeTransaction(const QueryCallback& operation);

    bool insert(const QString& tableName, const QVariantMap& data);
    bool update(const QString& tableName, const QVariantMap& data, const QString& condition);
    bool remove(const QString& tableName, const QString& condition);
    [[nodiscard]] QSqlQuery executeQuery(const QString& query);
    [[nodiscard]] bool recordExists(const QString& tableName, const QString& column, const QVariant& value);
    [[nodiscard]] QString databaseVersion() const;

    QSqlDatabase getDatabase() const;
    static QSqlQuery getQuery(const QString& query);
    bool checkExistence(const QString& tableName, const QString& searchId, const QString& search);

    void open();
    void open(const QString& name);
    void renameDB(const QString& newName = {});
    void deleteDB(const QString& name = {});
    QString getConnectionString() const;

signals:
    void databaseError(const QString& error);
    void operationCompleted(const QString& message);

protected:
    void initializeDatabase();
    void setupSchema();
    void executeSQLScript(const QString& scriptPath);
    void init();
    void setupSQLiteDatabase();
    void setupODBCDatabase();
    void setupPostgresDatabase();
    void prepareDB() const;
    bool checkDBVersion(const QString& actualVersion, const QString& actualRevision) const;

private:
    class Implementation;
    std::unique_ptr<Implementation> m_impl;

    void setupDatabase();
    void createNewDatabase();
    void openExistingDatabase();
    void setDatabaseConnection();
    bool validateConnection() const;
    bool migrateDatabase();
    void setupLogging();

    QString m_dbName;
    QString m_connectionString;
    QSqlDatabase m_database;
    DBType m_dbType{DBType::Unknown};
    QString m_dbHostName;
    QString m_dbUserName;
    QString m_dbPassWord;
    int m_dbPort{0};
};

} // namespace database
} // namespace jmbde

Q_DECLARE_METATYPE(jmbde::database::DBType)
