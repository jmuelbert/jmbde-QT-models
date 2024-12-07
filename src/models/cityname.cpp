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

#include "jmbdemodels/cityname.h"
#include "jmbdemodels/commondata.h"

Model::CityName::CityName()
    : Model::CommonData, m_dataContext(new DataContext()),
      m_db(m_dataContext->getDatabase()) {

  // Set the Model
  this->m_model = new QSqlRelationalTableModel(this, this->m_db);
  this->m_model->setTable(this->m_tableName);
  this->m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  setIndexes();
}

void Model::CityName::setIndexes() {
  m_CityNameIdIndex = this->m_model->fieldIndex(QLatin1String("city_name_id"));
  m_NameIndex = this->m_model->fieldIndex(QLatin1String("name"));
  m_LastUpdateIndex = this->m_model->fieldIndex(QLatin1String("last_update"));
  if (m_CityNameIdIndex == -1 || m_NameIndex == -1 || m_LastUpdateIndex == -1) {
    qWarning() << "One or more field indices are invalid!";
  }
}

auto Model::CityName::initializeRelationalModel()
    -> QSqlRelationalTableModel * {
  this->m_model = new QSqlRelationalTableModel(this, this->m_db);

  this->m_model->setTable(this->m_tableName);
  this->m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  this->m_model->select();

  return this->m_model;
}

auto Model::CityName::initializeListModel() -> QSqlTableModel * {
  auto *listModel = new QSqlTableModel(this, this->m_db);
  listModel->setTable(this->m_tableName);
  listModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  listModel->select();

  return listModel;
}

auto Model::CityName::initializeInputDataModel() -> QSqlRelationalTableModel * {
  this->m_model = new QSqlRelationalTableModel(this, this->m_db);

  this->m_model->setTable(this->m_tableName);

  return this->m_model;
}

auto Model::CityName::initializeViewModel() -> QSqlTableModel * {
  this->m_model->select();

  return this->m_model;
}

auto Model::CityName::generateTableString(const QString &header) -> QString {
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

  for (const auto headerText : set) {
    outString += QLatin1String("<th>");
    outString.append(
        m_model->headerData(headerText, Qt::Horizontal).toString());
    outString += QLatin1String("</th>");
  }

  return outString;
}

auto Model::CityName::generateFormularString(const QString &header) -> QString {
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
