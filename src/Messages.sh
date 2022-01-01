#! /bin/sh
# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org> 
$EXTRACTRC `find . -name '*.ui' -or -name '*.rc'` >> rc.cpp || exit 11
$XGETTEXT `find . -name '*.cpp' -o -name '*.h' | grep -v '/autotests/'` -o $podir/sieveeditor.pot
rm -f rc.cpp
