/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorpagewidget.h"
#include "sieveeditorglobalconfig.h"
#include <KSieveCore/CheckScriptJob>

#include <KManageSieve/SieveJob>

#include <KLocalizedString>
#include <KMessageBox>

#include "sieveeditor_debug.h"
#include <QVBoxLayout>

SieveEditorPageWidget::SieveEditorPageWidget(QWidget *parent)
    : QWidget(parent)
{
    auto vbox = new QVBoxLayout(this);
    mSieveEditorWidget = new KSieveUi::SieveEditorWidget(false);
    connect(mSieveEditorWidget, &KSieveUi::SieveEditorWidget::valueChanged, this, &SieveEditorPageWidget::slotValueChanged);
    vbox->addWidget(mSieveEditorWidget);
    connect(mSieveEditorWidget, &KSieveUi::SieveEditorWidget::checkSyntax, this, &SieveEditorPageWidget::slotCheckSyntaxClicked);
    connect(mSieveEditorWidget, &KSieveUi::SieveEditorWidget::modeEditorChanged, this, &SieveEditorPageWidget::modeEditorChanged);
    connect(mSieveEditorWidget, &KSieveUi::SieveEditorWidget::undoAvailable, this, &SieveEditorPageWidget::undoAvailable);
    connect(mSieveEditorWidget, &KSieveUi::SieveEditorWidget::redoAvailable, this, &SieveEditorPageWidget::redoAvailable);
    connect(mSieveEditorWidget, &KSieveUi::SieveEditorWidget::copyAvailable, this, &SieveEditorPageWidget::copyAvailable);
    connect(mSieveEditorWidget, &KSieveUi::SieveEditorWidget::sieveEditorTabCurrentChanged, this, &SieveEditorPageWidget::sieveEditorTabCurrentChanged);
}

SieveEditorPageWidget::~SieveEditorPageWidget() = default;

void SieveEditorPageWidget::slotCheckSyntaxClicked()
{
    const QString script = mSieveEditorWidget->script();
    if (script.isEmpty()) {
        return;
    }
    mSieveEditorWidget->addNormalMessage(i18n("Uploading script to server for checking it, please wait..."));

    auto checkScriptJob = new KSieveCore::CheckScriptJob(this);
    connect(checkScriptJob, &KSieveCore::CheckScriptJob::finished, this, &SieveEditorPageWidget::slotCheckScriptJobFinished);
    checkScriptJob->setUrl(mCurrentURL);
    checkScriptJob->setIsActive(mWasActive);
    checkScriptJob->setCurrentScript(script);
    checkScriptJob->setOriginalScript(mSieveEditorWidget->originalScript());
    checkScriptJob->start();
}

void SieveEditorPageWidget::slotCheckScriptJobFinished(const QString &errorMsg, bool success)
{
    if (success) {
        mSieveEditorWidget->addOkMessage(errorMsg);
    } else {
        mSieveEditorWidget->addFailedMessage(errorMsg);
    }
    mSieveEditorWidget->resultDone();
}

void SieveEditorPageWidget::setIsNewScript(bool isNewScript)
{
    mIsNewScript = isNewScript;
}

void SieveEditorPageWidget::loadScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info)
{
    mCurrentURL = info.currentUrl;
    mSieveEditorWidget->setSieveCapabilities(info.currentCapabilities);
    mSieveEditorWidget->setSieveImapAccountSettings(info.sieveImapAccountSettings);
    mSieveEditorWidget->setListOfIncludeFile(info.scriptList);
    mSieveEditorWidget->setReadOnly(true);
    mSieveEditorWidget->setWordWrap(SieveEditorGlobalConfig::self()->wrapText());
    KManageSieve::SieveJob *job = KManageSieve::SieveJob::get(mCurrentURL);
    connect(job, &KManageSieve::SieveJob::result, this, &SieveEditorPageWidget::slotGetResult);
}

QUrl SieveEditorPageWidget::currentUrl() const
{
    return mCurrentURL;
}

void SieveEditorPageWidget::renameScriptName(const QString &newName)
{
    mSieveEditorWidget->setScriptName(newName);
}

void SieveEditorPageWidget::setCurrentUrl(const QUrl &url)
{
    mCurrentURL = url;
}

void SieveEditorPageWidget::slotGetResult(KManageSieve::SieveJob *, bool success, const QString &script, bool isActive)
{
    mSieveEditorWidget->setReadOnly(false);
    if (!success) {
        return;
    }
    mSieveEditorWidget->setScriptName(mCurrentURL.fileName());
    mSieveEditorWidget->setScript(script, true);
    mWasActive = isActive;
    mSieveEditorWidget->setModified(false);
}

bool SieveEditorPageWidget::uploadScriptAndCloseTab(int index)
{
    if (mSieveEditorWidget->isModified()) {
        KManageSieve::SieveJob *job = KManageSieve::SieveJob::put(mCurrentURL, mSieveEditorWidget->script(), mWasActive, mWasActive);
        job->setProperty("showuploadinformation", true);
        job->setProperty("tabIndex", index);
        connect(job, &KManageSieve::SieveJob::result, this, &SieveEditorPageWidget::slotPutResult);
        return true;
    }
    return false;
}

void SieveEditorPageWidget::uploadScript(bool showInformation, bool forceSave)
{
    if (mSieveEditorWidget->isModified() || forceSave) {
        KManageSieve::SieveJob *job = KManageSieve::SieveJob::put(mCurrentURL, mSieveEditorWidget->script(), mWasActive, mWasActive);
        job->setProperty("showuploadinformation", showInformation);
        connect(job, &KManageSieve::SieveJob::result, this, &SieveEditorPageWidget::slotPutResult);
    }
}

void SieveEditorPageWidget::slotPutResult(KManageSieve::SieveJob *job, bool success)
{
    if (mIsNewScript) {
        Q_EMIT refreshList();
    }
    if (success) {
        if (job->property("showuploadinformation").toBool()) {
            KMessageBox::information(this, i18n("The Sieve script was successfully uploaded."), i18n("Sieve Script Upload"));
        }
        mIsNewScript = false;
        mSieveEditorWidget->updateOriginalScript();
        mSieveEditorWidget->setModified(false);
        if (job->property("tabIndex").isValid()) {
            const int index = job->property("tabIndex").toInt();
            Q_EMIT requestCloseTab(index);
        }
    } else {
        const QString msg = job->errorString();
        if (!msg.isEmpty()) {
            KMessageBox::error(this,
                               i18n("Uploading the Sieve script failed.\n"
                                    "The server responded:\n%1",
                                    msg),
                               i18n("Sieve Error"));
        } else {
            KMessageBox::error(this,
                               i18n("Uploading the Sieve script failed.\n"
                                    "unknown reason\n"),
                               i18n("Sieve Error"));
        }
    }
}

bool SieveEditorPageWidget::needToSaveScript()
{
    bool result = false;
    if (mIsNewScript) {
        const int resultQuestion = KMessageBox::warningTwoActionsCancel(this,
                                                                        i18n("Script '%1' is new. Do you want to save it?", mCurrentURL.fileName()),
                                                                        i18n("Save Script"),
                                                                        KStandardGuiItem::save(),
                                                                        KStandardGuiItem::discard());
        if (resultQuestion == KMessageBox::ButtonCode::PrimaryAction) {
            uploadScript();
            result = true;
        } else if (resultQuestion == KMessageBox::Cancel) {
            result = true;
        }
    } else {
        if (mSieveEditorWidget->isModified()) {
            const int resultQuestion = KMessageBox::warningTwoActionsCancel(this,
                                                                            i18n("Script '%1' was changed. Do you want to save it ?", mCurrentURL.fileName()),
                                                                            i18n("Save Script"),
                                                                            KStandardGuiItem::save(),
                                                                            KStandardGuiItem::discard());
            if (resultQuestion == KMessageBox::ButtonCode::PrimaryAction) {
                uploadScript();
                result = true;
            } else if (resultQuestion == KMessageBox::Cancel) {
                result = true;
            }
        }
    }
    return result;
}

void SieveEditorPageWidget::slotValueChanged(bool b)
{
    Q_EMIT scriptModified(b, this);
}

bool SieveEditorPageWidget::isModified() const
{
    return mSieveEditorWidget->isModified();
}

void SieveEditorPageWidget::goToLine()
{
    mSieveEditorWidget->goToLine();
}

void SieveEditorPageWidget::undo()
{
    mSieveEditorWidget->undo();
}

bool SieveEditorPageWidget::isUndoAvailable() const
{
    return mSieveEditorWidget->isUndoAvailable();
}

bool SieveEditorPageWidget::isRedoAvailable() const
{
    return mSieveEditorWidget->isRedoAvailable();
}

bool SieveEditorPageWidget::hasSelection() const
{
    return mSieveEditorWidget->hasSelection();
}

void SieveEditorPageWidget::redo()
{
    mSieveEditorWidget->redo();
}

void SieveEditorPageWidget::find()
{
    mSieveEditorWidget->find();
}

void SieveEditorPageWidget::replace()
{
    mSieveEditorWidget->replace();
}

void SieveEditorPageWidget::import()
{
    mSieveEditorWidget->slotImport();
}

void SieveEditorPageWidget::comment()
{
    mSieveEditorWidget->comment();
}

void SieveEditorPageWidget::uncomment()
{
    mSieveEditorWidget->uncomment();
}

void SieveEditorPageWidget::checkSpelling()
{
    mSieveEditorWidget->checkSpelling();
}

void SieveEditorPageWidget::createRulesGraphically()
{
    mSieveEditorWidget->slotCreateRulesGraphically();
}

void SieveEditorPageWidget::checkSyntax()
{
    mSieveEditorWidget->slotCheckSyntax();
}

void SieveEditorPageWidget::saveAs()
{
    mSieveEditorWidget->slotSaveAs();
}

void SieveEditorPageWidget::reverseCase()
{
    mSieveEditorWidget->reverseCase();
}

void SieveEditorPageWidget::lowerCase()
{
    mSieveEditorWidget->lowerCase();
}

void SieveEditorPageWidget::debugSieveScript()
{
    mSieveEditorWidget->debugSieveScript();
}

void SieveEditorPageWidget::upperCase()
{
    mSieveEditorWidget->upperCase();
}

void SieveEditorPageWidget::sentenceCase()
{
    mSieveEditorWidget->sentenceCase();
}

KSieveUi::SieveEditorWidget::EditorMode SieveEditorPageWidget::pageMode() const
{
    return mSieveEditorWidget->mode();
}

void SieveEditorPageWidget::paste()
{
    mSieveEditorWidget->paste();
}

void SieveEditorPageWidget::cut()
{
    mSieveEditorWidget->cut();
}

void SieveEditorPageWidget::copy()
{
    mSieveEditorWidget->copy();
}

void SieveEditorPageWidget::selectAll()
{
    mSieveEditorWidget->selectAll();
}

void SieveEditorPageWidget::zoomIn()
{
    mSieveEditorWidget->zoomIn();
}

void SieveEditorPageWidget::zoomOut()
{
    mSieveEditorWidget->zoomOut();
}

QString SieveEditorPageWidget::currentText() const
{
    return mSieveEditorWidget->script();
}

void SieveEditorPageWidget::setWordWrap(bool state)
{
    mSieveEditorWidget->setWordWrap(state);
}

bool SieveEditorPageWidget::isWordWrap() const
{
    return mSieveEditorWidget->isWordWrap();
}

void SieveEditorPageWidget::print()
{
    mSieveEditorWidget->print();
}

void SieveEditorPageWidget::printPreview()
{
    mSieveEditorWidget->printPreview();
}

bool SieveEditorPageWidget::printSupportEnabled() const
{
    return mSieveEditorWidget->printSupportEnabled();
}

bool SieveEditorPageWidget::isTextEditor() const
{
    return mSieveEditorWidget->isTextEditor();
}

void SieveEditorPageWidget::zoomReset()
{
    mSieveEditorWidget->zoomReset();
}

void SieveEditorPageWidget::openBookmarkUrl(const QUrl &url)
{
    mSieveEditorWidget->openBookmarkUrl(url);
}

QString SieveEditorPageWidget::currentHelpTitle() const
{
    return mSieveEditorWidget->currentHelpTitle();
}

QUrl SieveEditorPageWidget::currentHelpUrl() const
{
    return mSieveEditorWidget->currentHelpUrl();
}

#include "moc_sieveeditorpagewidget.cpp"
