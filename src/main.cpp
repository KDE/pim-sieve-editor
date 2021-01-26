/*
   Copyright (C) 2013-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "sieveeditor-version.h"
#include "sieveeditormainwindow.h"
#include <KAboutData>
#include <KCrash>
#include <KDBusService>
#include <KLocalizedString>
#include <Kdelibs4ConfigMigrator>
#include <QApplication>
#include <QCommandLineParser>
#include <QTextStream>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/FeedbackConfigWidget>
#include <KUserFeedback/Provider>
#endif

int main(int argc, char **argv)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication app(argc, argv);
    app.setDesktopFileName(QStringLiteral("org.kde.sieveeditor"));
    KCrash::initialize();
    Kdelibs4ConfigMigrator migrate(QStringLiteral("sieveeditor"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("sieveeditorrc") << QStringLiteral("sievetemplaterc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("sieveeditorui.rc"));
    migrate.migrate();

    KLocalizedString::setApplicationDomain("sieveeditor");

    KAboutData aboutData(QStringLiteral("sieveeditor"),
                         i18n("KSieve Editor"),
                         QStringLiteral(SIEVEEDITOR_VERSION),
                         i18n("Sieve Editor"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2013-2021 sieveeditor authors"));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));

    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kmail")));
    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
#ifdef WITH_KUSERFEEDBACK
    parser.addOption(QCommandLineOption(QStringLiteral("feedback"), i18n("Lists the available options for user feedback")));
#endif
    parser.process(app);
    aboutData.processCommandLine(&parser);

#ifdef WITH_KUSERFEEDBACK
    if (parser.isSet(QStringLiteral("feedback"))) {
        QTextStream(stdout) << UserFeedBackManager::self()->userFeedbackProvider()->describeDataSources() << '\n';
        return 0;
    }
#endif

    KDBusService service(KDBusService::Unique);

    auto mw = new SieveEditorMainWindow();
    mw->show();
    const int val = app.exec();
    return val;
}
