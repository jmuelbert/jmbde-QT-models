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
    \class Inventory
    \brief The Inventory class
    \details In this is handle all Inventory
    \author Jürgen Mülbert
    \since 0.4
    \version 0.7
    \date 25.12.2021
    \copyright GPL-3.0-or-later
    */

class Inventory : public CommonData {
  Q_OBJECT

public:
  /*!
      \fn explicit Inventory()
      \brief The Constructor for the Inventory
  */
  explicit JMBDEMODELS_EXPORT Inventory();

  /*!
      \fn  ~Inventory() override;

      \brief Destructor for Inventory
   */
  JMBDEMODELS_EXPORT ~Inventory() {}

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
      \fn int InventoryIdIndex()

      \brief Get the index of the fieldname InventoryId form the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getInventoryIdIndex() const {
    return m_InventoryIdIndex;
  }

  /*!
      \fn int NumberIndex()

      \brief Get the index of the fieldname Number form the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getNumberIndex() const { return m_NumberIndex; }

  /*!
      \fn int DescriptionIndex()

      \brief Get the index of the fieldname Description form the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getDescriptionIndex() const {
    return m_DescriptionIndex;
  }

  /*!
      \fn int ActiveIndex()

      \brief Get the index of the fieldname Active form the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getActiveIndex() const { return m_ActiveIndex; }

  /*!
      \fn  int int LastUpateIndex() const

      \brief Get the index of the fieldname LastUpdate form the database

      Returns the value of the index
   */
  JMBDEMODELS_EXPORT int getLastUpateIndex() const { return m_LastUpdateIndex; }

private:
  /*!
      \brief The Tablename in the database \e is const
   */
  const QString m_tableName = QLatin1String("inventory");

  /*!
   * @ brief m_db
   */
  QSqlDatabase m_db = {};

  /*!
   * @brief DataContext
   */
  Model::DataContext *m_dataContext = {};

  /*!
     \var int m_InventoryIdIndex
     \brief The value of the InventoryIdIndex
  */
  int m_InventoryIdIndex{0};

  /*!
     \var int m_NumberIndex
     \brief The value of the NumberIndex
  */
  int m_NumberIndex{0};

  /*!
     \var int m_DescriptionIndex
     \brief The value of the DescriptionIndex
  */
  int m_DescriptionIndex{0};

  /*!
     \var int m_ActiveIndex
     \brief The value of the ActiveIndex
  */
  int m_ActiveIndex{0};

  /*!
      \var int m_LastUpdateIndex
      \brief The value of the LastUpdateIndex
  */
  int m_LastUpdateIndex{0};
};
} // namespace Model
