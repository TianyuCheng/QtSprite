#include <QApplication>
#include <QTextCodec> 
#include <QTranslator>
#include <QString>

#include "DesktopSprite.h"

/*
 * a launcher for the sprite program
 */
int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  
    QTextCodec::setCodecForLocale(codec);  
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForTr(codec);  

    QTranslator translator;
    translator.load("QtSprite_zh_CN");
    app.installTranslator(&translator);

    DesktopSprite sprite(&app);
    sprite.show();

    app.alert(&sprite);
    return app.exec();
}
