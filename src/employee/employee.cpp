/*
 * SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert <juergen.muelbert@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "jmbdemodels/employee.h"

#include <QDebug>
#include <QString>
#include <QStringBuilder>
#include <format>
#include <stdexcept>

namespace {
    // Database field name constants
    constexpr struct FieldNames {
        QLatin1String employeeId{"employee_id"};
        QLatin1String employeeNr{"employee_nr"};
        QLatin1String gender{"gender"};
        QLatin1String titleId{"title_id"};
        QLatin1String firstName{"first_name"};
        QLatin1String lastName{"last_name"};
        QLatin1String birthDay{"birth_day"};
        QLatin1String address{"address"};
        QLatin1String zipCityId{"zip_city_id"};
        QLatin1String homePhone{"home_phone"};
        QLatin1String homeMobile{"home_mobile"};
        QLatin1String homeMail{"home_mail_address"};
        QLatin1String businessMail{"business_mail_address"};
        QLatin1String dataCare{"data_care"};
        QLatin1String active{"active"};
        QLatin1String photo{"photo"};
        QLatin1String notes{"notes"};
        QLatin1String hireDate{"hire_date"};
        QLatin1String endDate{"end_date"};
        QLatin1String departmentId{"department_id"};
        QLatin1String functionId{"function_id"};
        QLatin1String computerId{"computer_id"};
        QLatin1String printerId{"printer_id"};
        QLatin1String phoneId{"phone_id"};
        QLatin1String mobileId{"mobile_id"};
        QLatin1String faxId{"fax_id"};
        QLatin1String employeeAccountId{"employee_account_id"};
        QLatin1String employeeDocumentId{"employee_document_id"};
        QLatin1String chipCardId{"chip_card_id"};
        QLatin1String lastUpdate{"last_update"};
    } FIELD_NAMES;
}

namespace Model {

Employee::Employee()
    : CommonData()
    , m_dataContext(std::make_unique<DataContext>())
    , m_db(m_dataContext->getDatabase())
    , m_model(std::make_unique<QSqlRelationalTableModel>(this, m_db)) {
    
    try {
        m_model->setTable(TABLE_NAME);
        m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        setIndexes();
    } catch (const std::exception& e) {
        qCritical() << "Failed to initialize Employee model:" << e.what();
        throw;
    }
}

void Employee::setIndexes() {
    if (!m_model) {
        throw std::runtime_error("Model not initialized");
    }

    // Map all field indexes using the struct from the header
    m_indexes = {
        .employeeId = m_model->fieldIndex(FIELD_NAMES.employeeId),
        .employeeNr = m_model->fieldIndex(FIELD_NAMES.employeeNr),
        .gender = m_model->fieldIndex(FIELD_NAMES.gender),
        // ... continue for all fields
        .lastUpdate = m_model->fieldIndex(FIELD_NAMES.lastUpdate)
    };

    // Validate indexes
    if (m_indexes.employeeId == -1) {
        throw std::runtime_error("Failed to initialize field indexes");
    }
}

std::unique_ptr<QSqlRelationalTableModel> Employee::initializeRelationalModel() {
    auto model = std::make_unique<QSqlRelationalTableModel>(this, m_db);
    model->setTable(TABLE_NAME);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    if (!model->select()) {
        qWarning() << "Failed to select data for relational model:" << model->lastError().text();
    }
    
    return model;
}

std::unique_ptr<QSqlRelationalTableModel> Employee::initializeInputDataModel() {
    auto model = std::make_unique<QSqlRelationalTableModel>(this, m_db);
    model->setTable(TABLE_NAME);
    return model;
}

std::unique_ptr<QSqlTableModel> Employee::initializeViewModel() {
    if (!m_model->select()) {
        qWarning() << "Failed to select data for view model:" << m_model->lastError().text();
    }
    return std::make_unique<QSqlTableModel>(*m_model);
}

std::unique_ptr<QSqlTableModel> Employee::initializeListModel() {
    auto listModel = std::make_unique<QSqlTableModel>(this, m_db);
    listModel->setTable(TABLE_NAME);
    listModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    if (!listModel->select()) {
        qWarning() << "Failed to select data for list model:" << listModel->lastError().text();
    }
    
    return listModel;
}

QString Employee::generateTableString(const QString& header) const {
    const auto columnCount = m_model->columnCount();
    const auto rowCount = m_model->rowCount();

    qDebug() << "Generating table with header:" << header
             << "Columns:" << columnCount
             << "Rows:" << rowCount;

    QString result;
    result.reserve(1000); // Preallocate memory for better performance

    result = QString("<h1>%1</h1><hr />"
                    "<table width='100%' cellspacing='0' class='tbl'>"
                    "<thead><tr>").arg(header);

    for (int i = 0; i < columnCount; ++i) {
        result += QString("<th>%1</th>")
                     .arg(m_model->headerData(i, Qt::Horizontal).toString());
    }

    result += "</tr></thead></table>";
    return result;
}

QString Employee::generateFormularString(const QString& header) const {
    qDebug() << "Generating form with header:" << header
             << "Model status - Columns:" << m_model->columnCount()
             << "Rows:" << m_model->rowCount();

    return QString("<h1>%1</h1><hr />").arg(header);
}

} // namespace Model


