#ifndef CONTENTMODEL_H
#define CONTENTMODEL_H

#include <QObject>

class ContentModel : public QObject
{
public:
    ContentModel(QString uuid = "", QString title = "", int createDate = 0, int updateDate = 0, QString body = "",
                 QString filePath = "");

    void setUuid(QString uuid = "");

    void setTitle(QString title);

    void setCreateDate(QString createDate = "");

    void setCreateDate(int createDate = 0);

    void setUpdateDate(QString updateDate = "");

    void setUpdateDate(int updateDate = 0);

    void setBody(QString body);

    void setFilePath(const QString &filePath);

    QString getUuid();

    QString getTitle();

    int getCreateDate();

    int getUpdateDate();

    QString getBody();

    const QString &getFilePath() const;

private:
    QString uuid;
    QString title;
    int createDate;
    int updateDate;
    QString body;
    QString filePath;

};

#endif // CONTENTMODEL_H