
#include <QTest>

#include <QSqlDatabase>

#include "jmbdemodels/account.h"

class TestAccount : public QObject {
    Q_OBJECT

private slots:
    void init() {
        // Setup code before each test
        account = new Model::Account();
    }

    void cleanup() {
        // Cleanup code after each test
        delete account;
    }

    void testConstructor() {
        QVERIFY(account != nullptr);
        QSqlDatabase dataBase = account->getDB();
        QVERIFY(dataBase.isOpen());
    }

    void testSetIndexes() {
        account->setIndexes();
        QVERIFY(account->getAccountIdIndex() >= 0);
        QVERIFY(account->getUserNameIndex() >= 0);
        QVERIFY(account->getPasswordIndex() >= 0);
        QVERIFY(account->getSystemDataIndex() >= 0);
        QVERIFY(account->getLastUpdateIndex() >= 0);
    }

    void testInitializeRelationalModel() {
        QSqlRelationalTableModel *model = account->initializeRelationalModel();
        QVERIFY(model != nullptr);
        QCOMPARE(model->tableName(), account->getTableName());
    }

    void testInitializeInputDataModel() {
        QSqlRelationalTableModel *model = account->initializeInputDataModel();
        QVERIFY(model != nullptr);
        QCOMPARE(model->tableName(), account->getTableName());
    }

    void testInitializeViewModel() {
        QSqlTableModel *model = account->initializeViewModel();
        QVERIFY(model != nullptr);
        // Hier könnte man auch die Anzahl der Zeilen überprüfen, wenn Daten vorhanden sind
    }

    void testInitializeListModel() {
        QSqlTableModel *listModel = account->initializeListModel();
        QVERIFY(listModel != nullptr);
        QCOMPARE(listModel->tableName(), account->getTableName());
    }

    void testGenerateTableString() {
        QString header = "Test Header";
        QString result = account->generateTableString(header);
        QVERIFY(!result.isEmpty());
        QVERIFY(result.contains(header));
    }

    void testGenerateFormularString() {
        QString header = "Test Header";
        QString result = account->generateFormularString(header);
        QVERIFY(!result.isEmpty());
        QVERIFY(result.contains(header));
    }

private:
    Model::Account *account;
};

QTEST_MAIN(TestAccount);

#include "test_account.moc"
