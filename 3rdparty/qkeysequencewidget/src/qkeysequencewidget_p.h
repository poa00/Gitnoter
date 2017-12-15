/******************************************************************************
Copyright (c) 2010, Artem Galichkin <doomer3d@gmail.com>
All rights reserved.

BSD License Usage

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************

GNU General Public License Usage

Alternatively, this file may be used under the terms of the GNU
General Public License version 3.0 as published by the Free Software
Foundation and appearing in the file LICENSE.GPL included in the
packaging of this file. Please review the following information to
ensure the GNU General Public License version 3.0 requirements will be
met: http://www.gnu.org/copyleft/gpl.html.

*******************************************************************************/

#ifndef QKEYSEQUENCEWIDGET_P_H
#define QKEYSEQUENCEWIDGET_P_H

#include <QKeySequence>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QTimer>
#include <QDebug>
#include <QIcon>

#include "qkeysequencewidget.h"

class QShortcutButton;
class QKeySequenceWidget;

class QKeySequenceWidgetPrivate // : public QObject
{
    //Q_OBJECT
    Q_DECLARE_PUBLIC(QKeySequenceWidget);

public:
    QKeySequenceWidget * q_ptr;

    QKeySequenceWidgetPrivate();
    virtual ~QKeySequenceWidgetPrivate();

    void init(const QKeySequence keySeq, const QString noneStr);
    void updateView();

    void startRecording();
    void doneRecording();
    inline void cancelRecording();
    inline void clearSequence();
    inline void controlModifierlessTimout();
    inline void keyNotSupported();

    void updateDisplayShortcut();

    // members
    QKeySequence currentSequence;
    QKeySequence oldSequence;
    QKeySequence defaultSequence;
    QString noneSequenceText;

    QTimer modifierlessTimeout;

    quint32 numKey;
    quint32 modifierKeys;

    void setToolTip(const QString &shortcutButtonText = QString(""),
                    const QString &clearButtonText = QString(""));

    QHBoxLayout *layout;
    QToolButton *clearButton;
    QShortcutButton *shortcutButton;
    QColor shortcutButtonActiveColor;
    QColor shortcutButtonInactiveColor;

    int showClearButton;

    bool isRecording;

    void updateShortcutButtonColor();
};

class QShortcutButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QShortcutButton(QKeySequenceWidgetPrivate *p, QWidget *parent = 0)
        :  QPushButton(parent)
        , d(p)
    {
        setMinimumWidth(QPushButton::minimumWidth());
        QPushButton::setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    }

    virtual ~QShortcutButton()
    {

    }

    virtual QSize sizeHint() const;

protected:
    // Reimplemented for internal reasons.
    virtual bool event(QEvent *e);
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void keyReleaseEvent(QKeyEvent *keyEvent);

private:
    QKeySequenceWidgetPrivate * const d;
};

#endif // QKEYSEQUENCEWIDGET_P_H
