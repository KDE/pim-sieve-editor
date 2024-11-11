/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "config-pim-sieve-editor.h"
#include "sieveeditor-version.h"
#include "sieveeditormainwindow.h"
#include <KAboutData>
#include <KCrash>
#if WITH_DBUS
#include <KDBusService>
#else
#include "3rdparty/SingleApplication/SingleApplication"
#endif

#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <QTextStream>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/Provider>
#endif

#define HAVE_KICONTHEME __has_include(<KIconTheme>)
#if HAVE_KICONTHEME
#include <KIconTheme>
#endif

#define HAVE_STYLE_MANAGER __has_include(<KStyleManager>)
#if HAVE_STYLE_MANAGER
#include <KStyleManager>
#endif

int main(int argc, char **argv)
{
#if HAVE_KICONTHEME
    KIconTheme::initTheme();
#endif
#if WITH_DBUS
    QApplication app(argc, argv);
    app.setDesktopFileName(QStringLiteral("org.kde.sieveeditor"));
#else
    SingleApplication app(argc, argv, true);
#endif
#if HAVE_STYLE_MANAGER
    KStyleManager::initStyle();
#else // !HAVE_STYLE_MANAGER
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif // defined(Q_OS_MACOS) || defined(Q_OS_WIN)
#endif // HAVE_STYLE_MANAGER
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
    KCrash::initialize();

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
#ifdef WITH_KUSERFEEDBACK
    const QCommandLineOption feedbackOption(QStringLiteral("feedback"), i18nc("@info:shell", "Lists the available options for user feedback"));
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

#if WITH_DBUS
    KDBusService service(KDBusService::Unique);
#else
    if (!app.isPrimary()) {
        return 0;
    }
#endif

    auto mw = new SieveEditorMainWindow();
    mw->show();
    const int val = app.exec();
    return val;
}
