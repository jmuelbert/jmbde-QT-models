#include <QTest>
#include "jmbdemodels/datacontext.h"

class TestDataContext : public Model::DataContext {
    Q_OBJECT

private slots:
    void init() {
        // Setup code before each test
        dataContext = new Model::DataContext();
    }

    void cleanup() {
        // Cleanup code after each test
        delete dataContext;
    }

    void testDefaultConstructor() {
        QVERIFY(dataContext != nullptr);
        QVERIFY(dataContext->getDatabase().isOpen()); // Überprüfen, ob die Datenbank geöffnet ist
    }

    void testParameterizedConstructor() {
        Model::DataContext *customContext = new Model::DataContext(nullptr, "TestDB");
        QVERIFY(customContext != nullptr);
        QVERIFY(customContext->getDatabase().isOpen());
        delete customContext;
    }

    void testOpenDatabase() {
        dataContext->open("TestDB");
        QVERIFY(dataContext->getDatabase().isOpen());
    }

    void testRenameDatabase() {
        QString oldName = "TestDB";
        QString newName = "RenamedDB";
        dataContext->open(oldName);
        dataContext->renameDB(newName);
        // Überprüfen, ob die Datenbank umbenannt wurde
        QVERIFY(!dataContext->getDatabase().isOpen()); // Die alte Verbindung sollte geschlossen sein
        dataContext->open(newName);
        QVERIFY(dataContext->getDatabase().isOpen());
    }

    void testDeleteDatabase() {
        QString dbName = "TestDB";
        dataContext->open(dbName);
        dataContext->deleteDB(dbName);
        // Überprüfen, ob die Datenbankdatei gelöscht wurde
        QVERIFY(!QFile::exists(dataContext->getConnectionString()));
    }

    void testCheckExistence() {
        // Hier könnte man eine Tabelle und einen Wert angeben, um die Existenz zu überprüfen
        // Beispiel: Überprüfen, ob ein bestimmter Eintrag existiert
        QVERIFY(dataContext->checkExistence("some_table", "id", "1"));
    }

    /**
    void testInsert() {
        QVariantMap data;
        data["column1"] = "value1";
        data["column2"] = "value2";
        QVERIFY(dataContext->insert("some_table", data)); // Überprüfen, ob der Insert erfolgreich war
    }

    void testUpdate() {
        QVERIFY(dataContext->update("some_table", "column1", "new_value", "id", "1"));
    }

    void testCheckDBVersion() {
        // Hier könnte man eine Version in der Datenbank setzen und dann überprüfen
        // ob die Methode die richtige Version zurückgibt.
        QVERIFY(dataContext->checkDBVersion("1.0", "1"));
    }
*/
private:
    Model::DataContext *dataContext;
};

QTEST_MAIN(TestDataContext)
#include "test_datacontext.moc"
