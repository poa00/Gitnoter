#include "database.h"
#include "tools.h"
#include "sqlquerybuilder.h"
#include "appconfig.h"

using namespace SqlQueryBuilder;

Database::Database()
{
    this->connect(AppConfig::dbName);
}

Database::Database(const QString filename)
{
    this->connect(filename);
}

Database::~Database()
{
    db.close();
}

void Database::connect(const QString filename)
{
    QDir dir(AppConfig::dbPath);
    if (!dir.exists()) {
        dir.mkpath(AppConfig::dbPath);
        qDebug() << "dir is not exists";
    }

    QString path = dir.filePath(filename);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (db.open()) {
        qDebug() << "Database: connection ok";
        query = QSqlQuery(db);
        this->initTables();
    }
    else {
        qDebug() << "Error: connection with database fail";
    }
}

void Database::createTables()
{
    QFile tablesFile(":/database/gitnoter.sql");
    tablesFile.open(QIODevice::ReadOnly);
    QString tablesSql = QString(tablesFile.readAll());
    QStringList sqlList = tablesSql.split("\n");

    for (int i = 0; i < sqlList.length(); ++i) {
        if (!sqlList[i].isEmpty()) {
            query.exec(sqlList[i]);
        }
        query.clear();
    }
}

void Database::initTables()
{
    if (!query.exec("select * from config")) {
        query.clear();
        this->createTables();
    }
}

NoteModel* Database::selectNoteByUuid(QString uuid)
{
    NoteModel *result = nullptr;


    return result;
}

void Database::insertNoteText(NoteModel *noteModel) {
//    QString uuid = this->insertNotesTable(noteModel->noteTableModel);
//    for (auto &&tagTableModel : *noteModel->tagTableList) {
//        tagTableModel->setNotesUuid(uuid);
//        this->insertTagsTable(tagTableModel->getName());
//    }
//    noteModel->categoriseTableModel->setNotesUuid(uuid);
//    this->insertCategorie(noteModel->categoriseTableModel);
}

QString Database::insertNotesTable(NoteTableModel *noteTableModel)
{
    InsertModel insertBuilder;
    insertBuilder.insert("uuid", Tools::getUuid().toStdString())
            ("title", noteTableModel->getTitle().toStdString())
            ("create_date", noteTableModel->getCreateDate())
            ("update_date", noteTableModel->getUpdateDate())
            ("body", noteTableModel->getBody().toStdString()).into("notes");
    QString sql = QString::fromStdString(insertBuilder.str());

    QString insertId = "";
    if (!query.exec(sql)) {
        qDebug() << "void Database::insertNotes(NoteModel noteModel): Failed!";
    }
    else {
        insertId = query.lastInsertId().toString();
    }

    return insertId;
}

bool Database::deleteNotesTableByUuid(QString uuid)
{
    DeleteModel deleteBuilder;
    deleteBuilder._delete().from("notes").where(column("uuid") == uuid.toStdString());
    QString sql = QString::fromStdString(deleteBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

bool Database::updateNotesTableByUuid(QString uuid, QString title, QString body, int updateDate)
{
    UpdateModel updateBuilder;
    updateBuilder.update("notes")
            .set("title", title.toStdString())
                    ("body", body.toStdString())
                    ("update_date", updateDate)
            .where(column("uuid") == uuid.toStdString());
    QString sql = QString::fromStdString(updateBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

NoteTableModel* Database::selectNotesTableByUuid(QString uuid)
{
    SelectModel selectBuilder;
    selectBuilder.select("title", "create_date", "update_date", "body")
            .from("notes")
            .where(column("uuid") == uuid.toStdString());
    QString sql = QString::fromStdString(selectBuilder.str());

    NoteTableModel *result = nullptr;
    if (query.exec(sql)) {
        query.first();
        QString title = query.value(0).toString();
        int createDate = query.value(1).toInt();
        int updateDate = query.value(2).toInt();
        QString body = query.value(3).toString();
        result = new NoteTableModel(uuid, title, createDate, updateDate, body);
    }
    else {
        qDebug() << "QList<NoteModel *> Database::selectNoteList(): failed!";
    }

    return result;
}

int Database::insertTagsTable(QString name)
{
    InsertModel insertBuilder;
    insertBuilder.insert("name", name.toStdString())
            ("create_date", QDateTime::currentSecsSinceEpoch())
            ("update_date", QDateTime::currentSecsSinceEpoch()).into("tags");
    QString sql = QString::fromStdString(insertBuilder.str());

    int insertId = 0;
    if (!query.exec(sql)) {
        qDebug() << "void Database::insertNotes(NoteModel noteModel): Failed!";
    }
    else {
        insertId = query.lastInsertId().toInt();
    }

    return insertId;
}

bool Database::deleteTagsTableByName(QString name)
{
    DeleteModel deleteBuilder;
    deleteBuilder._delete().from("tags").where(column("name") == name.toStdString());
    QString sql = QString::fromStdString(deleteBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

bool Database::updateTagsTableByName(QString name, QString whereName)
{
    UpdateModel updateBuilder;
    updateBuilder.update("notes")
            .set("name", name.toStdString())
                    ("update_date", QDateTime::currentSecsSinceEpoch())
            .where(column("name") == whereName.toStdString());
    QString sql = QString::fromStdString(updateBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

QList<TagTableModel *> Database::selectTagsTable()
{
    SelectModel selectBuilder;
    selectBuilder.select("id", "name", "create_date", "update_date").from("tags");
    QString sql = QString::fromStdString(selectBuilder.str());

    QList<TagTableModel *> result;
    if (query.exec(sql)) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            int createDate = query.value(2).toInt();
            int updateDate = query.value(3).toInt();

            TagTableModel *tagTableModel = new TagTableModel(id, name, createDate, updateDate);
            result.append(tagTableModel);
        }
    }
    else {
        qDebug() << "QList<NoteModel *> Database::selectNoteList(): failed!";
    }

    return result;
}

TagTableModel* Database::selectTagsTableById(int id)
{
    SelectModel selectBuilder;
    selectBuilder.select("id", "name", "create_date", "update_date").from("tags").where(column("id") == id);
    QString sql = QString::fromStdString(selectBuilder.str());

    TagTableModel *result = nullptr;
    if (query.exec(sql)) {
        QString name = query.value(1).toString();
        int createDate = query.value(2).toInt();
        int updateDate = query.value(3).toInt();

        result = new TagTableModel(id, name, createDate, updateDate);
    }
    else {
        qDebug() << "QList<NoteModel *> Database::selectNoteList(): failed!";
    }

    return result;
}

int Database::insertCategoriesTable(QString name)
{
    InsertModel insertBuilder;
    insertBuilder.insert("name", name.toStdString())
            ("create_date", QDateTime::currentSecsSinceEpoch())
            ("update_date", QDateTime::currentSecsSinceEpoch()).into("categories");
    QString sql = QString::fromStdString(insertBuilder.str());

    int insertId = 0;
    if (!query.exec(sql)) {
        qDebug() << "int Database::insertCategoriesTable(QString name): Failed!";
    }
    else {
        insertId = query.lastInsertId().toInt();
    }

    return insertId;
}

bool Database::deleteCategoriesTableByName(QString name)
{
    DeleteModel deleteBuilder;
    deleteBuilder._delete().from("categories").where(column("name") == name.toStdString());
    QString sql = QString::fromStdString(deleteBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

bool Database::updateCategoriesTableByName(QString name, QString whereName)
{
    UpdateModel updateBuilder;
    updateBuilder.update("categories")
            .set("name", name.toStdString())
                    ("update_date", QDateTime::currentSecsSinceEpoch())
            .where(column("name") == whereName.toStdString());
    QString sql = QString::fromStdString(updateBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

QList<CategoriseTableModel *> Database::selectCategoriesTable()
{
    SelectModel selectBuilder;
    selectBuilder.select("id", "name", "create_date", "update_date", "is_default").from("categories");
    QString sql = QString::fromStdString(selectBuilder.str());

    QList<CategoriseTableModel *> result;
    if (query.exec(sql)) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            int createDate = query.value(2).toInt();
            int updateDate = query.value(3).toInt();
            int isDefault = query.value(4).toInt();

            CategoriseTableModel *categoriseTableModel = new CategoriseTableModel(id, name, createDate,
                                                                                  updateDate, isDefault);
            result.append(categoriseTableModel);
        }
    }
    else {
        qDebug() << "QList<NoteModel *> Database::selectNoteList(): failed!";
    }

    return result;
}

CategoriseTableModel* Database::selectCategoriesTableById(int id)
{
    SelectModel selectBuilder;
    selectBuilder.select("id", "name", "create_date", "update_date", "is_default").from("categories").where(column("id") == id);
    QString sql = QString::fromStdString(selectBuilder.str());

    CategoriseTableModel *result = nullptr;
    if (query.exec(sql)) {
        QString name = query.value(1).toString();
        int createDate = query.value(2).toInt();
        int updateDate = query.value(3).toInt();
        int isDefault = query.value(4).toInt();

        result = new CategoriseTableModel(id, name, createDate, updateDate, isDefault);
    }
    else {
        qDebug() << "QList<NoteModel *> Database::selectNoteList(): failed!";
    }

    return result;
}

int Database::insertNJTTable(QString notesUuid, int tagsId)
{
    InsertModel insertBuilder;
    insertBuilder.insert("tags_id", tagsId)
            ("notes_uuid", notesUuid.toStdString())
            ("create_date", QDateTime::currentSecsSinceEpoch()).into("notes_join_tags");
    QString sql = QString::fromStdString(insertBuilder.str());

    int insertId = 0;
    if (!query.exec(sql)) {
        qDebug() << "int Database::insertCategoriesTable(QString name): Failed!";
    }
    else {
        insertId = query.lastInsertId().toInt();
    }

    return insertId;
}

bool Database::deleteNJTTableById(int id)
{
    DeleteModel deleteBuilder;
    deleteBuilder._delete().from("notes_join_tags").where(column("id") == id);
    QString sql = QString::fromStdString(deleteBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

bool Database::deleteNJTTableById(QString notesUuid, int tagsId)
{
    DeleteModel deleteBuilder;
    deleteBuilder._delete().from("notes_join_tags")
            .where(column("notes_uuid") == notesUuid.toStdString() and column("tags_id") == tagsId);
    QString sql = QString::fromStdString(deleteBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

QList<TagTableModel *> Database::selectNJTTableByNotesUuid(QString notesUuid)
{
    SelectModel selectBuilder;
    selectBuilder.select("id", "tags_id", "create_date").from("notes_join_tags")
            .where(column("notes_uuid") == notesUuid.toStdString());
    QString sql = QString::fromStdString(selectBuilder.str());

    QList<TagTableModel *> result;
    if (query.exec(sql)) {
        while (query.next()) {
            int id = query.value(0).toInt();
            int tagsId = query.value(1).toInt();
            int createDate = query.value(2).toInt();

            TagTableModel *tagTableModel = new TagTableModel(id, tagsId, notesUuid, createDate);
            result.append(tagTableModel);
        }
    }
    else {
        qDebug() << "QList<NoteModel *> Database::selectNoteList(): failed!";
    }

    return result;
}

QList<TagTableModel *> Database::selectNJTTableByTagsId(int tagsId)
{
    SelectModel selectBuilder;
    selectBuilder.select("id", "notes_Uuid", "create_date").from("notes_join_tags")
            .where(column("tags_id") == tagsId);
    QString sql = QString::fromStdString(selectBuilder.str());

    QList<TagTableModel *> result;
    if (query.exec(sql)) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString notesUuid = query.value(1).toString();
            int createDate = query.value(2).toInt();

            TagTableModel *tagTableModel = new TagTableModel(id, tagsId, notesUuid, createDate);
            result.append(tagTableModel);
        }
    }
    else {
        qDebug() << "QList<TagTableModel> Database::selectNJTTableByTagsId(int tagsId): failed!";
    }

    return result;
}


int Database::insertNJCTable(QString notesUuid, int categoriesId)
{
    InsertModel insertBuilder;
    insertBuilder.insert("categories_id", categoriesId)
            ("notes_uuid", notesUuid.toStdString())
            ("create_date", QDateTime::currentSecsSinceEpoch()).into("notes_join_categories");
    QString sql = QString::fromStdString(insertBuilder.str());

    int insertId = 0;
    if (!query.exec(sql)) {
        qDebug() << "int Database::insertCategoriesTable(QString name): Failed!";
    }
    else {
        insertId = query.lastInsertId().toInt();
    }

    return insertId;
}

bool Database::deleteNJCTableById(int id)
{
    DeleteModel deleteBuilder;
    deleteBuilder._delete().from("notes_join_categories").where(column("id") == id);
    QString sql = QString::fromStdString(deleteBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

bool Database::deleteNJCTableById(QString notesUuid, int categoriesId)
{
    DeleteModel deleteBuilder;
    deleteBuilder._delete().from("notes_join_categories")
            .where(column("notes_uuid") == notesUuid.toStdString() and column("categories_id") == categoriesId);
    QString sql = QString::fromStdString(deleteBuilder.str());
    query.exec(sql);

    return query.numRowsAffected() != -1;
}

QList<CategoriseTableModel *> Database::selectNJCTableByNotesUuid(QString notesUuid)
{
    SelectModel selectBuilder;
    selectBuilder.select("id", "categories_id", "create_date").from("notes_join_categories")
            .where(column("notes_uuid") == notesUuid.toStdString());
    QString sql = QString::fromStdString(selectBuilder.str());

    QList<CategoriseTableModel *> result;
    if (query.exec(sql)) {
        while (query.next()) {
            int id = query.value(0).toInt();
            int categoriesId = query.value(1).toInt();
            int createDate = query.value(2).toInt();

            CategoriseTableModel *tagTableModel = new CategoriseTableModel(id, categoriesId, notesUuid, createDate);
            result.append(tagTableModel);
        }
    }
    else {
        qDebug() << "QList<NoteModel *> Database::selectNoteList(): failed!";
    }

    return result;
}

QList<CategoriseTableModel *> Database::selectNJCTableByTagsId(int categoriesId)
{
    SelectModel selectBuilder;
    selectBuilder.select("id", "notes_Uuid", "create_date").from("notes_join_categories")
            .where(column("categories_id") == categoriesId);
    QString sql = QString::fromStdString(selectBuilder.str());

    QList<CategoriseTableModel *> result;
    if (query.exec(sql)) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString notesUuid = query.value(1).toString();
            int createDate = query.value(2).toInt();

            CategoriseTableModel *tagTableModel = new CategoriseTableModel(id, categoriesId, notesUuid, createDate);
            result.append(tagTableModel);
        }
    }
    else {
        qDebug() << "QList<TagTableModel> Database::selectNJTTableByTagsId(int tagsId): failed!";
    }

    return result;
}
