/*
 *  SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert
 * <juergen.muelbert@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QDebug>
#include <QList>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QString>

#include "commondata.h"
#include "datacontext.h"
#include "jmbdemodels_export.h"

namespace Model {
/*!
 * \class Account
 * \brief Handles employee account data, providing database interaction and
 *        data model management.
 * \author Jürgen Mülbert
 * \since 0.4
 * \version 0.8
 * \date 25.11.2024
 * \copyright GPL-3.0-or-later
 */

class Company : public CommonData {
  Q_OBJECT

public:
/*!
 * \brief Constructor: Initializes the company model and database connection.
 */
  explicit JMBDEMODELS_EXPORT Company();

  /*!
   * \brief Sets the field indexes for database table fields.
   */
  JMBDEMODELS_EXPORT ~Company() override;

  /*!
   * \brief Sets the field indexes for database table fields.
   */
  virtual JMBDEMODELS_EXPORT void setIndexes() final;

  /*!
   * \brief Initializes a QSqlRelationalTableModel for account data.
   * \return A pointer to the initialized QSqlRelationalTableModel.
   */
  virtual JMBDEMODELS_EXPORT auto initializeRelationalModel()
      -> QSqlRelationalTableModel * final;

  /*!
   * \brief Initializes the input data model.
   * \return A pointer to the QSqlRelationalTableModel used for input.
   */
  virtual JMBDEMODELS_EXPORT auto initializeInputDataModel()
      -> QSqlRelationalTableModel * final;

  /*!
   * \fn virtual auto initializeViewModel() -> QSqlTableModel* final
   * \brief Initialize the ViewModel
   *
   * \return QSqlTableModel
   */
  virtual JMBDEMODELS_EXPORT auto initializeViewModel()
      -> QSqlTableModel * final;

  /*!
   * \brief Initializes the list model for selecting datasets.
   * \return A pointer to the QSqlTableModel used for listing.
   */
  virtual JMBDEMODELS_EXPORT auto initializeListModel() -> QSqlTableModel * final;

  /*!
   * \brief Generates an HTML table string for display or export.
   * \param header The header text for the table.
   * \return A QString containing the HTML representation of the table.
   */
  virtual JMBDEMODELS_EXPORT auto generateTableString(const QString &header) -> const QString override;

  /*!
   * \brief Generates an HTML form string for display or export.
   * \param header The header text for the form.
   * \return A QString containing the HTML representation of the form.
   */
  virtual JMBDEMODELS_EXPORT auto generateFormularString(const QString &header) -> const QString override;

  // Getter
  /*!
   * \brief Retrieves the table name.
   * \return A QString containing the table name.
   */
  JMBDEMODELS_EXPORT auto getTableName() const -> QString { return m_tableName; }

  /*!
   * \brief Retrieves the database connection.
   * \return A QSqlDatabase object representing the database connection.
   */
  JMBDEMODELS_EXPORT auto getDB() const -> QSqlDatabase { return m_db; }

  /*!
   * \brief Gets the index of the `company_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getCompanyIdIndex() const { return m_CompanyIdIndex; }

  /*!
   * \brief Gets the index of the `name` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getNameIndex() const { return m_NameIndex; }

  /*!
   * \brief Gets the index of the `name2_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getName2Index() const { return m_Name2Index; }

  /*!
   * \brief Gets the index of the `street_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getStreetIndex() const { return m_StreetIndex; }

  /*!
   * \brief Gets the index of the `city` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getCityIndex() const { return m_CityIndex; }

  /*!
   * \brief Gets the index of the `zip_code` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getZipCodeIndex() const { return m_ZipCodeIndex; }

  /*!
   * \brief Gets the index of the `phone_number_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getPhoneNumberIndex() const {
    return m_PhoneNumberIndex;
  }

  /*!
   * \brief Gets the index of the `fax_number_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getFaxNumberIndex() const { return m_FaxNumberIndex; }

  /*!
   * \brief Gets the index of the `mobile_number` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getMobileNumberIndex() const {
    return m_MobileNumberIndex;
  }

  /*!
   * \brief Gets the index of the `mail_address_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getMailAddressIndex() const {
    return m_MailAddressIndex;
  }

  /*!
   * \brief Gets the index of the `active` field in the database.
   * \return The index as an integer.
   */
   JMBDEMODELS_EXPORT int getActiveIndex() const { return m_ActiveIndex; }

   /*!
    * \brief Gets the index of the `employee_id` field in the database.
    * \return The index as an integer.
    */
  JMBDEMODELS_EXPORT int getEmployeeIdIndex() const {
    return m_EmployeeIdIndex;
  }

  /*!
   * \brief Gets the index of the `last_update_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getLastUpdateIndex() const {
    return m_LastUpdateIndex;
  }

private:
/*!
 * \brief The name of the database table used for companies.
 */
  const QString m_tableName = QLatin1String("company"); ///< QString m_tableName

  /*!
   * @ brief m_db
   */
  QSqlDatabase m_db = {};

  /*!
   * \brief Pointer to the DataContext managing database connections.
   */
  Model::DataContext *m_dataContext = nullptr;

  // Field indices
  int m_CompanyIdIndex{-1};    ///< int CompanyIdIndex
  int m_NameIndex{-1};         ///< int NameIndex
  int m_Name2Index{-1};        ///< int Name2Index
  int m_StreetIndex{-1};       ///< int m_StreetIndex
  int m_CityIndex{-1};         ///< int CityIndex
  int m_ZipCodeIndex{-1};      ///< int ZipCodeIndex
  int m_PhoneNumberIndex{-1};  ///< int PhoneNumberIndex
  int m_FaxNumberIndex{-1};    ///< int FaxNumberIndex
  int m_MobileNumberIndex{-1}; ///< int MobileNumberIndex
  int m_MailAddressIndex{-1};  ///< int MailAddressIndex
  int m_ActiveIndex{-1};       ///< int ActiveIndex
  int m_EmployeeIdIndex{-1};   ///< int EmployeeIdIndex
  int m_LastUpdateIndex{-1};   ///< int LastUpdateIndex
};
} // namespace Model
