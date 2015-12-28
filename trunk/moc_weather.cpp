/****************************************************************************
** Weather meta object code from reading C++ file 'weather.h'
**
** Created: Thu Feb 5 23:39:39 2009
**      by: The Qt MOC ($Id: qt/src/moc/moc.y   2.3.8   edited 2004-08-05 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "weather.h"
#include <qmetaobject.h>
#include <qapplication.h>
#ifdef QWS
#include <qobjectdict.h>
#endif



const char *Weather::className() const
{
    return "Weather";
}

QMetaObject *Weather::metaObj = 0;

#ifdef QWS
static class Weather_metaObj_Unloader {
public:
    ~Weather_metaObj_Unloader()
    {
         if ( objectDict )
             objectDict->remove( "Weather" );
    }
} Weather_metaObj_unloader;
#endif

void Weather::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(ZMainWidget::className(), "ZMainWidget") != 0 )
	badSuperclassWarning("Weather","ZMainWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString Weather::tr(const char* s)
{
    return qApp->translate( "Weather", s, 0 );
}

QString Weather::tr(const char* s, const char * c)
{
    return qApp->translate( "Weather", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* Weather::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) ZMainWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void (Weather::*m1_t0)(int);
    typedef void (QObject::*om1_t0)(int);
    typedef void (Weather::*m1_t1)(int);
    typedef void (QObject::*om1_t1)(int);
    typedef void (Weather::*m1_t2)();
    typedef void (QObject::*om1_t2)();
    typedef void (Weather::*m1_t3)();
    typedef void (QObject::*om1_t3)();
    typedef void (Weather::*m1_t4)();
    typedef void (QObject::*om1_t4)();
    typedef void (Weather::*m1_t5)();
    typedef void (QObject::*om1_t5)();
    typedef void (Weather::*m1_t6)();
    typedef void (QObject::*om1_t6)();
    typedef void (Weather::*m1_t7)();
    typedef void (QObject::*om1_t7)();
    typedef void (Weather::*m1_t8)(int,ZLinkInfo&);
    typedef void (QObject::*om1_t8)(int,ZLinkInfo&);
    typedef void (Weather::*m1_t9)();
    typedef void (QObject::*om1_t9)();
    typedef void (Weather::*m1_t10)();
    typedef void (QObject::*om1_t10)();
    typedef void (Weather::*m1_t11)();
    typedef void (QObject::*om1_t11)();
    typedef void (Weather::*m1_t12)();
    typedef void (QObject::*om1_t12)();
    typedef void (Weather::*m1_t13)();
    typedef void (QObject::*om1_t13)();
    typedef void (Weather::*m1_t14)();
    typedef void (QObject::*om1_t14)();
    typedef void (Weather::*m1_t15)();
    typedef void (QObject::*om1_t15)();
    typedef void (Weather::*m1_t16)(int);
    typedef void (QObject::*om1_t16)(int);
    typedef void (Weather::*m1_t17)(QIconViewItem*);
    typedef void (QObject::*om1_t17)(QIconViewItem*);
    typedef void (Weather::*m1_t18)();
    typedef void (QObject::*om1_t18)();
    m1_t0 v1_0 = &Weather::togcity;
    om1_t0 ov1_0 = (om1_t0)v1_0;
    m1_t1 v1_1 = &Weather::recentslots;
    om1_t1 ov1_1 = (om1_t1)v1_1;
    m1_t2 v1_2 = &Weather::tosearch;
    om1_t2 ov1_2 = (om1_t2)v1_2;
    m1_t3 v1_3 = &Weather::searchcity;
    om1_t3 ov1_3 = (om1_t3)v1_3;
    m1_t4 v1_4 = &Weather::tocancel;
    om1_t4 ov1_4 = (om1_t4)v1_4;
    m1_t5 v1_5 = &Weather::setUnits;
    om1_t5 ov1_5 = (om1_t5)v1_5;
    m1_t6 v1_6 = &Weather::ReProfile;
    om1_t6 ov1_6 = (om1_t6)v1_6;
    m1_t7 v1_7 = &Weather::update;
    om1_t7 ov1_7 = (om1_t7)v1_7;
    m1_t8 v1_8 = &Weather::slotLinkConnected;
    om1_t8 ov1_8 = (om1_t8)v1_8;
    m1_t9 v1_9 = &Weather::socketHostFound;
    om1_t9 ov1_9 = (om1_t9)v1_9;
    m1_t10 v1_10 = &Weather::closeConnection;
    om1_t10 ov1_10 = (om1_t10)v1_10;
    m1_t11 v1_11 = &Weather::sendToServer;
    om1_t11 ov1_11 = (om1_t11)v1_11;
    m1_t12 v1_12 = &Weather::socketReadyRead;
    om1_t12 ov1_12 = (om1_t12)v1_12;
    m1_t13 v1_13 = &Weather::socketConnected;
    om1_t13 ov1_13 = (om1_t13)v1_13;
    m1_t14 v1_14 = &Weather::socketConnectionClosed;
    om1_t14 ov1_14 = (om1_t14)v1_14;
    m1_t15 v1_15 = &Weather::socketClosed;
    om1_t15 ov1_15 = (om1_t15)v1_15;
    m1_t16 v1_16 = &Weather::socketError;
    om1_t16 ov1_16 = (om1_t16)v1_16;
    m1_t17 v1_17 = &Weather::switchDN;
    om1_t17 ov1_17 = (om1_t17)v1_17;
    m1_t18 v1_18 = &Weather::about;
    om1_t18 ov1_18 = (om1_t18)v1_18;
    QMetaData *slot_tbl = QMetaObject::new_metadata(19);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(19);
    slot_tbl[0].name = "togcity(int)";
    slot_tbl[0].ptr = (QMember)ov1_0;
    slot_tbl_access[0] = QMetaData::Private;
    slot_tbl[1].name = "recentslots(int)";
    slot_tbl[1].ptr = (QMember)ov1_1;
    slot_tbl_access[1] = QMetaData::Private;
    slot_tbl[2].name = "tosearch()";
    slot_tbl[2].ptr = (QMember)ov1_2;
    slot_tbl_access[2] = QMetaData::Private;
    slot_tbl[3].name = "searchcity()";
    slot_tbl[3].ptr = (QMember)ov1_3;
    slot_tbl_access[3] = QMetaData::Private;
    slot_tbl[4].name = "tocancel()";
    slot_tbl[4].ptr = (QMember)ov1_4;
    slot_tbl_access[4] = QMetaData::Private;
    slot_tbl[5].name = "setUnits()";
    slot_tbl[5].ptr = (QMember)ov1_5;
    slot_tbl_access[5] = QMetaData::Private;
    slot_tbl[6].name = "ReProfile()";
    slot_tbl[6].ptr = (QMember)ov1_6;
    slot_tbl_access[6] = QMetaData::Private;
    slot_tbl[7].name = "update()";
    slot_tbl[7].ptr = (QMember)ov1_7;
    slot_tbl_access[7] = QMetaData::Private;
    slot_tbl[8].name = "slotLinkConnected(int,ZLinkInfo&)";
    slot_tbl[8].ptr = (QMember)ov1_8;
    slot_tbl_access[8] = QMetaData::Private;
    slot_tbl[9].name = "socketHostFound()";
    slot_tbl[9].ptr = (QMember)ov1_9;
    slot_tbl_access[9] = QMetaData::Private;
    slot_tbl[10].name = "closeConnection()";
    slot_tbl[10].ptr = (QMember)ov1_10;
    slot_tbl_access[10] = QMetaData::Private;
    slot_tbl[11].name = "sendToServer()";
    slot_tbl[11].ptr = (QMember)ov1_11;
    slot_tbl_access[11] = QMetaData::Private;
    slot_tbl[12].name = "socketReadyRead()";
    slot_tbl[12].ptr = (QMember)ov1_12;
    slot_tbl_access[12] = QMetaData::Private;
    slot_tbl[13].name = "socketConnected()";
    slot_tbl[13].ptr = (QMember)ov1_13;
    slot_tbl_access[13] = QMetaData::Private;
    slot_tbl[14].name = "socketConnectionClosed()";
    slot_tbl[14].ptr = (QMember)ov1_14;
    slot_tbl_access[14] = QMetaData::Private;
    slot_tbl[15].name = "socketClosed()";
    slot_tbl[15].ptr = (QMember)ov1_15;
    slot_tbl_access[15] = QMetaData::Private;
    slot_tbl[16].name = "socketError(int)";
    slot_tbl[16].ptr = (QMember)ov1_16;
    slot_tbl_access[16] = QMetaData::Private;
    slot_tbl[17].name = "switchDN(QIconViewItem*)";
    slot_tbl[17].ptr = (QMember)ov1_17;
    slot_tbl_access[17] = QMetaData::Private;
    slot_tbl[18].name = "about()";
    slot_tbl[18].ptr = (QMember)ov1_18;
    slot_tbl_access[18] = QMetaData::Private;
    metaObj = QMetaObject::new_metaobject(
	"Weather", "ZMainWidget",
	slot_tbl, 19,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
