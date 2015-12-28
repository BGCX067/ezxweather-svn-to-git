#include "weather.h"

int main( int argc, char** argv )
{
    printf("EZXweather by www.dnxh.cn %s, %s \n",__TIME__,__DATE__);
    QString APPDIR(argv[0]);
    APPDIR.replace(strlen(argv[0])-7,7,"");
    QDir::setCurrent(APPDIR);
    putenv("TZ=GMT-00:00");

    ZApplication app( argc, argv );
    app.setDefaultCodec( QTextCodec::codecForName("GBK") );

    // translation file for application strings
    QTranslator myapp( 0 );
    myapp.load(ZLanguage::getSystemLanguageCode(), "." );
    app.installTranslator( &myapp );

    Weather *client = new Weather(0);
    client->show();

    QLabel *logo = new QLabel(0, "logo");
    logo->setBackgroundPixmap(QPixmap("Logo.jpg"));
    //logo->setFont(QFont::QFont("", 12, 75));
    logo->setPalette( QPalette( QColor( 0, 0, 0) ) );
    QLabel *vertext = new QLabel(logo,"ver");
    //vertext->setTextFormat(Qt::RichText);
    vertext->setText(QString("%1 %2 %3\n(c)2008 www.dnxh.cn").arg("Ver1.0").arg(__TIME__).arg(__DATE__));
    vertext->setFont(QFont::QFont("", 12, 75));
    vertext->setGeometry(0, 250, 240, 80);
    vertext->setBackgroundPixmap(QPixmap("Logo.jpg"));
    vertext->setBackgroundOrigin(QWidget::ParentOrigin);
    vertext->setAlignment(Qt::AlignCenter);
    logo->showFullScreen();
    //app.setMainWidget( client );
    return(app.exec());
}

