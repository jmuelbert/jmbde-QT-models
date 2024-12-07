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
  * \class ChipCardProfile
  * \brief Handles employee account data, providing database interaction and
  *        data model management.
  * \author Jürgen Mülbert
  * \since 0.4
  * \version 0.8
  * \date 25.11.2024
  * \copyright GPL-3.0-or-later
  */

  class ChipCardProfile : public CommonData {
  Q_OBJECT

public:
/*!
 * \brief Constructor: Initializes the account model and database connection.
 */
  explicit JMBDEMODELS_EXPORT ChipCardProfile();

  /*!
   * \brief Destructor: Cleans up resources used by the Account class.
   */
  JMBDEMODELS_EXPORT ~ChipCardProfile() {}

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
   * \brief Gets the index of the `chip_card_profile_Id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getChipCardProfileIdIndex() const {
    return m_ChipCardProfileIdIndex;
  }

  /*!
   * \brief Gets the index of the `number` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getNumberIndex() const { return m_NumberIndex; }

  /*!
   * \brief Gets the index of the `chip_card_door_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getChipCardDoorIdIndex() const {
    return m_ChipCardDoorIdIndex;
  }

  /*!
   * \brief Gets the index of the `employee_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getEmployeeIdIndex() const {
    return m_EmployeeIdIndex;
  }

  /*!
   * \brief Gets the index of the `last_update` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT int getLastUpdateIndex() const {
    return m_LastUpdateIndex;
  }

private:
/*!
 * \brief The name of the database table used for ChipCardProfiles.
 */
  const QString m_tableName = QLatin1String("chip_card_profile");

  /*!
   * \brief The database connection used by this model.
   */
  QSqlDatabase m_db = {};

  /*!
   * \brief Pointer to the DataContext managing database connections.
   */
  DataContext *m_dataContext = nullptr;

  // Field indices
  int m_ChipCardProfileIdIndex{-1}; ///< int m_ChipCardProfileIdIndex
  int m_NumberIndex{-1}; ///< int m_NumberIndex
  int m_ChipCardDoorIdIndex{-1}; ///< int m_ChipCardDoorIdIndex
  int m_EmployeeIdIndex{-1}; ///< int m_EmployeeIdIndex
  int m_LastUpdateIndex{-1}; ///< int m_LastUpdateIndex
};
} // namespace Model
