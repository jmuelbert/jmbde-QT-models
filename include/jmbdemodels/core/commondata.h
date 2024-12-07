/*
 *  SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert
 *  <juergen.muelbert@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QAbstractTableModel>
#include <QItemSelectionModel>
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QString>
#include <QTextDocument>
#include <QtSql>
#include <memory>

#include "datacontext.h"
#include "jmbdemodels_export.h"

Q_DECLARE_LOGGING_CATEGORY(commonDataLog)

namespace jmbde {
namespace models {

/**
 * @brief Provides a base class for database interactions, offering utilities for model creation,
 *        CSS generation, and HTML representation of data.
 * 
 * This class abstracts common operations for database-driven applications, 
 * enabling integration with `QAbstractTableModel` and SQL models.
 * 
 * @author Jürgen Mülbert
 * @since 0.4
 * @version 0.8
 * @date 2024-11-25
 * @license GPL-3.0-or-later
 */
class JMBDEMODELS_EXPORT CommonData : public QAbstractTableModel {
    Q_OBJECT

public:
    // Smart pointer aliases
    using Ptr = std::shared_ptr<CommonData>;
    using ConstPtr = std::shared_ptr<const CommonData>;

    /**
     * @brief Enumeration of supported model types.
     */
    enum class ModelType {
        Relational,  ///< Relational table model
        Table        ///< Standard table model
    };

    /**
     * @brief Constructor with DataContext.
     * @param context Pointer to the DataContext instance.
     * @param parent Parent QObject (optional).
     */
    explicit CommonData(DataContext* context, QObject* parent = nullptr);

    /**
     * @brief Constructor without DataContext, initializes its own context.
     * @param parent Parent QObject (optional).
     */
    explicit CommonData(QObject* parent = nullptr);

    /**
     * @brief Destructor for cleaning up allocated resources.
     */
    virtual ~CommonData() override;

    // Prevent copying
    CommonData(const CommonData&) = delete;
    CommonData& operator=(const CommonData&) = delete;

    // Allow moving
    CommonData(CommonData&&) noexcept = default;
    CommonData& operator=(CommonData&&) noexcept = default;

    // Required QAbstractTableModel overrides
    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Generate an HTML representation of a table.
     * @param header Table header.
     * @return HTML string.
     */
    [[nodiscard]] virtual QString generateTableString(const QString& header) const = 0;

    /**
     * @brief Generate an HTML representation of a form.
     * @param header Form header.
     * @return HTML string.
     */
    [[nodiscard]] virtual QString generateFormularString(const QString& header) const = 0;

    /**
     * @brief Create a QTextDocument for export or printing.
     * @return Unique pointer to a QTextDocument.
     */
    [[nodiscard]] static std::unique_ptr<QTextDocument> createSheet();

    /**
     * @brief Retrieve CSS for styling tables.
     * @return CSS string.
     */
    [[nodiscard]] static QString tableStyle();

    /**
     * @brief Retrieve CSS for styling forms.
     * @return CSS string.
     */
    [[nodiscard]] static QString formStyle();

protected:
    /**
     * @brief Create an SQL table model of the specified type.
     * @param type The type of model to create (Relational or Table).
     * @param tableName The name of the database table.
     * @return Unique pointer to the created model.
     */
    [[nodiscard]] std::unique_ptr<QSqlTableModel> createModel(ModelType type, const QString& tableName);

    /**
     * @brief Create a relational table model.
     * @param tableName The name of the table.
     * @param parent Optional parent QObject.
     * @return Unique pointer to a relational table model.
     */
    [[nodiscard]] std::unique_ptr<QSqlRelationalTableModel> createRelationalModel(const QString& tableName, QObject* parent = nullptr);

    /**
     * @brief Create a standard table model.
     * @param tableName The name of the table.
     * @param parent Optional parent QObject.
     * @return Unique pointer to a table model.
     */
    [[nodiscard]] std::unique_ptr<QSqlTableModel> createTableModel(const QString& tableName, QObject* parent = nullptr);

    /**
     * @brief Locate the index of a column by its name.
     * @param model The table model to search within.
     * @param columnName The name of the column.
     * @return The column index, or -1 if not found.
     */
    [[nodiscard]] int findColumnIndex(const QSqlTableModel* model, const QString& columnName) const;

    /**
     * @brief Generate an HTML table with headers and data rows.
     * @param headers List of table headers.
     * @param rows List of data rows (each row is a list of strings).
     * @return HTML string representing the table.
     */
    [[nodiscard]] QString generateHtmlTable(const QStringList& headers, const QList<QStringList>& rows) const;

protected slots:
    /**
     * @brief Handle changes to model data.
     * @param topLeft Top-left index of the changed range.
     * @param bottomRight Bottom-right index of the changed range.
     */
    void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    /**
     * @brief Handle model reset.
     */
    void onModelReset();

private:
    // Private member variables
    std::unique_ptr<DataContext> m_dbContext;              ///< Pointer to the database context.
    std::unique_ptr<QSqlRelationalTableModel> m_model;     ///< Pointer to the current SQL model.
    std::unique_ptr<QItemSelectionModel> m_selectionModel; ///< Pointer to the selection model.

    /**
     * @brief Initialize the data model.
     */
    void initializeModel();

    /**
     * @brief Setup internal signal-slot connections.
     */
    void setupConnections();
};

} // namespace models
} // namespace jmbde
