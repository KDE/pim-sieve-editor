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
#include <KWindowSystem>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWindow>
#include <kdsingleapplication.h>
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
#include <private/qtx11extras_p.h>
#endif
#endif

#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <QTextStream>
#if WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/Provider>
#endif

#include <KIconTheme>

#include <KStyleManager>

int main(int argc, char **argv)
{
    KIconTheme::initTheme();
    QApplication app(argc, argv);
    app.setDesktopFileName(QStringLiteral("org.kde.sieveeditor"));
#if !WITH_DBUS
    KDSingleApplication sapp;
#endif
    KStyleManager::initStyle();
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
#if WITH_KUSERFEEDBACK
    const QCommandLineOption feedbackOption(QStringLiteral("feedback"), i18nc("@info:shell", "Lists the available options for user feedback"));
    parser.addOption(feedbackOption);
#endif
    parser.process(app);
    aboutData.processCommandLine(&parser);

#if WITH_KUSERFEEDBACK
    if (parser.isSet(feedbackOption)) {
        QTextStream(stdout) << UserFeedBackManager::self()->userFeedbackProvider()->describeDataSources() << '\n';
        return 0;
    }
#endif

#if WITH_DBUS
    KDBusService service(KDBusService::Unique);
#else
    if (!sapp.isPrimaryInstance()) {
        QJsonDocument doc;

        QJsonObject obj;
        obj[QLatin1String("working_dir")] = QDir::currentPath();
        obj[QLatin1String("args")] = QJsonArray::fromStringList(app.arguments());
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
        if (KWindowSystem::isPlatformWayland()) {
            obj[QLatin1String("xdg_activation_token")] = qEnvironmentVariable("XDG_ACTIVATION_TOKEN");
        } else if (KWindowSystem::isPlatformX11()) {
            obj[QLatin1String("startup_id")] = QString::fromUtf8(QX11Info::nextStartupId());
        }
#endif

        doc.setObject(obj);

        sapp.sendMessage(doc.toJson(QJsonDocument::Compact));
        return 0;
    }
#endif
    auto mw = new SieveEditorMainWindow();
#if WITH_DBUS
    QObject::connect(&service, &KDBusService::activateRequested, mw, &SieveEditorMainWindow::slotActivateRequested);
#else
    QApplication::connect(&sapp, &KDSingleApplication::messageReceived, &app, [mw](const QByteArray &messageData) {
        QJsonDocument doc = QJsonDocument::fromJson(messageData);
        QJsonObject message = doc.object();

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
        if (KWindowSystem::isPlatformWayland()) {
            qputenv("XDG_ACTIVATION_TOKEN", message[QLatin1String("xdg_activation_token")].toString().toUtf8());
        } else if (KWindowSystem::isPlatformX11()) {
            QX11Info::setNextStartupId(message[QLatin1String("startup_id")].toString().toUtf8());
        }
#endif
        QStringList arguments;

        const auto argumentsJson = message[QLatin1String("args")].toArray();
        for (const QJsonValue &val : argumentsJson) {
            arguments << val.toString();
        }

        mw->slotActivateRequested(arguments, message[QLatin1String("working_dir")].toString());
    });
#endif

    mw->show();
    const int val = app.exec();
    return val;
}
