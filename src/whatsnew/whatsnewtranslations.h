/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "config-pim-sieve-editor.h"
#include "libsieveeditor_export.h"
#include <TextAddonsWidgets/WhatsNewTranslationsBase>

class LIBSIEVEEDITOR_EXPORT WhatsNewTranslations : public TextAddonsWidgets::WhatsNewTranslationsBase
{
public:
    WhatsNewTranslations();
    ~WhatsNewTranslations() override;

    [[nodiscard]] QList<TextAddonsWidgets::WhatsNewInfo> createWhatsNewInfo() const override;
    [[nodiscard]] QList<KLazyLocalizedString> lastNewFeatures() const override;
};
