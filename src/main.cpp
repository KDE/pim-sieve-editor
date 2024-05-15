/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditor-version.h"
#include "sieveeditormainwindow.h"
#include <KAboutData>
#include <KCrash>
#include <KDBusService>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <QTextStream>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/Provider>
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setDesktopFileName(QStringLiteral("org.kde.sieveeditor"));
    KCrash::initialize();

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("sieveeditor"));

    KAboutData aboutData(QStringLiteral("sieveeditor"),
                         i18n("KSieve Editor"),
                         QStringLiteral(SIEVEEDITOR_VERSION),
                         i18n("Sieve Editor"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2013-%1 sieveeditor authors", QStringLiteral("2024")));
    aboutData.addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));

    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("sieveeditor")));
    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
#ifdef WITH_KUSERFEEDBACK
    const QCommandLineOption feedbackOption(QStringLiteral("feedback"), i18n("Lists the available options for user feedback"));
    parser.addOption(feedbackOption);
#endif
    parser.process(app);
    aboutData.processCommandLine(&parser);

#ifdef WITH_KUSERFEEDBACK
    if (parser.isSet(feedbackOption)) {
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
