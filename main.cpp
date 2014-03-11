#include <QApplication>
#include <QTextCodec> 
#include <QTranslator>
#include <QString>

#include "DesktopSprite.h"
#include "Configuration.h"

/*
 * a launcher for the sprite program
 */
int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    // locale display
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);  
    QTranslator translator;
    translator.load("QtSprite_zh_CN");
    app.installTranslator(&translator);

    // configuration
    Configuration configs;
    configs.init();

    DesktopSprite sprite(&app, &configs);
    sprite.show();
    return app.exec();
}
