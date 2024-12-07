/*
 *  SPDX-FileCopyrightText: 2013-2021 Jürgen Mülbert
 * <juergen.muelbert@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QDebug>
#include <QLatin1String>
#include <QList>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QString>
#include <Qt>
#include <QtGlobal>
#include <QtLogging>

#include "jmbdemodels/chipcardprofiledoor.h"
#include "jmbdemodels/commondata.h"

Model::ChipCardProfileDoor::ChipCardProfileDoor()
    : Model::CommonData, m_dataContext(new DataContext()),
      m_db(m_dataContext->getDatabase()) {
  // Set the Model
  this->m_model = new QSqlRelationalTableModel(this, this->m_db);
  this->m_model->setTable(this->m_tableName);
  this->m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  setIndexes();
}

void Model::ChipCardProfileDoor::setIndexes() {
  m_ChipCardProfileDoorIdIndex =
      this->m_model->fieldIndex(QLatin1String("chip_card_profile_door_id"));
  m_ChipCardProfileIdIndex =
      this->m_model->fieldIndex(QLatin1String("chip_card_profile_id"));
  m_ChipCardDoorsIdIndex =
      this->m_model->fieldIndex(QLatin1String("chip_card_door_id"));
  m_LastUpdateIndex = this->m_model->fieldIndex(QLatin1String("last_update"));
  if (m_ChipCardProfileDoorIdIndex == -1 || m_ChipCardProfileIdIndex == -1 ||
      m_ChipCardDoorsIdIndex == -1 || m_LastUpdateIndex == -1) {
    qWarning() << "One or more field indices are invalid!";
  }
}

auto Model::ChipCardProfileDoor::initializeRelationalModel()
    -> QSqlRelationalTableModel * {
  this->m_model = new QSqlRelationalTableModel(this, this->m_db);

  this->m_model->setTable(this->m_tableName);
  this->m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  this->m_model->select();

  return this->m_model;
}

auto Model::ChipCardProfileDoor::initializeInputDataModel()
    -> QSqlRelationalTableModel * {
  this->m_model = new QSqlRelationalTableModel(this, this->m_db);

  this->m_model->setTable(this->m_tableName);

  return this->m_model;
}

auto Model::ChipCardProfileDoor::initializeViewModel() -> QSqlTableModel * {
  this->m_model->select();

  return this->m_model;
}

auto Model::ChipCardProfileDoor::initializeListModel() -> QSqlTableModel * {
  auto *listModel = new QSqlTableModel(this, this->m_db);
  listModel->setTable(this->m_tableName);
  listModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  listModel->select();

  return listModel;
}

auto Model::ChipCardProfileDoor::generateTableString(const QString &header)
    -> QString {
  QString outString;

  qDebug() << "Header:" << header << "( Columns: " << m_model->columnCount()
           << " Rows: " << m_model->rowCount() << " )";

  const QList<int> set;

  // Document Title
  outString = QLatin1String("<h1>");
  outString += header;
  outString += QLatin1String("</h1>");
  outString += QLatin1String("<hr />");
  outString +=
      QLatin1String(R"(<table width="100%" cellspacing="0" class="tbl">)");
  outString += QLatin1String("<thead> <tr>");

  for (const auto i : set) {
    outString += QLatin1String("<th>");
    outString.append(m_model->headerData(i, Qt::Horizontal).toString());
    outString += QLatin1String("</th>");
  }

  return outString;
}

auto Model::ChipCardProfileDoor::generateFormularString(const QString &header)
    -> QString {
  QString outString;

  qDebug() << "Header:" << header << "( Columns: " << m_model->columnCount()
           << " Rows: " << m_model->rowCount() << " )";

  // Document Title
  outString = QLatin1String("<h1>");
  outString += header;
  outString += QLatin1String("</h1>");
  outString += QLatin1String("<hr />");

  return outString;
}
