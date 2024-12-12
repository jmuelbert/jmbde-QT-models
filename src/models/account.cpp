/*
 * SPDX-FileCopyrightText: 2013-2021 Jürgen Mülbert
 * <juergen.muelbert@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QLoggingCategory>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QVariant>
#include <utility> // For std::move and std::exchange

#include "jmbde/models/account.h"
#include "jmbde/models/core/commondata.h"
#include "jmbde/models/database/datacontext.h"



Q_LOGGING_CATEGORY(accountLog, "jmbde.models.account")

namespace jmbde {
namespace model {

// Static member initialization
const QString Account::TABLE_NAME = QStringLiteral("account");
const QString Account::COLUMN_ID = QStringLiteral("id");
const QString Account::COLUMN_USERNAME = QStringLiteral("username");
const QString Account::COLUMN_PASSWORD = QStringLiteral("password");
const QString Account::COLUMN_IS_ACTIVE = QStringLiteral("is_active");
const QString Account::COLUMN_LAST_LOGIN = QStringLiteral("last_login");
const QString Account::COLUMN_CREATED_AT = QStringLiteral("created_at");
const QString Account::COLUMN_UPDATED_AT = QStringLiteral("updated_at");

// Constructors
Account::Account(DataContext* context, QObject* parent)
    : CommonData(context, parent), m_isActive(true), m_id(0) {
    initializeDatabase();
}

Account::Account(QObject* parent)
    : QObject(parent), m_isActive(true), m_id(0) {
    initializeDatabase();
}

Account::Account(const Account& other)
    : QObject(other.parent()), 
      m_id(other.m_id),
      m_username(other.m_username),
      m_password(other.m_password),
      m_isActive(other.m_isActive),
      m_lastLogin(other.m_lastLogin),
      m_createdAt(other.m_createdAt),
      m_updatedAt(other.m_updatedAt) {}

Account::Account(Account&& other) noexcept
    : QObject(other.parent()),
      m_id(std::exchange(other.m_id, 0)),
      m_username(std::move(other.m_username)),
      m_password(std::move(other.m_password)),
      m_isActive(std::exchange(other.m_isActive, true)),
      m_lastLogin(std::move(other.m_lastLogin)),
      m_createdAt(std::move(other.m_createdAt)),
      m_updatedAt(std::move(other.m_updatedAt)) {}

Account::~Account() = default;

// Assignment Operators
Account& Account::operator=(const Account& other) {
    if (this != &other) {
        m_id = other.m_id;
        m_username = other.m_username;
        m_password = other.m_password;
        m_isActive = other.m_isActive;
        m_lastLogin = other.m_lastLogin;
        m_createdAt = other.m_createdAt;
        m_updatedAt = other.m_updatedAt;
        emit dataChanged();
    }
    return *this;
}

Account& Account::operator=(Account&& other) noexcept {
    if (this != &other) {
        m_id = std::exchange(other.m_id, 0);
        m_username = std::move(other.m_username);
        m_password = std::move(other.m_password);
        m_isActive = std::exchange(other.m_isActive, true);
        m_lastLogin = std::move(other.m_lastLogin);
        m_createdAt = std::move(other.m_createdAt);
        m_updatedAt = std::move(other.m_updatedAt);
        emit dataChanged();
    }
    return *this;
}

// Data Manipulation
void Account::setUsername(const QString& username) {
    if (username.isEmpty()) {
        emit saveError(tr("Username cannot be empty"));
        return;
    }
    if (m_username != username) {
        m_username = username;
        emit dataChanged();
    }
}

void Account::setPassword(const QString& password) {
    if (password.isEmpty()) {
        emit saveError(tr("Password cannot be empty"));
        return;
    }
    QString hashed = hashPassword(password);
    if (m_password != hashed) {
        m_password = std::move(hashed);
        emit dataChanged();
    }
}

// Database Operations
bool Account::load(int id) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        qCCritical(accountLog) << "Invalid database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT * FROM %1 WHERE %2 = :id")
                      .arg(TABLE_NAME, COLUMN_ID));
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        qCCritical(accountLog) << "Failed to load account:" << query.lastError().text();
        emit saveError(tr("Failed to load account data"));
        return false;
    }

    m_id = query.value(COLUMN_ID).toInt();
    m_username = query.value(COLUMN_USERNAME).toString();
    m_password = query.value(COLUMN_PASSWORD).toString();
    m_isActive = query.value(COLUMN_IS_ACTIVE).toBool();
    m_lastLogin = query.value(COLUMN_LAST_LOGIN).toDateTime();
    m_createdAt = query.value(COLUMN_CREATED_AT).toDateTime();
    m_updatedAt = query.value(COLUMN_UPDATED_AT).toDateTime();

    emit dataChanged();
    return true;
}

bool Account::save() {
    if (!isValid()) {
        qCWarning(accountLog) << "Cannot save invalid account data";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        qCCritical(accountLog) << "Invalid database connection";
        return false;
    }

    db.transaction();
    QSqlQuery query(db);

    try {
        if (m_id == 0) {
            // Insert
            query.prepare(QStringLiteral(
                "INSERT INTO %1 (%2, %3, %4, %5, %6, %7) "
                "VALUES (:username, :password, :isActive, :lastLogin, :createdAt, :updatedAt)")
                .arg(TABLE_NAME, COLUMN_USERNAME, COLUMN_PASSWORD, COLUMN_IS_ACTIVE,
                     COLUMN_LAST_LOGIN, COLUMN_CREATED_AT, COLUMN_UPDATED_AT));
        } else {
            // Update
            query.prepare(QStringLiteral(
                "UPDATE %1 SET %2 = :username, %3 = :password, %4 = :isActive, "
                "%5 = :lastLogin, %6 = :updatedAt WHERE %7 = :id")
                .arg(TABLE_NAME, COLUMN_USERNAME, COLUMN_PASSWORD, COLUMN_IS_ACTIVE,
                     COLUMN_LAST_LOGIN, COLUMN_UPDATED_AT, COLUMN_ID));
            query.bindValue(":id", m_id);
        }

        query.bindValue(":username", m_username);
        query.bindValue(":password", m_password);
        query.bindValue(":isActive", m_isActive);
        query.bindValue(":lastLogin", m_lastLogin);
        query.bindValue(":createdAt", m_createdAt.isValid() ? m_createdAt : QDateTime::currentDateTime());
        query.bindValue(":updatedAt", QDateTime::currentDateTime());

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        if (m_id == 0) {
            m_id = query.lastInsertId().toInt();
        }

        db.commit();
        emit dataChanged();
        return true;

    } catch (const std::exception& ex) {
        db.rollback();
        qCCritical(accountLog) << "Failed to save account:" << ex.what();
        emit saveError(tr("Failed to save account: %1").arg(ex.what()));
        return false;
    }
}

// Utility
bool Account::isValid() const {
    return !m_username.isEmpty() && !m_password.isEmpty();
}

void Account::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        qCCritical(accountLog) << "Invalid database connection";
        return;
    }

    if (!db.tables().contains(TABLE_NAME)) {
        QSqlQuery query(db);
        query.exec(QStringLiteral(
            "CREATE TABLE IF NOT EXISTS %1 ("
            "%2 INTEGER PRIMARY KEY AUTOINCREMENT, "
            "%3 TEXT NOT NULL UNIQUE, "
            "%4 TEXT NOT NULL, "
            "%5 BOOLEAN NOT NULL DEFAULT 1, "
            "%6 DATETIME, "
            "%7 DATETIME NOT NULL, "
            "%8 DATETIME NOT NULL)")
            .arg(TABLE_NAME, COLUMN_ID, COLUMN_USERNAME, COLUMN_PASSWORD,
                 COLUMN_IS_ACTIVE, COLUMN_LAST_LOGIN, COLUMN_CREATED_AT, COLUMN_UPDATED_AT));
    }
}

QString Account::hashPassword(const QString& password) {
    return QString::fromLatin1(
        QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

} // namespace model
} // namespace jmbde
