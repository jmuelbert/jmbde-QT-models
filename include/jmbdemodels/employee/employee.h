/**
 * @file employee.cpp
 * @brief Implementation of the Employee class for database employee management
 * @author Jürgen Mülbert
 *
 * @copyright Copyright (c) 2013-2024 Jürgen Mülbert <juergen.muelbert@gmail.com>
 * @license GPL-3.0-or-later
 *
 * @details
 * Implements functionality for:
 * - Employee database record management
 * - SQL model initialization and configuration
 * - Data presentation and formatting
 * - Field mapping and validation
 *
 * Features:
 * - RAII-compliant resource management
 * - Exception-safe operations
 * - Thread-safe database access
 * - Memory-optimized data handling
 * - Performance-focused HTML generation
 *
 * Technical Details:
 * - Uses Qt SQL framework for database operations
 * - Implements relational and table models
 * - Provides HTML output generation
 * - Handles field indexing and mapping
 *
 * Dependencies:
 * - Qt Core (QString, QDebug)
 * - Qt SQL (QSqlDatabase, QSqlTableModel)
 * - C++ Standard Library
 *
 * @note All database operations should be performed within transactions
 * @warning Ensure proper database connection before using this class
 *
 * @see Employee.h
 * @see CommonData
 * @see DataContext
 */

 #pragma once

 #include <QObject>
 #include <QString>
 #include <QSqlDatabase>
 #include <QSqlRelationalTableModel>
 #include <QSqlTableModel>
 #include <memory>

 #include "jmbdemodels/core/commondata.h"
 #include "jmbdemodels/database/datacontext.h"

namespace Model {

/**
 * @brief The Employee class handles employee data and database operations.
 *
 * This class provides:
 * - CRUD operations for employee records.
 * - Table model initialization for input, view, and list models.
 * - Utility methods for generating HTML representations of employee data.
 */
 class Employee final : public CommonData {
     Q_OBJECT

public:
    /**
     * @brief Constructs an Employee object and initializes the database model.
     */
    Employee();

    /// @brief Destructor
    ~Employee() override = default;

    /**
     * @brief Initialize different model types for employee data
     * @return Smart pointer to the initialized model
     */
    [[nodiscard]] std::unique_ptr<QSqlRelationalTableModel> initializeRelationalModel();
    [[nodiscard]] std::unique_ptr<QSqlRelationalTableModel> initializeInputDataModel();
    [[nodiscard]] std::unique_ptr<QSqlTableModel> initializeViewModel();
    [[nodiscard]] std::unique_ptr<QSqlTableModel> initializeListModel();

    /**
     * @brief Generate HTML representations of employee data
     * @param header The title for the HTML output
     * @return Generated HTML string
     */
    [[nodiscard]] QString generateTableString(const QString& header) const;
    [[nodiscard]] QString generateFormularString(const QString& header) const;

private:
    /**
     * @brief Sets field indexes for the table model.
     */
    void setIndexes();

    // Constants
    static constexpr const char* TABLE_NAME = "employees";

    // Core members
    std::unique_ptr<DataContext> m_dataContext;         ///< Context for database interactions.
    QSqlDatabase m_db;                                  ///< Database connection.
    std::unique_ptr<QSqlRelationalTableModel> m_model;  ///< Model for interacting with employee data.

    // Field indexes
    struct FieldIndexes {
        int employeeId = -1;
        int employeeNr = -1;
        int gender = -1;
        int titleId = -1;
        int firstName = -1;
        int lastName = -1;
        int birthDay = -1;
        int address = -1;
        int zipCityId = -1;
        int homePhone = -1;
        int homeMobile = -1;
        int homeMail = -1;
        int businessMail = -1;
        int dataCare = -1;
        int active = -1;
        int photo = -1;
        int notes = -1;
        int hireDate = -1;
        int endDate = -1;
        int departmentId = -1;
        int functionId = -1;
        int computerId = -1;
        int printerId = -1;
        int phoneId = -1;
        int mobileId = -1;
        int faxId = -1;
        int employeeAccountId = -1;
        int employeeDocumentId = -1;
        int chipCardId = -1;
        int lastUpdate = -1;
    } m_indexes;
};

} // namespace Model
