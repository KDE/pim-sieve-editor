# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
########### install files ###############

file(GLOB sieveeditor_icons "*-apps-sieveeditor.*")

ecm_install_icons(ICONS ${sieveeditor_icons}
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)
