/*
 *  SPDX-FileCopyrightText: 2013-2021 Jürgen Mülbert
 * <juergen.muelbert@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QLoggingCategory>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRelation>
#include <QTextDocument>
#include <QtSql>

#include "commondata.h"
#include "datacontext.h"

#include "jmbdemodels_export.h"

namespace Model {
/*!
    \class SystemData
    \brief The SystemData class
    \details This Class is for the SystemData
    \author Jürgen Mülbert
    \since 0.2
    \version 0.7
    \date 25.12.2021
    \copyright GPL-3.0-or-later
    */
class SystemData : public CommonData {
  Q_OBJECT

public:
  /*!
      \fn  SystemData()

      \brief Constructor for the  SystemData
   */
  explicit JMBDEMODELS_EXPORT SystemData();

  /*!
      \fn  ~SystemData() override;

      \brief Destructor for SystemData
   */
  JMBDEMODELS_EXPORT ~SystemData() {}

  // implement the virtuals

  /*!
      \fn virtual void setIndexes()
      \brief Set the fieldindexes from the datafieldnames of the db.
   */
  virtual JMBDEMODELS_EXPORT void setIndexes() final;

  /*!
      \fn virtual JMBDEMODELS_EXPORT auto initializeRelationalModel() ->
     QSqlRelationalTableModel* final \brief set the QSqlRelationalTableModel for
     the DataModel \return The QSqlRelationalTableModel
   */
  virtual JMBDEMODELS_EXPORT auto initializeRelationalModel()
      -> QSqlRelationalTableModel * final;

  /*!
      \fn virtual auto initializeInputDataModel() -> QSqlRelationalTableModel*
     fina \brief Initialize the InputDataModel

       \return The QSqlRelationalTableModel
   */
  virtual JMBDEMODELS_EXPORT auto initializeInputDataModel()
      -> QSqlRelationalTableModel * final;

  /*!
      \fn virtual auto initializeViewModel() -> QSqlTableModel* final
      \brief Initialize the ViewModel

      \return QSqlTableModel
   */
  virtual JMBDEMODELS_EXPORT auto initializeViewModel()
      -> QSqlTableModel * final;

  /*!
      \fn virtual auto initializeListModel() -> QSqlTableModel* final
      \brief Initiallize the list Model for select one dataset
  */
  virtual JMBDEMODELS_EXPORT auto initializeListModel()
      -> QSqlTableModel * final;

  /*!
   * \fn virtual auto generateTableString(const QString& header) -> QString
   final \brief generateTableString

      \return a QString with the generated Table for Output
   */
  virtual JMBDEMODELS_EXPORT auto generateTableString(const QString &header)
      -> QString final;

  /*!
      \fn virtual auto generateFormularString(const QString& header) -> QString
     final \brief generateFormularString

      \return a QString with the generated Table for Output
   */
  virtual JMBDEMODELS_EXPORT auto generateFormularString(const QString &header)
      -> QString final;

  // Getter
  /*!
   * \brief getTableName
   * \return
   */
  JMBDEMODELS_EXPORT auto getTableName() const -> QString {
    return this->m_tableName;
  }
  /*!
      \fn auot getDB() const -> QSqlDatabase
   */
  JMBDEMODELS_EXPORT auto getDB() const -> QSqlDatabase { return this->m_db; }

  /*!
      \fn  int SystemDataIdIndex()

      \brief Get the index of the fieldname SystemDataId from the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getSystemDataIdIndex() const {
    return m_SystemDataIdIndex;
  }

  /*!
      \fn  int NameIndex()

      \brief Get the index of the fieldname Name from the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getNameIndex() const { return m_NameIndex; }

  /*!
      \fn  int LocalIndex()

      \brief Get the index of the fieldname Local from the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getLocalIndex() const { return m_LocalIndex; }

  /*!
      \fn  int CompanyIdIndex()

      \brief Get the index of the fieldname CompanyId from the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getCompanyIdIndex() const { return m_CompanyIdIndex; }

  /*!
      \fn  int LastUpdateIndex()

      \brief Get the index of the fieldname LastUpdate form the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getLastUpdateIndex() const {
    return m_LastUpdateIndex;
  }

private:
  /*!
      \brief The Tablename in the database \e is const
   */
  const QString m_tableName = QLatin1String("system_data");

  /*!
   * @ brief m_db
   */
  QSqlDatabase m_db = {};

  /*!
   * @brief DataContext
   */
  Model::DataContext *m_dataContext = {};

  /*!
      \var int m_SystemDataIdIndex
      \brief The value of the SystemDataIdIndex
   */
  int m_SystemDataIdIndex{0};

  /*!
     \var int m_NameIndex
     \brief The value of the NameIndex
  */
  int m_NameIndex{0};

  /*!
     \var int m_LocalIndex
     \brief The value of the LocalIndex
  */
  int m_LocalIndex{0};

  /*!
     \var int m_CompanyIdIndex
     \brief The value of the CompanyIdIndex
  */
  int m_CompanyIdIndex{0};

  /*!
      \var int m_LastUpdateIndex
      \brief The value of the LastUpdateIndex
  */
  int m_LastUpdateIndex{0};
};
} // namespace Model
