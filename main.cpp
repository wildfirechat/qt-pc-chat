#ifdef __unix__
#include <gtk/gtk.h>
#endif // __unix__
#include "src/pages/login/loginwidget.h"
#include "singleinstance.h"
#include <QMetaEnum>
#include <QTranslator>
#include <QFontDatabase>
#include <QFile>
#include "src/vendor/zenshot/starterui.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    QApplication a(argc, argv);

#ifdef Q_OS_LINUX
    gdk_init(NULL, NULL);
#endif // Q_OS_LINUX

    //加载并应用语言翻译界面
    QLocale locale;
    QLocale::Language langEnum = locale.language();
    QMetaEnum metaEnum = QMetaEnum::fromType<QLocale::Language>();
    const char* langChar = metaEnum.valueToKey(langEnum);
    QString langName(langChar);

    QString langFile = ":/translations/" + langName + ".qm";
    if(QFile(langFile).exists() == false)
    {
        langFile = ":/translations/English.qm";
    }

    QTranslator translator;
    translator.load(langFile);
    a.installTranslator(&translator);

    QString lang_country = QLocale::system().name();
    QTranslator qt_ts;
    if(qt_ts.load(QString(":/qt_%1").arg(lang_country)))
        a.installTranslator( &qt_ts );
    else if(qt_ts.load(QString("lang/qt_%1").arg(lang_country)))
        a.installTranslator( &qt_ts );
    QTranslator app_ts;
    if(app_ts.load(QString(":/EzViewer_%1").arg(lang_country)))
        a.installTranslator( &app_ts );
    else if(app_ts.load(QString("lang/EzViewer_%1").arg(lang_country)))
        a.installTranslator( &app_ts );

    //加载图标字体文件
    int fontId = QFontDatabase::addApplicationFont(":/iconfont.ttf");
    QString fontNmae = QFontDatabase::applicationFontFamilies(fontId).at(0);

    //设置系统默认字体
    QFont defaultFont("微软雅黑",12);
    a.setFont(defaultFont);

    //初始化截屏
    StarterUI::InitMetaType();

    // 单例应用
    SingleInstance singleInstance(&a);

    // 设置应用程序样式
    a.setStyle("Fusion");

    LoginWidget w;
    w.show();

    return a.exec();
}
