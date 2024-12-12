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

class ChipCardDoor : public CommonData {
  Q_OBJECT

public:
/*!
 * \brief Constructor: Initializes the ChipCardDoor model and database connection.
 */
  explicit JMBDEMODELS_EXPORT ChipCardDoor();

  /*!
   * \brief Destructor: Cleans up resources used by the Account class.
   */
  JMBDEMODELS_EXPORT ~ChipCardDoor() override;

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
  JMBDEMODELS_EXPORT auto getTableName() const -> QString { return this->m_tableName;  }

  /*!
   * \brief Retrieves the database connection.
   * \return A QSqlDatabase object representing the database connection.
   */
  JMBDEMODELS_EXPORT auto getDB() const -> QSqlDatabase { return this->m_db; }

  /*!
   * \brief Gets the index of the `ChipCardDoor_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT auto getChipCardDoorIdIndex() const -> int {
    return m_ChipCardDoorIdIndex;
  }

  /*!
   * \brief Gets the index of the `number` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT auto getNumberIndex() const -> int {
    return m_NumberIndex;
  }

  /*!
   * \brief Gets the index of the `place` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT auto getPlaceIdIndex() const -> int { return m_PlaceIdIndex; }

  /*!
   * \brief Gets the index of the `department_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT auto getDepartmentIdIndex() const -> int { return m_DepartmetIdIndex;  }

  /*!
   * \brief Gets the index of the `employeee_id` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT auto getEmployeeIdIndex() const -> int {
    return m_EmployeeIdIndex;
  }

  /*!
   * \brief Gets the index of the `last_update` field in the database.
   * \return The index as an integer.
   */
  JMBDEMODELS_EXPORT auto getLastUpdateIndex() const -> int {
    return m_LastUpdateIndex;
  }

private:
  /*!
      \brief The Tablename in the database \e is const
   */
  const QString m_tableName = QLatin1String("chip_card_door");

  /*!
   * @ brief m_db
   */
  QSqlDatabase m_db = {};

  /*!
   * @brief DataContext
   */
  Model::DataContext *m_dataContext = {};

  /*!
      \brief The value of the ChipCardDoorIdIndex
  */
  int m_ChipCardDoorIdIndex{0};

  /*!
      \brief The value of the NumberIndex
  */
  int m_NumberIndex{0};

  /*!
      \brief The value of the PlaceIdIndex
  */
  int m_PlaceIdIndex{0};

  /*!
      \brief The value of the DepartmetIdIndex
  */
  int m_DepartmetIdIndex{0};

  /*!
    \brief The value of the EmployeeIdIndex
*/
  int m_EmployeeIdIndex{0};

  /*!
      \brief The value of the LastUpdateIndex
  */
  int m_LastUpdateIndex{0};
};
} // namespace Model
