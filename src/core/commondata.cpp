/*
 *  SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert
 * <juergen.muelbert@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

 #include <QDebug>
 #include <QLatin1String>
 #include <QSqlError>
 #include <QSqlQuery>
 #include <QSqlRecord>
 #include <QtGlobal>
 #include <memory>

 Q_LOGGING_CATEGORY(commonDataLog, "jmbde.models.commondata")

 #include "jmbdemodels/commondata.h"

 namespace jmbde {
 namespace models {

 namespace {
 // Consolidated CSS styles for reuse
 constexpr auto DEFAULT_TABLE_STYLE = R"(
     table {
         border-collapse: collapse;
         width: 100%;
         margin: 1em 0;
         font-family: Arial, sans-serif;
     }
     th, td {
         border: 1px solid #ddd;
         padding: 8px;
         text-align: left;
     }
     th {
         background-color: #f5f5f5;
         font-weight: bold;
     }
     tr:nth-child(even) {
         background-color: #f9f9f9;
     }
     tr:hover {
         background-color: #f0f0f0;
     }
 )";

 constexpr auto DEFAULT_FORM_STYLE = R"(
     .form-container {
         max-width: 800px;
         margin: 0 auto;
         padding: 20px;
     }
     .form-group {
         margin-bottom: 15px;
     }
     .form-label {
         display: block;
         margin-bottom: 5px;
         font-weight: bold;
     }
     .form-input {
         width: 100%;
         padding: 8px;
         border: 1px solid #ddd;
         border-radius: 4px;
     }
 )";
 } // namespace

 CommonData::CommonData(DataContext* context, QObject* parent)
     : QAbstractTableModel(parent)
     , m_dbContext(context ? context : new DataContext(this))
 {
     try {
         initializeModel();
         setupConnections();
         qCInfo(commonDataLog) << "CommonData successfully initialized.";
     } catch (const std::exception& e) {
         qCCritical(commonDataLog) << "Initialization failed:" << e.what();
     }
 }

 CommonData::CommonData(QObject* parent)
     : CommonData(nullptr, parent)
 {}

 CommonData::~CommonData() = default;

 void CommonData::initializeModel()
 {
     if (!m_dbContext->isConnected()) {
         throw std::runtime_error("Database connection failed");
     }
     qCInfo(commonDataLog) << "Database context connected successfully.";
 }

 void CommonData::setupConnections()
 {
     connect(this, &QAbstractItemModel::dataChanged, this, &CommonData::onDataChanged);
     connect(this, &QAbstractItemModel::modelReset, this, &CommonData::onModelReset);
 }

 std::unique_ptr<QSqlTableModel> CommonData::createTableModel(const QString& tableName, QObject* parent)
 {
     auto model = std::make_unique<QSqlTableModel>(parent ? parent : this, QSqlDatabase::database());
     if (!model->setTable(tableName)) {
         qCCritical(commonDataLog) << "Failed to set table:" << model->lastError().text();
         return nullptr;
     }
     model->setEditStrategy(QSqlTableModel::OnRowChange);
     if (!model->select()) {
         qCCritical(commonDataLog) << "Failed to select data:" << model->lastError().text();
         return nullptr;
     }
     return model;
 }

 std::unique_ptr<QSqlRelationalTableModel> CommonData::createRelationalModel(const QString& tableName, QObject* parent)
 {
     auto model = std::make_unique<QSqlRelationalTableModel>(parent ? parent : this, QSqlDatabase::database());
     if (!model->setTable(tableName)) {
         qCCritical(commonDataLog) << "Failed to set table:" << model->lastError().text();
         return nullptr;
     }
     model->setEditStrategy(QSqlTableModel::OnFieldChange);
     if (!model->select()) {
         qCCritical(commonDataLog) << "Failed to select data:" << model->lastError().text();
         return nullptr;
     }
     return model;
 }

 QString CommonData::generateHtmlTable(const QStringList& headers, const QList<QStringList>& rows) const
 {
     QString html = R"(<table style=")" + tableStyle() + R"(">)";

     if (!headers.isEmpty()) {
         html += "<thead><tr>";
         for (const auto& header : headers) {
             html += "<th>" + header.toHtmlEscaped() + "</th>";
         }
         html += "</tr></thead>";
     }

     if (!rows.isEmpty()) {
         html += "<tbody>";
         for (const auto& row : rows) {
             html += "<tr>";
             for (const auto& cell : row) {
                 html += "<td>" + cell.toHtmlEscaped() + "</td>";
             }
             html += "</tr>";
         }
         html += "</tbody>";
     }

     html += "</table>";
     return html;
 }

 QString CommonData::tableStyle()
 {
     return QLatin1String(DEFAULT_TABLE_STYLE);
 }

 QString CommonData::formStyle()
 {
     return QLatin1String(DEFAULT_FORM_STYLE);
 }

 void CommonData::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
 {
     qCInfo(commonDataLog) << "Data changed:" << topLeft << "to" << bottomRight;
 }

 void CommonData::onModelReset()
 {
     qCInfo(commonDataLog) << "Model reset.";
 }

 int CommonData::rowCount(const QModelIndex& parent) const
 {
     return (!parent.isValid() && m_model) ? m_model->rowCount() : 0;
 }

 int CommonData::columnCount(const QModelIndex& parent) const
 {
     return (!parent.isValid() && m_model) ? m_model->columnCount() : 0;
 }

 QVariant CommonData::data(const QModelIndex& index, int role) const
 {
     return (index.isValid() && m_model) ? m_model->data(index, role) : QVariant();
 }

 QHash<int, QByteArray> CommonData::roleNames() const
 {
     return {{Qt::DisplayRole, "display"}};
 }

 } // namespace models
 } // namespace jmbde
