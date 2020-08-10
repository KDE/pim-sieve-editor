/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2007 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef LIBSIEVEEDITORPRIVATE_EXPORT_H
#define LIBSIEVEEDITORPRIVATE_EXPORT_H

#include "libsieveeditor_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
# ifndef LIBSIEVEEDITOR_TESTS_EXPORT
#  define LIBSIEVEEDITOR_TESTS_EXPORT LIBSIEVEEDITOR_EXPORT
# endif
#else /* not compiling tests */
# define LIBSIEVEEDITOR_TESTS_EXPORT
#endif

#endif
