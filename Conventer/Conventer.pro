QT += quick
QT += testlib
CONFIG += qt warn_on
CONFIG += c++17

INCLUDEPATH += C:/msys64/mingw64/include/poppler/qt5
LIBS += -LC:/msys64/mingw64/lib -lpoppler-qt5
INCLUDEPATH += C:/msys64/mingw64/include
LIBS += -LC:/msys64/mingw64/lib -lqpdf


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        AppController.cpp \
        ConvertDocToPdfCommand.cpp \
        ConvertImageToPdfCommand.cpp \
        ConverterFacade.cpp \
        ExtractPdfPageCommand.cpp \
        ImageService.cpp \
        JobQueue.cpp \
        LibreOfficeDocEngine.cpp \
        MergeFilesCommand.cpp \
        PdfEngine_Poppler.cpp \
        PdfWriter.cpp \
        main.cpp \
        tests/ConvertImageToPdfCommandTest.cpp \
        tests/ImageServiceTest.cpp \
        tests/PdfWriterTest.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AppController.h \
    ConvertDocToPdfCommand.h \
    ConvertImageToPdfCommand.h \
    ConverterFacade.h \
    ExtractPdfPageCommand.h \
    IConverterCommand.h \
    IDocEngine.h \
    IPdfEngine.h \
    ImageService.h \
    JobQueue.h \
    LibreOfficeDocEngine.h \
    MergeFilesCommand.h \
    PdfEngine_Poppler.h \
    PdfWriter.h \
    PdfWriterTest.h \
    tests/ConvertImageToPdfCommandTest.h \
    tests/ConverterFacadeJobTest.h \
    tests/ConverterFacadeTest.h \
    tests/ExtractPdfPageCommandTest.h \
    tests/ImageServiceTest.h \
    tests/LibreOfficeDocEngine_ConverterTest.h \
    tests/MergeFilesCommand_SpecificTest.h \
    tests/PdfEngine_Poppler_MergeSpecificTest.h \
    tests/PdfEngine_Poppler_RenderSpecificFileTest.h \
    tests/PdfWriterTest.h \
    tests/PdfWriter_SpecificImagesTest.h
