#ifndef GLOBALS_H
#define GLOBALS_H

#include "categorymodellist.h"
#include "notemodellist.h"
#include "configmodel.h"
#include "tagmodellist.h"
#include "gitmanager.h"
#include "thememanager.h"

#include <QObject>

class Global : public QObject
{
Q_OBJECT

public:
    static const QString packageName;
    static const QString version;
    static const QString dateFormat;
    static const QString tagSplit;

    static const QString appDataLocation;
    static const QString appDataPath;
    static const QString appConfigPath;
    static const QString repoPath;
    static const QString repoNoteTextPath;
    static const QString noteTextFileName;
    static const QString noteDataFileName;
    static const QString repoNoteDataPath;
    static const QString repoCategoryListPath;
    static const QString repoTagsListPath;

    static const QString newNoteKeySequence;
    static const QString lockWindowKeySequence;
    static const QString cutWindowKeySequence;

    static const QByteArray aesKey;
    static const QByteArray aesIv;

    static NoteModelList noteModelList;
    static CategoryModelList categoryModelList;
    static TagModelList tagModelList;

    static ConfigModel *configModel;
    static GitManager *gitManager;
    static ThemeManager *themeManager;

};

#endif // GLOBALS_H
