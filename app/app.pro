TEMPLATE=subdirs
SUBDIRS=src
CONFIG+=ordered

OTHER_FILES += infiles/configMD.cfg

DIRS = outfiles

for(DIR, DIRS) {
     mkcommands += $$OUT_PWD/$$DIR
}

!equals(PWD, $${OUT_PWD}) {

	copydata.commands = $(COPY_DIR) $$PWD/infiles $$OUT_PWD

}

createDirs.commands = $(MKDIR) $$mkcommands

first.depends = $(first) copydata createDirs

export(first.depends)

QMAKE_EXTRA_TARGETS += first copydata createDirs
