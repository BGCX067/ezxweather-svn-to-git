#ifndef _WEATHER_H_
#define _WEATHER_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <ZApplication.h>
#include <ZDataSessionManager.h>
#include <ZMessageBox.h>
#include <ZMainWidget.h>
#include <ZScrollView.h>
#include <ZPushButton.h>
#include <ZLanguage.h>
#include <UTIL_CST.h>
#include <UTIL_Language.h>
#include <UTIL_ProgressBar.h>
#include <SETUP_SET_Manager.h>
#include <qwallpaper.h>

#include <qtranslator.h> 
#include <qcolor.h>
#include <qsocket.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qtextview.h>
#include <qtextcodec.h>
#include <qiconview.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qdom.h>
#include <qfont.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qdir.h>
#include <qcheckbox.h>
#include <qstringlist.h>
#include <qdatetime.h>

#define DEBUG_PRINT 1
#define dbg_printf(fmt,args...) \
    ((void) ({ \
     if (DEBUG_PRINT) \
        printf(fmt,## args); \
     0; \
    }))

class Weather : public ZMainWidget
{
    Q_OBJECT
    public:
        Weather( QWidget * parent );
        ~Weather();

    protected:
        bool qwsEvent(QWSEvent* e);
        QString getnodetext(QDomElement Element,const char*nodename, int i);
        void insertData();
        int parsResponse();
        QString NAPIerr(int t);
        QString c2s(QString c);
        QString f2c(QString f);
        void setGPRSmode(QString pro);
        QPixmap alphaProcess(QString aName,QColor bgcolor);

    private slots:
        void togcity(int id);
        void recentslots(int id);
        void tosearch();
        void searchcity();
        void tocancel();
        void setUnits();
        void ReProfile();
        void update();
        void slotLinkConnected(int linkid, ZLinkInfo&);
        void socketHostFound();
        void closeConnection();
        void sendToServer();
        void socketReadyRead();
        void socketConnected();
        void socketConnectionClosed();
        void socketClosed();
        void socketError( int e );
        void switchDN(QIconViewItem *item);
        void about();

    private:
        QSocket *socket;
        ZPushButton *search,*cancel;
        QLineEdit *inputText;
        ZDataSessionManager *conn;
        QString profilename,units,currentCity,lsup,ds,cs;
        QString xmlStr,GetData;
        ZPushButton* Title;
        QLabel *cchmidT,*ccflikT,*ccuviT,*ccvisT,*ccwindT;
        QLabel *cc,*ccT,*ccicon,*cctmpT;
        ZScrollView *sv;
        QIconView *iv;
        SETUP_SET_Manager *mgr, *mgrcity;
        QCheckBox *SaveProfile;
        QPopupMenu *menuPop;
        int linkId,rc,proID,unitID,searchID,progress;
        bool flag,isWap,wapFlag,saveprofilename,isfive;
        QString SendTypes,code;
        QStringList seachlist,updatelist;
        QPopupMenu *scitymenu ,*recentmenu;
        UTIL_ProgressBar *pbr;
        QDomNodeList *daylist;
};

#endif
