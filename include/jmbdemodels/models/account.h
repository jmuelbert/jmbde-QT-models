/*
 * SPDX-FileCopyrightText: 2013-2021 Jürgen Mülbert
 * <juergen.muelbert@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Account class declaration.
 *
 * - Represents user account data.
 * - Supports CRUD operations, password hashing, and database interaction.
 * - Uses Qt signals/slots for updates and error handling.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QLoggingCategory>
#include <QSqlDatabase>

#include "jmbdemodels/database/datacontext.h"

namespace jmbde {
namespace models {

// Logging category for account operations
Q_DECLARE_LOGGING_CATEGORY(accountLog)

/**
 * @class Account
 * @brief Represents a user account in the system.
 *
 * Handles database operations, password management, and validation.
 */
class Account : public QObject {
    Q_OBJECT

public:
    // Database table and column names
    static const QString TABLE_NAME;
    static const QString COLUMN_ID;
    static const QString COLUMN_USERNAME;
    static const QString COLUMN_PASSWORD;
    static const QString COLUMN_IS_ACTIVE;
    static const QString COLUMN_LAST_LOGIN;
    static const QString COLUMN_CREATED_AT;
    static const QString COLUMN_UPDATED_AT;

    // Constructors and Destructor
    explicit Account(DataContext* context, QObject* parent = nullptr);
    explicit Account(QObject* parent = nullptr);
    Account(const Account& other);
    Account(Account&& other) noexcept;
    ~Account() override;

    // Operators
    Account& operator=(const Account& other);
    Account& operator=(Account&& other) noexcept;

    // Public API
    void setUsername(const QString& username);
    void setPassword(const QString& password);
    bool load(int id);     // Load account by ID
    bool save();           // Save account to the database
    bool isValid() const;  // Check account validity

signals:
    void dataChanged();       // Emitted on data update
    void saveError(const QString& message); // Emitted on save failure

private:
    // Account data members
    int m_id = 0;               // Account ID
    QString m_username;         // Username
    QString m_password;         // Hashed password
    bool m_isActive = true;     // Active status
    QDateTime m_lastLogin;      // Last login timestamp
    QDateTime m_createdAt;      // Creation timestamp
    QDateTime m_updatedAt;      // Last update timestamp

    // Internal utility functions
    void initializeDatabase();                     // Ensure database schema
    QString hashPassword(const QString& password); // Hash password securely
};

} // namespace model
} // namespace jmbde
