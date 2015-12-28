#include "weather.h"

Weather::Weather( QWidget * parent )
        :ZMainWidget( tr("weather"), true, parent,NULL,0)
{
    QColor bgcolor = QColor("#003399");
    //QPixmap bgpm;
    //bgpm.fill(bgcolor);
    //QWallpaper::setAppWallpaperMode(QWallpaper::UseAppWallpaper);
    //QWallpaper::setAppWallpaper(&bgpm);
    //QWallpaper::setAppWallpaper(&(alphaProcess(QString("bg.png"),bgcolor)));
    flag = false;
    isWap = false;
    wapFlag = false;
    isfive = false;
    progress = 0;
    scitymenu = new QPopupMenu(this);
    mgr = new SETUP_SET_Manager("weather.cfg", true);
    mgrcity = new SETUP_SET_Manager("citys.cfg", true);
    mgr->getSettingInfo("SET", "profilename", profilename, "none", true);
    mgr->getSettingInfo("SET", "saveprofilename", saveprofilename,true);
    mgr->getSettingInfo("SET", "units", units, "m", true);
    mgr->getSettingInfo("SET", "currentcity", currentCity, "CHXX0101", true);
    mgrcity->getSettingInfo(currentCity, "cclsup", cs, "1997:01:01:01:00:00", false);
    mgrcity->getSettingInfo(currentCity, "dayflsup", ds, "1997:01:01:01:00:00", false);

    // GUI layout
    UTIL_CST *cp = new UTIL_CST(this, tr("Update"));
    ZPushButton *quit = cp->getRightBtn();
    ZPushButton *update = cp->getMidBtn();
    ZPushButton *menu = cp->getLeftBtn();

    menuPop=new QPopupMenu(this);

    //recent city
    mgrcity->getGroupsKeyList(updatelist);
    recentmenu = new QPopupMenu(this);
    for(unsigned int i=0;i<updatelist.count();i++)
    {
        QString tmpname;
        mgrcity->getSettingInfo(updatelist[i], "dnam", tmpname, "CHXX0101", true);
        recentmenu->insertItem(tmpname,i);
    }
    searchID = menuPop->insertItem(tr("Search"),this,SLOT(tosearch()));
    menuPop->insertSeparator();
    menuPop->insertItem(tr("Recent"),recentmenu);
    menuPop->insertSeparator();
    unitID = menuPop->insertItem( (units == "m")?tr("Imperial"):tr("Metric"),this,SLOT(setUnits()) );
    proID = menuPop->insertItem( saveprofilename?tr("Do not remember"):tr("Remember Profile"),this,SLOT(ReProfile()));
    menuPop->insertItem(tr("About..."),this,SLOT(about()));
    menu->setPopup( menuPop );

    connect( recentmenu, SIGNAL(activated(int)), SLOT(recentslots(int)) );

    setCSTWidget(cp);

    sv = new ZScrollView(this);
    sv->enableClipper(true);
    sv->setFont(QFont::QFont("", 14, 75));
    sv->setDragAutoScroll(true);

    QLabel *windL = new QLabel(tr("Wind:"),sv->viewport());
    sv->addChild(windL,5,110);

    QLabel *visL = new QLabel(tr("Visibility:"),sv->viewport());
    sv->addChild(visL,5,88);

    QLabel *uviL = new QLabel(tr("UV index:"),sv->viewport());
    sv->addChild(uviL,5,66);

    QLabel *flikL = new QLabel(tr("Feels Like:"),sv->viewport());
    sv->addChild(flikL,5,44);

    QLabel *hmidL = new QLabel(tr("Humidity:"),sv->viewport());
    sv->addChild(hmidL,5,22);

    cchmidT = new QLabel(sv->viewport());
    sv->addChild(cchmidT,80,22,true);

    ccflikT = new QLabel(sv->viewport());
    sv->addChild(ccflikT,80,44,true);

    ccuviT = new QLabel(sv->viewport());
    sv->addChild(ccuviT,80,66,true);

    ccvisT = new QLabel(sv->viewport());
    sv->addChild(ccvisT,80,88,true);

    ccwindT = new QLabel(sv->viewport());
    ccwindT->resize(120, 22);
    sv->addChild(ccwindT,50,110);

    ccicon = new QLabel(sv->viewport());
    ccicon->resize(52, 52);
    ccicon->setPixmap(alphaProcess(QString("icon/d/44.png"),bgcolor));
    sv->addChild(ccicon,170,50);

    cctmpT = new QLabel(sv->viewport());
    cctmpT->setFont(QFont::QFont("", 16, 75));
    cctmpT->resize(52, 22);
    cctmpT->setAlignment(Qt::AlignRight);
    sv->addChild(cctmpT,170,27);

    ccT = new QLabel(sv->viewport());
    ccT->setFont(QFont::QFont("", 18, 75));
    sv->addChild(ccT,5,0);

    inputText = new QLineEdit( sv->viewport() );
    inputText->resize(240, 25);
    inputText->hide();
    search = new ZPushButton( sv->viewport() );
    search->setText(tr("Search"));
    search->resize(120, 30);
    search->hide();
    cancel = new ZPushButton( sv->viewport() );
    cancel->setText(tr("Cancel"));
    cancel->resize(120, 30);
    cancel->hide();
    sv->addChild(inputText,0,0,true);
    sv->addChild(search,0,25);
    sv->addChild(cancel,120,25,true);

    pbr = new UTIL_ProgressBar(sv->viewport());
    pbr->resize(240, 5);
    pbr->hide();
    pbr->setTotalSteps(100);
    pbr->setProgress(0);
    sv->addChild(pbr,0,0,true);

    iv = new QIconView(sv->viewport(), "iv");
    iv->resize(240, 120);
    iv->setArrangement(QIconView::TopToBottom);//横向滚动
    iv->setDragAutoScroll(true);
    sv->addChild(iv,0,135,true);

    setContentWidget(sv);

    Title = new ZPushButton(0);
    setTitleBarWidget(Title);
    Title->setFont(QFont::QFont("", 14, 75));

    connect( Title, SIGNAL(clicked()), SLOT(showFullScreen()) );
    connect( update, SIGNAL(clicked()), SLOT(update()) );
    connect( search, SIGNAL(clicked()), SLOT(searchcity()) );
    connect( cancel, SIGNAL(clicked()), SLOT(tocancel()) );
    connect( quit, SIGNAL(clicked()), qApp, SLOT(quit()) );
    connect( scitymenu, SIGNAL(activated(int)), SLOT(togcity(int)) );
    connect( iv, SIGNAL(clicked(QIconViewItem *)), SLOT(switchDN(QIconViewItem *)) );

    // create the socket and connect various of its signals
    socket = new QSocket( this );
    connect( socket, SIGNAL(connected()),
            SLOT(socketConnected()) );
    connect( socket, SIGNAL(connectionClosed()),
            SLOT(socketConnectionClosed()) );
    connect( socket, SIGNAL(readyRead()),
            SLOT(socketReadyRead()) );
    connect( socket, SIGNAL(error(int)),
            SLOT(socketError(int)) );
    connect( socket, SIGNAL(hostFound()),
            SLOT(socketHostFound()) );

    // create GPRS connect

    conn = new ZDataSessionManager();
    connect(conn, SIGNAL( connected(int, ZLinkInfo&) ), SLOT(
        slotLinkConnected(int, ZLinkInfo&) ));
    connect(conn, SIGNAL( openFailed(int, unsigned short, unsigned short) ), SLOT(
        slotLinkOpenFailed(int, unsigned short, unsigned short) ));
    connect(conn, SIGNAL( broken(int, ZLinkInfo&) ), SLOT(
        slotLinkBroken(int, ZLinkInfo&) ));

    if((!saveprofilename) || (profilename == "none"))
    {
        if(getProfileNamebySelect(profilename) == 0)
        {
            setGPRSmode(profilename);
        }
        else
        {
            Title->setText(tr("User cancel"));
            exit(0);
        }
    }
    else
    {
        setGPRSmode(profilename);
    }
}

void Weather::about()
{
    showNormal();
    ZMainWidget *about = new ZMainWidget(tr("About..."),false,0,"about");
    UTIL_CST *aboutCst = new UTIL_CST(about, tr("OK"),EZX_CST_BTN_MENU,"CST_Back");
    QTextView *aboutContent = new QTextView(about, "aboutContent");
               //aboutContent->setPalette( QPalette( QColor( 100, 100, 100),QColor( 100, 100, 100) ) );
               aboutContent->setFont(QFont::QFont("", 12, 75));
               aboutContent->setHScrollBarMode(QScrollView::AlwaysOff);
               aboutContent->setVScrollBarMode(QScrollView::AlwaysOff);
               //aboutContent->setPaper(QBrush(QColor( 148, 166, 204),QPixmap("Logos.jpg")));
               aboutContent->setText("<p align=\"center\"><big>EZXWeather for E6/A1200</big></p>"
                                          "<hr/>"
                                          "<p>This software is free software, you can free dissemination and use. Prohibited any form of commercial uses, including bundled in paid products.<br/><br/>"
                                          "Thank Yan0, Foxe6 such as the contribution. To thank my wife and my family to my encouragement and support!<br/>"
                                          "<a href='http://www.dnxh.cn'>http://www.dnxh.cn</a><br/><a href='http://www.dnxh.cn/blog'>http://www.dnxh.cn/blog</a></p>");
    about->setContentWidget(aboutContent);
    about->setCSTWidget(aboutCst);
    QObject::connect( aboutCst->getMidBtn(), SIGNAL(clicked()), about, SLOT(close()));
    QObject::connect( aboutCst->getRightBtn(), SIGNAL(clicked()), about, SLOT(close()));
    QObject::connect( aboutCst->getMidBtn(), SIGNAL(clicked()), this, SLOT(showFullScreen()));
    QObject::connect( aboutCst->getRightBtn(), SIGNAL(clicked()), this, SLOT(showFullScreen()));
    about->show();

}

void Weather::setGPRSmode(QString pro)
{
    QFile tmp("/ezxlocal/download/appwrite/NetProfile/"+pro);
    if((tmp.exists()) && (tmp.open(IO_ReadOnly)))
    {
        QTextStream t( &tmp );
        QString s;
        s = t.read();
        if(s.contains("cmnet")>0)
        {
            dbg_printf("cmnet\n");
            isWap = false;
        }
        if(s.contains("cmwap")>0)
        {
            dbg_printf("cmwap\n");
            isWap = true;
            wapFlag = true;
        }
        tmp.close();
    }
    Title->setText(tr("Create GPRS connect..."));
    rc = conn->init();
    if(rc<0)
    {
        Title->setText(NAPIerr(linkId));
    }
    linkId = conn->openLinkNoDialog(profilename);
    if(linkId<0)
    {
        Title->setText(NAPIerr(linkId));
    }
    mgr->setSettingInfo("SET", "profilename", profilename, true);
}

QString Weather::NAPIerr(int t)
{
    QString s;
    switch(t)
    {
        case -1:
            s = tr("Not initialized");
            break;
        case -2:
            s = tr("Invalid profile");
            break;
        case -3:
            s = tr("Already initialized");
            break;
        case -4:
            s = tr("DSM socket");
            break;
        case -5:
            s = tr("Name too long");
            break;
        case -6:
            s = tr("DSM response");
            break;
        case -8:
            s = tr("Bind failed");
            break;
        case -100:
            s = tr("Link removed");
            break;
        default :
            s = tr("Unknown error");
    }
    return s;
}

Weather::~Weather()
{
}

bool Weather::qwsEvent(QWSEvent* e)
{
    QWSFocusEvent* focus;
    if( e->type == QWSEvent::Focus)
    {

        focus=(QWSFocusEvent*)e;
        if (focus->simpleData.get_focus==0)
        {
            showNormal();
        }
        else
        {
            //if(fullscreen)
               //showFullScreen();
        }
    }
    return 0;
}

QString Weather::getnodetext(QDomElement Element,const char*nodename, int i)
{
    QDomNode n = Element.elementsByTagName(nodename).item(i);
    QDomElement e = n.toElement();
        return e.text();
}

void Weather::insertData()
{
    Title->setText(tr("Weather-Update..."));

    QDomElement docElem;
    QDomNode headElem,locElem,ccElem,dayfElem;
    QDomDocument doc( "getxml" );
    doc.setContent(xmlStr.replace(QRegExp("N\\/A"),"--"));
    docElem = doc.documentElement();


    QFile f("tmp/"+currentCity+".xml");
    if(f.open( IO_ReadOnly ))
    {
        QDomDocument olddoc( "oldxml" );
        olddoc.setContent( &f );
        headElem = olddoc.documentElement().elementsByTagName("head").item(0);
        locElem = olddoc.documentElement().elementsByTagName("loc").item(0);
        ccElem = olddoc.documentElement().elementsByTagName("cc").item(0);
        dayfElem = olddoc.documentElement().elementsByTagName("dayf").item(0);
    }
    f.close();

    //error
    if(docElem.tagName()=="error")
    {
        Title->setText(tr("Weather server ERR"));
        dbg_printf("%s\n", xmlStr.latin1());
        return;
    }

    //search city
    if(docElem.tagName()=="search")
    {
        if((docElem.elementsByTagName("loc").length()>0))
        {

            Title->setText(tr("Select City"));
            seachlist.clear();
            scitymenu->clear();
            int citysnum = docElem.elementsByTagName("loc").length();
            for(int ci=0;ci<citysnum;ci++)
            {
                seachlist<<docElem.elementsByTagName("loc").item(ci).toElement().attribute("id");
                scitymenu->insertItem(docElem.elementsByTagName("loc").item(ci).toElement().text(),ci,ci);
                dbg_printf("%s\n", docElem.elementsByTagName("loc").item(ci).toElement().text().latin1());
            }
            scitymenu->exec(QPoint(0,30));
        }
        else
        {
            Title->setText(tr("No City"));
        }
        return;
    }

    //loc
    if((docElem.elementsByTagName("loc").length()>0) && (docElem.elementsByTagName("head").length()>0))
    {
        headElem = docElem.elementsByTagName("head").item(0);
        locElem = docElem.elementsByTagName("loc").item(0);
        code = locElem.toElement().attribute("id");
        mgr->setSettingInfo("SET", "currentcity", code, true);
        mgrcity->setSettingInfo(code, "dnam", getnodetext(locElem.toElement(), "dnam", 0), true);
    }

    //CC
    if(docElem.elementsByTagName("cc").length()>0)
    {
        ccElem = docElem.elementsByTagName("cc").item(0);
    }
    if(!lsup.isNull())mgrcity->setSettingInfo(code, "cclsup", lsup, true);
    ccT->setText(c2s(getnodetext(ccElem.toElement(), "t", 0)));
    cchmidT->setText(getnodetext(ccElem.toElement(), "hmid", 0)+"%");
    ccflikT->setText(getnodetext(ccElem.toElement(), "flik", 0)+tr("°"));
    ccuviT->setText(getnodetext(ccElem.toElement(), "i", 0));
    if(getnodetext(ccElem.toElement(), "vis", 0)!="Unlimited")
        ccvisT->setText(c2s(getnodetext(ccElem.toElement(), "vis", 0))+c2s(getnodetext(headElem.toElement(), "ud", 0)));
    else
        ccvisT->setText(c2s(getnodetext(ccElem.toElement(), "vis", 0)));
    if(getnodetext(ccElem.toElement(), "t", 1) == "CALM")
        ccwindT->setText(c2s("Calm"));
    else
        ccwindT->setText(c2s(getnodetext(ccElem.toElement(), "t", 1))+" "+getnodetext(ccElem.toElement(), "s", 0)+c2s(getnodetext(headElem.toElement(), "us", 0)));
    ccicon->setPixmap(QPixmap("icon/d/"+getnodetext(ccElem.toElement(), "icon", 0)+".png"));
    ccicon->update();
    cctmpT->setText(getnodetext(ccElem.toElement(), "tmp", 0)+tr("°"));
    dbg_printf("cc lsup : %s\n",getnodetext(ccElem.toElement(), "lsup", 0).latin1());

    //dayf
    if(docElem.elementsByTagName("dayf").length()>0)
    {
        dayfElem = docElem.elementsByTagName("dayf").item(0);
    }
    if(!lsup.isNull())mgrcity->setSettingInfo(code, "dayflsup", lsup, true);
    daylist = new QDomNodeList(dayfElem.toElement().elementsByTagName("day"));
    int daycount = daylist->count();
    iv->clear();
    for(int i=0;i<daycount;i++)
    {
        QDomElement dayiElem = daylist->item(i).toElement();
        QPixmap *pm = new QPixmap("icon/s/" + getnodetext(dayiElem, "icon", 0) + ".png");
        QString daytext = c2s(dayiElem.attribute("t").left(3)) + "\n";//星期
                daytext += c2s(getnodetext(dayiElem, "t", 0)) + "\n";//气象描述
                daytext += getnodetext(dayiElem, "hi", 0) + tr("°~") + getnodetext(dayiElem, "low", 0) + tr("°");//温度
        QIconViewItem *item =  new QIconViewItem( iv, daytext, *pm );
        item->setKey("d");
    }
    dbg_printf("dayf lsup : %s\n",getnodetext(dayfElem.toElement(), "lsup", 0).latin1());

    Title->setText(tr("Weather-")+getnodetext(locElem.toElement(), "dnam", 0));
    //write xml
    QDomDocument newdoc( "new" );
    QDomElement root = newdoc.createElement("weather");
                root.setAttribute("ver","2.0");
                root.appendChild(headElem);
                root.appendChild(locElem);
                root.appendChild(ccElem);
                root.appendChild(dayfElem);
    f.remove();
    if(f.open(IO_ReadWrite ))
    {
        QTextStream t(&f);
        root.save(t,0);
    }
    f.close();

     xmlStr = "";
     mgr->flush();
     mgrcity->flush();

    //update recent city
    mgrcity->getGroupsKeyList(updatelist);
    recentmenu->clear();
    for(unsigned int i=0;i<updatelist.count();i++)
    {
        QString tmpname;
        mgrcity->getSettingInfo(updatelist[i], "dnam", tmpname, "CHXX0101", true);
        recentmenu->insertItem(tmpname,i);
    }
}

int Weather::parsResponse()
{
    if( ! GetData.startsWith( "HTTP/1." ) )
    {
        return -11;
    }
    int rc = GetData.mid(9, 3).toInt();
    if((rc > 299) || (rc < 200))
    {
        Title->setText(tr("Weather-HTTP %1 ERR...").arg(rc));
        //dbg_printf("Failed Requests\n");
        return -rc;
    }

    //get data
    QString s = GetData.mid(GetData.find("Date",0,false),(GetData.find("\n",GetData.find("Date",0,false),false) - GetData.find("Date",0,false)));
    QStringList date = QStringList::split(" ",s);
    QStringList months;
    months << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
    lsup = "%1:%2:%3:%4";
    lsup = lsup.arg(date[4]).arg(months.findIndex(date[3])+1).arg(date[2]).arg(date[5]);

    if(GetData.find("mpdData[",0,false) != -1)
    {
        QDomNode headElem,locElem,ccElem,dayfElem;
        QFile f("tmp/"+currentCity+".xml");
        if(f.open( IO_ReadOnly ))
        {
            QDomDocument olddoc( "oldxml" );
            olddoc.setContent( &f );
            headElem = olddoc.documentElement().elementsByTagName("head").item(0);
            locElem = olddoc.documentElement().elementsByTagName("loc").item(0);
            ccElem = olddoc.documentElement().elementsByTagName("cc").item(0);
            dayfElem = olddoc.documentElement().elementsByTagName("dayf").item(0);
        }
        f.close();

        //write xml
        QDomDocument newdoc( "new" );
        QDomNode dayfElem2(dayfElem);

        QStringList daylist = QStringList::split(";\n",GetData);
        //dbg_printf("%s\n",daylist.join("\n\n").latin1());
        for ( QStringList::Iterator it = daylist.begin(); it != daylist.end(); ++it )
        {
            if((*it).find(QRegExp("mpdData\\[\\'dayf\\'\\]\\.day\\[[5-9]\\]"),0) != -1)
            {
                QStringList daydatalist = QStringList::split(",",(*it).replace(QRegExp("\\]="),",").replace(QRegExp("mpdData\\[\\'dayf\\'\\]\\.day\\["),"").replace(QRegExp("new mpdFDObj\\(new Date\\("),"").replace(QRegExp("'"),""));
                dbg_printf( "%s|%s|%s|%s\n", daydatalist[0].latin1(),daydatalist[1].latin1(),daydatalist[12].latin1(),daydatalist[15].latin1() );
                QDomElement day = newdoc.createElement("day");
                            day.setAttribute("t",daydatalist[7]);
                            day.setAttribute("d",daydatalist[0]);
                            day.setAttribute("dt",daydatalist[2]+" "+daydatalist[3]);
                QDomElement hi = newdoc.createElement("hi");

                            hi.appendChild(newdoc.createTextNode((units == "m")?f2c(daydatalist[8]):daydatalist[8]));
                            day.appendChild(hi);
                QDomElement low = newdoc.createElement("low");
                            low.appendChild(newdoc.createTextNode((units == "m")?f2c(daydatalist[9]):daydatalist[9]));
                            day.appendChild(low);
                QDomElement sunr = newdoc.createElement("sunr");
                            sunr.appendChild(newdoc.createTextNode("--"));
                            day.appendChild(sunr);
                QDomElement suns = newdoc.createElement("suns");
                            suns.appendChild(newdoc.createTextNode("--"));
                            day.appendChild(suns);
                            QDomElement part1 = newdoc.createElement("part");
                                        part1.setAttribute("p","d");
                                        QDomElement icon = newdoc.createElement("icon");
                                                    icon.setTagName("icon");
                                                    icon.appendChild(newdoc.createTextNode(daydatalist[11]));
                                                    part1.appendChild(icon);
                                        QDomElement t = newdoc.createElement("t");
                                                    t.setTagName("t");
                                                    t.appendChild(newdoc.createTextNode(daydatalist[12]));
                                                    part1.appendChild(t);
                                        QDomElement wind = newdoc.createElement("wind");
                                                    wind.setTagName("wind");
                                                    QDomElement s = newdoc.createElement("s");
                                                                s.setTagName("s");
                                                                s.appendChild(newdoc.createTextNode(daydatalist[13]));
                                                                wind.appendChild(s); 
                                                    QDomElement gust = newdoc.createElement("gust");
                                                                gust.setTagName("gust");
                                                                gust.appendChild(newdoc.createTextNode("--"));
                                                                wind.appendChild(gust); 
                                                    QDomElement d = newdoc.createElement("d");
                                                                d.setTagName("d");
                                                                d.appendChild(newdoc.createTextNode("--"));
                                                                wind.appendChild(d); 
                                                    QDomElement wt = newdoc.createElement("t");
                                                                wt.setTagName("t");
                                                                wt.appendChild(newdoc.createTextNode(daydatalist[15]));
                                                                wind.appendChild(wt);
                                                    part1.appendChild(wind);
                                        QDomElement bt = newdoc.createElement("bt");
                                                    bt.setTagName("bt");
                                                    bt.appendChild(newdoc.createTextNode("--"));
                                                    part1.appendChild(bt);
                                        QDomElement ppcp = newdoc.createElement("ppcp");
                                                    ppcp.setTagName("ppcp");
                                                    ppcp.appendChild(newdoc.createTextNode(daydatalist[16]));
                                                    part1.appendChild(ppcp);
                                        QDomElement hmid = newdoc.createElement("hmid");
                                                    hmid.setTagName("hmid");
                                                    hmid.appendChild(newdoc.createTextNode(daydatalist[17]));
                                                    part1.appendChild(hmid);
                                        day.appendChild(part1);
                            QDomElement part2 = newdoc.createElement("part");
                                        part2.setAttribute("p","n");
                                        part2.appendChild(icon.cloneNode());
                                        part2.appendChild(t.cloneNode());
                                        part2.appendChild(wind.cloneNode());
                                        part2.appendChild(bt.cloneNode());
                                        part2.appendChild(ppcp.cloneNode());
                                        part2.appendChild(hmid.cloneNode());
                                        day.appendChild(part2);
                                        //dbg_printf("%s\n",sdff.latin1());
                dayfElem2.appendChild(day);
            }
        }

        QDomElement root = newdoc.createElement("weather");
                    root.setAttribute("ver","2.0");
                    root.appendChild(headElem);
                    root.appendChild(locElem);
                    root.appendChild(ccElem);
                    root.appendChild(dayfElem2);
        f.remove();
        if(f.open(IO_ReadWrite ))
        {
            QTextStream t(&f);
            root.save(t,0);
        }
        f.close();
        return 1;
    }
    //get xmldata
    if(GetData.find("<?xml",0,false) != -1)
    {
        xmlStr = GetData.mid(GetData.find("<?xml",0,false),GetData.findRev(">",-1,false)-GetData.find("<?xml",0,false)+1);
        return 1;
    }
    mgr->setSettingInfo("SET", "currentcity", currentCity, true);
    mgr->flush();
    return -101;
}

QString Weather::c2s(QString o)
{
    QString c = o.replace(QRegExp(" "), "").lower();
    QString s;
    if(c=="km/h"){s = tr("km/h");return s;}else
    if(c=="km"){s = tr("km");return s;}else
    if(c=="mm"){s = tr("mm");return s;}else
    if(c=="mi"){s = tr("mi");return s;}else
    if(c=="in"){s = tr("in");return s;}else
    if(c=="mph"){s = tr("mph");return s;}else
    if(c=="unlimited"){s = tr("Unlimited");return s;}else
    if(c=="sun"){s = tr("SUN");return s;}else
    if(c=="mon"){s = tr("MON");return s;}else
    if(c=="tue"){s = tr("TUE");return s;}else
    if(c=="wed"){s = tr("WED");return s;}else
    if(c=="thu"){s = tr("THU");return s;}else
    if(c=="fri"){s = tr("FRI");return s;}else
    if(c=="sat"){s = tr("SAT");return s;}else
    if(c=="e"){s = tr("E");return s;}else
    if(c=="s"){s = tr("S");return s;}else
    if(c=="n"){s = tr("N");return s;}else
    if(c=="w"){s = tr("W");return s;}else
    if(c=="ne"){s = tr("NE");return s;}else
    if(c=="sw"){s = tr("SW");return s;}else
    if(c=="se"){s = tr("SE");return s;}else
    if(c=="nw"){s = tr("NW");return s;}else
    if(c=="wnw"){s = tr("WNW");return s;}else
    if(c=="nne"){s = tr("NNE");return s;}else
    if(c=="ese"){s = tr("ESE");return s;}else
    if(c=="nnw"){s = tr("NNW");return s;}else
    if(c=="ssw"){s = tr("SSW");return s;}else
    if(c=="wsw"){s = tr("WSW");return s;}else
    if(c=="ene"){s = tr("ENE");return s;}else
    if(c=="sse"){s = tr("SSE");return s;}else
    if(c=="var"){s = tr("VAR");return s;}else
    if(c=="calm"){s = tr("CALM");return s;}else
    if(c=="afewclouds"){s = tr("A Few Clouds");return s;}else
    if(c=="amclouds/pmsun"){s = tr("AM Clouds / PM Sun");return s;}else
    if(c=="amclouds/pmsun/wind"){s = tr("AM Clouds / PM Sun / Wind");return s;}else
    if(c=="amdrizzle"){s = tr("AM Drizzle");return s;}else
    if(c=="amlightrain"){s = tr("AM Light Rain");return s;}else
    if(c=="amlightrain/wind"){s = tr("AM Light Rain / Wind");return s;}else
    if(c=="amlightsnow"){s = tr("AM Light Snow");return s;}else
    if(c=="amlightsnow/wind"){s = tr("AM Light Snow / Wind");return s;}else
    if(c=="amlightwintrymix"){s = tr("AM Light Wintry Mix");return s;}else
    if(c=="amrRain"){s = tr("AM Rain");return s;}else
    if(c=="amrain/ice"){s = tr("AM Rain / Ice");return s;}else
    if(c=="amrain/snow"){s = tr("AM Rain / Snow");return s;}else
    if(c=="amrain/snow/wind"){s = tr("AM Rain / Snow / Wind");return s;}else
    if(c=="amrain/snowshowers"){s = tr("AM Rain / Snow Showers");return s;}else
    if(c=="amrain/wind"){s = tr("AM Rain / Wind");return s;}else
    if(c=="amshowers"){s = tr("AM Showers");return s;}else
    if(c=="amshowers/wind"){s = tr("AM Showers / Wind");return s;}else
    if(c=="amsnow"){s = tr("AM Snow");return s;}else
    if(c=="amsnowshowers"){s = tr("AM Snow Showers");return s;}else
    if(c=="amsnowshowers/wind"){s = tr("AM Snow Showers / Wind");return s;}else
    if(c=="amthunderstorms"){s = tr("AM Thunderstorms");return s;}else
    if(c=="amwintrymix"){s = tr("AM Wintry Mix");return s;}else
    if(c=="blowingdust"){s = tr("Blowing Dust");return s;}else
    if(c=="blowingsandandwindy"){s = tr("Blowing Sand and Windy");return s;}else
    if(c=="blowingsnow"){s = tr("Blowing Snow");return s;}else
    if(c=="clear"){s = tr("Clear");return s;}else
    if(c=="cloudsearly/clearinglate"){s = tr("Clouds Early / Clearing Late");return s;}else
    if(c=="cloudy"){s = tr("Cloudy");return s;}else
    if(c=="cloudy/wind"){s = tr("Cloudy / Wind");return s;}else
    if(c=="cloudyandwindy"){s = tr("Cloudy and Windy");return s;}else
    if(c=="driftingsnow"){s = tr("Drifting Snow");return s;}else
    if(c=="drizzle"){s = tr("Drizzle");return s;}else
    if(c=="fair"){s = tr("Fair");return s;}else
    if(c=="fairandwindy"){s = tr("Fair and Windy");return s;}else
    if(c=="fewshowers"){s = tr("Few Showers");return s;}else
    if(c=="fewshowers/wind"){s = tr("Few Showers / Wind");return s;}else
    if(c=="fewsnowshowers"){s = tr("Few Snow Showers");return s;}else
    if(c=="fewsnowshowers/wind"){s = tr("Few Snow Showers / Wind");return s;}else
    if(c=="flurries"){s = tr("Flurries");return s;}else
    if(c=="flurries/wind"){s = tr("Flurries / Wind");return s;}else
    if(c=="fog"){s = tr("Fog");return s;}else
    if(c=="freezingrain"){s = tr("Freezing Rain");return s;}else
    if(c=="haze"){s = tr("Haze");return s;}else
    if(c=="heavydrizzle"){s = tr("Heavy Drizzle");return s;}else
    if(c=="heavyrain"){s = tr("Heavy Rain");return s;}else
    if(c=="heavyrain/wind"){s = tr("Heavy Rain / Wind");return s;}else
    if(c=="heavyrainshower"){s = tr("Heavy Rain Shower");return s;}else
    if(c=="heavyrainshowerandwindy"){s = tr("Heavy Rain Shower And Windy");return s;}else
    if(c=="heavysnow"){s = tr("Heavy Snow");return s;}else
    if(c=="heavyt-storms/wind"){s = tr("Heavy T-Storms / Wind");return s;}else
    if(c=="hvyrain/freezingrain"){s = tr("Hvy Rain / Freezing Rain");return s;}else
    if(c=="heavyt-stormandwindy"){s = tr("Heavy T-storm And Windy");return s;}else
    if(c=="heavyt-storms"){s = tr("Heavy T-storms");return s;}else
    if(c=="heavyt-stormswind"){s = tr("heavyt-stormswind");return s;}else
    if(c=="icetorain"){s = tr("Ice to Rain");return s;}else
    if(c=="isolatedt-storms/wind"){s = tr("Isolated T-Storms / Wind");return s;}else
    if(c=="isolatedt-storms"){s = tr("Isolated T-Storms");return s;}else
    if(c=="lightdrizzle"){s = tr("Light Drizzle");return s;}else
    if(c=="lightfreezingdrizzle"){s = tr("Light Freezing Drizzle");return s;}else
    if(c=="lightfreezingrain"){s = tr("Light Freezing Rain");return s;}else
    if(c=="lightrain"){s = tr("Light Rain");return s;}else
    if(c=="lightrainlate"){s = tr("Light Rain Late");return s;}else
    if(c=="lightrain/freezingrain"){s = tr("Light Rain / Freezing Rain");return s;}else
    if(c=="lightrain/wind"){s = tr("Light Rain / Wind");return s;}else
    if(c=="lightrainandfog"){s = tr("Light Rain And Fog");return s;}else
    if(c=="lightrainandfreezingrain"){s = tr("Light Rain And Freezing Rain");return s;}else
    if(c=="lightrainandwindy"){s = tr("Light Rain And Windy");return s;}else
    if(c=="lightrainshower"){s = tr("Light Rain Shower");return s;}else
    if(c=="lightrainshowerandwindy"){s = tr("Light Rain Shower and Windy");return s;}else
    if(c=="lightrainwiththunder"){s = tr("Light Rain with Thunder");return s;}else
    if(c=="lightsnow"){s = tr("Light Snow");return s;}else
    if(c=="lightsnow/wind"){s = tr("Light Snow / Wind");return s;}else
    if(c=="lightsnowandfog"){s = tr("Light Snow and Fog");return s;}else
    if(c=="lightsnowgrains"){s = tr("Light Snow Grains");return s;}else
    if(c=="lightsnowlate"){s = tr("Light Snow Late");return s;}else
    if(c=="lightsnowshower"){s = tr("Light Snow Shower");return s;}else
    if(c=="lightsnowshower/wind"){s = tr("Light Snow Shower / Wind");return s;}else
    if(c=="mist"){s = tr("Mist");return s;}else
    if(c=="mostlyclear"){s = tr("Mostly Clear");return s;}else
    if(c=="mostlyclear/wind"){s = tr("Mostly Clear / Wind");return s;}else
    if(c=="mostlycloudy"){s = tr("Mostly Cloudy");return s;}else
    if(c=="mostlycloudy/wind"){s = tr("Mostly Cloudy / Wind");return s;}else
    if(c=="mostlycloudyandwindy"){s = tr("Mostly Cloudy and Windy");return s;}else
    if(c=="mostlysunny"){s = tr("Mostly Sunny");return s;}else
    if(c=="mostlysunny/wind"){s = tr("Mostly Sunny / Wind");return s;}else
    if(c=="partlycloudy"){s = tr("Partly Cloudy");return s;}else
    if(c=="partlycloudy/wind"){s = tr("Partly Cloudy / Wind");return s;}else
    if(c=="partlycloudyandwindy"){s = tr("Partly Cloudy and Windy");return s;}else
    if(c=="patchesoffog"){s = tr("Patches Of Fog");return s;}else
    if(c=="pmdrizzle"){s = tr("PM Drizzle");return s;}else
    if(c=="pmlightrain"){s = tr("PM Light Rain");return s;}else
    if(c=="pmlightrain/ice"){s = tr("PM Light Rain / Ice");return s;}else
    if(c=="pmlightrain/wind"){s = tr("PM Light Rain / Wind");return s;}else
    if(c=="pmlightsnow"){s = tr("PM Light Snow");return s;}else
    if(c=="pmlightsnow/wind"){s = tr("PM Light Snow / Wind");return s;}else
    if(c=="pmrain"){s = tr("PM Rain");return s;}else
    if(c=="pmrain/snow"){s = tr("PM Rain / Snow");return s;}else
    if(c=="pmrain/snow/wind"){s = tr("PM Rain / Snow / Wind");return s;}else
    if(c=="pmrain/snowwhowers"){s = tr("PM Rain / Snow Showers");return s;}else
    if(c=="pmwhowers"){s = tr("PM Showers");return s;}else
    if(c=="pmshowers/wind"){s = tr("PM Showers / Wind");return s;}else
    if(c=="pmsnow"){s = tr("PM Snow");return s;}else
    if(c=="pmsnow/wind"){s = tr("PM Snow / Wind");return s;}else
    if(c=="pmsnowshowers"){s = tr("PM Snow Showers");return s;}else
    if(c=="pmsnowshowers/wind"){s = tr("PM Snow Showers / Wind");return s;}else
    if(c=="pmt-storms"){s = tr("PM T-Storms");return s;}else
    if(c=="pmwintrymix"){s = tr("PM Wintry Mix");return s;}else
    if(c=="rain"){s = tr("Rain");return s;}else
    if(c=="rainlate"){s = tr("Rain Late");return s;}else
    if(c=="rain/freezingrain"){s = tr("Rain / Freezing Rain");return s;}else
    if(c=="rain/sleet"){s = tr("Rain / Sleet");return s;}else
    if(c=="rain/snow"){s = tr("Rain / Snow");return s;}else
    if(c=="rainandsnow"){s = tr("Rain and Snow");return s;}else
    if(c=="rain/snow/wind"){s = tr("Rain / Snow / Wind");return s;}else
    if(c=="rain/snowshowers"){s = tr("Rain / Snow Showers");return s;}else
    if(c=="rain/snowshowers/wind"){s = tr("Rain / Snow Showers / Wind");return s;}else
    if(c=="rain/snowshowerslate"){s = tr("Rain / Snow Showers Late");return s;}else
    if(c=="rain/thunder"){s = tr("Rain / Thunder");return s;}else
    if(c=="rain/thunder/wind"){s = tr("Rain / Thunder / Wind");return s;}else
    if(c=="rainandsleet"){s = tr("Rain And Sleet");return s;}else
    if(c=="rain/wind"){s = tr("Rain / Wind");return s;}else
    if(c=="rainshower"){s = tr("Rain Shower");return s;}else
    if(c=="rainshowerandwindy"){s = tr("Rain Shower and Windy");return s;}else
    if(c=="raintosnow"){s = tr("Rain to Snow");return s;}else
    if(c=="raintosnow/wind"){s = tr("Rain to Snow / Wind");return s;}else
    if(c=="scatteredflurries"){s = tr("Scattered Flurries");return s;}else
    if(c=="scatteredflurries/wind"){s = tr("Scattered Flurries / Wind");return s;}else
    if(c=="scatteredshowers"){s = tr("Scattered Showers");return s;}else
    if(c=="scatteredshowers/wind"){s = tr("Scattered Showers / Wind");return s;}else
    if(c=="scatteredsnowshowers"){s = tr("Scattered Snow Showers");return s;}else
    if(c=="scatteredsnowshowers/wind"){s = tr("Scattered Snow Showers / Wind");return s;}else
    if(c=="scatteredstrongstorms"){s = tr("Scattered Strong Storms");return s;}else
    if(c=="scatteredt-storms"){s = tr("Scattered T-Storms");return s;}else
    if(c=="scatteredt-storms/wind"){s = tr("Scattered T-Storms / Wind");return s;}else
    if(c=="scatteredthunderstorms"){s = tr("Scattered Thunderstorms");return s;}else
    if(c=="showers"){s = tr("Showers");return s;}else
    if(c=="showers/wind"){s = tr("Showers / Wind");return s;}else
    if(c=="showers/windlate"){s = tr("Showers / Wind Late");return s;}else
    if(c=="showersearly"){s = tr("Showers Early");return s;}else
    if(c=="showersinthevicinity"){s = tr("Showers in the Vicinity");return s;}else
    if(c=="showerslate"){s = tr("Showers Late");return s;}else
    if(c=="sleet"){s = tr("Sleet");return s;}else
    if(c=="smoke"){s = tr("Smoke");return s;}else
    if(c=="snow"){s = tr("Snow");return s;}else
    if(c=="snow/wind"){s = tr("Snow / Wind");return s;}else
    if(c=="snowandicetorain"){s = tr("Snow and Ice to Rain");return s;}else
    if(c=="snowgrains"){s = tr("Snow Grains");return s;}else
    if(c=="snowshower"){s = tr("Snow Shower");return s;}else
    if(c=="snowshowerwind"){s = tr("Snow Shower / Wind");return s;}else
    if(c=="snowshowerwindearly"){s = tr("Snow Shower / Wind Early");return s;}else
    if(c=="snowshowersearly"){s = tr("Snow Showers Early");return s;}else
    if(c=="snowshowerslate"){s = tr("Snow Showers Late");return s;}else
    if(c=="snowtoice"){s = tr("Snow to Ice");return s;}else
    if(c=="snowtoice/wind"){s = tr("Snow to Ice / Wind");return s;}else
    if(c=="snowtorain"){s = tr("Snow to Rain");return s;}else
    if(c=="snowtorain/wind"){s = tr("Snow to Rain / Wind");return s;}else
    if(c=="snowtowintrymix"){s = tr("Snow to Wintry Mix");return s;}else
    if(c=="sprinkles"){s = tr("Sprinkles");return s;}else
    if(c=="strongstorms"){s = tr("Strong Storms");return s;}else
    if(c=="strongstorms/wind"){s = tr("Strong Storms / Wind");return s;}else
    if(c=="sunny"){s = tr("Sunny");return s;}else
    if(c=="sunnywindy"){s = tr("Sunny / Windy");return s;}else
    if(c=="sunnywind"){s = tr("Sunny / Wind");return s;}else
    if(c=="t-showers"){s = tr("T-Showers");return s;}else
    if(c=="t-showers/wind"){s = tr("T-Showers / Wind");return s;}else
    if(c=="t-storm"){s = tr("T-Storm");return s;}else
    if(c=="t-storms"){s = tr("T-Storms");return s;}else
    if(c=="t-storms/wind"){s = tr("T-Storms / Wind");return s;}else
    if(c=="t-stormsearly"){s = tr("T-Storms Early");return s;}else
    if(c=="t-stormslate"){s = tr("T-Storms Late");return s;}else
    if(c=="thunder"){s = tr("Thunder");return s;}else
    if(c=="t-stormsandwintrymix"){s = tr("Thunder And Wintry Mix");return s;}else
    if(c=="t-stormsinthevicinity"){s = tr("Thunder in the Vicinity");return s;}else
    if(c=="widespreaddust"){s = tr("Widespread Dust");return s;}else
    if(c=="wintrymix"){s = tr("Wintry Mix");return s;}else
    if(c=="wintrymix/wind"){s = tr("Wintry Mix / Wind");return s;}else
    if(c=="wintrymixtosnow"){s = tr("Wintry Mix to Snow");return s;}else
    if(c=="notavailable"){s = tr("not available");return s;}else
    if(c=="unknown"){s = tr("Unknown");return s;}else
    {s = o;}
    return s;
}

void Weather::switchDN(QIconViewItem *item)
{
    if(item!=NULL)
    {
        QDomElement dayiElem = daylist->item(item->index()).toElement();
        if(item->key() == "d")
        {
            item->setPixmap(QPixmap("icon/s/" + getnodetext(dayiElem, "icon", 1) + ".png"));
            QString daytext = c2s(dayiElem.attribute("t").left(3)) + "\n";//星期
                    daytext += c2s(getnodetext(dayiElem, "t", 2)) + "\n";//气象描述
                    daytext += getnodetext(dayiElem, "hi", 0) + tr("°~") + getnodetext(dayiElem, "low", 0) + tr("°");//温度
            item->setText(daytext);
            item->setKey("n");
        }
        else
        {
            item->setPixmap(QPixmap("icon/s/" + getnodetext(dayiElem, "icon", 0) + ".png")); 
            QString daytext = c2s(dayiElem.attribute("t").left(3)) + "\n";//星期
                    daytext += c2s(getnodetext(dayiElem, "t", 0)) + "\n";//气象描述
                    daytext += getnodetext(dayiElem, "hi", 0) + tr("°~") + getnodetext(dayiElem, "low", 0) + tr("°");//温度
            item->setText(daytext);
            item->setKey("d");
        }
        iv->updateScrollBars();
    }
}

void Weather::togcity(int id)
{
    currentCity = seachlist[id];
    update();
}

void Weather::recentslots(int id)
{
    currentCity = updatelist[id];
    update();
}

void Weather::tosearch()
{
    //search city
    inputText->show();
    search->show();
    cancel->show();
}

void Weather::searchcity()
{
    //http://xoap.weather.com/search/search?where=nantong
    SendTypes="GetCode";
    if(isWap)
    {
       socket->connectToHost( "10.0.0.172", 80 );
    }
    else
    {
       socket->connectToHost( "xoap.weather.com", 80 );
    }
    inputText->hide();
    search->hide();
    cancel->hide();
}
void Weather::tocancel()
{
    inputText->hide();
    search->hide();
    cancel->hide();
}
void Weather::setUnits()
{
    if(units == "m")
    {
        menuPop->changeItem(unitID,tr("Metric"));
        units = "s";
    }
    else
    {
        menuPop->changeItem(unitID,tr("Imperial"));
        units = "m";
    }
    mgr->setSettingInfo("SET", "units", units, true);
    QFile f("tmp/"+currentCity+".xml");
    f.remove();
    update();
}

void Weather::ReProfile()
{
    if(saveprofilename)
    {
        saveprofilename = false;
        menuPop->changeItem(proID,tr("Remember Profile"));
    }
    else
    {
        saveprofilename = true;
        menuPop->changeItem(proID,tr("Do not remember"));
    }
    mgr->setSettingInfo("SET", "saveprofilename", saveprofilename);
}

void Weather::update()
{
    mgrcity->getSettingInfo(currentCity, "cclsup", cs, "1997:01:01:01:00:00", false);
    mgrcity->getSettingInfo(currentCity, "dayflsup", ds, "1997:01:01:01:00:00", false);
    if((cs.isNull()) || (ds.isNull()))return;
    QDateTime n = QDateTime::currentDateTime();
    QStringList sl = QStringList::split(":",cs);
    QDateTime c(QDate((int)(sl[0].toInt()),(int)(sl[1].toInt()),(int)(sl[2].toInt())),
                QTime((int)(sl[3].toInt()),(int)(sl[4].toInt()),(int)(sl[5].toInt())) );
    sl = QStringList::split(":",ds);
    QDateTime d(QDate((int)(sl[0].toInt()),(int)(sl[1].toInt()),(int)(sl[2].toInt())),
                QTime((int)(sl[3].toInt()),(int)(sl[4].toInt()),(int)(sl[5].toInt())) );

    QFile f("tmp/"+currentCity+".xml");
    dbg_printf("update to now = %d\n",c.secsTo(n));
    if((c.secsTo(n) > 60*25) || !f.exists())
    {
        SendTypes="GetCC";
        if((d.secsTo(n) > 60*210) || !f.exists())
        {
            SendTypes="GetFive";
        }
        if(socket->state() == QSocket::Connection )
        {
           return;
        }
        else
        {
            Title->setText(tr("try connect to server"));
            if(isWap)
            {
                socket->connectToHost( "10.0.0.172", 80 );
            }
            else
            {
                socket->connectToHost( "xoap.weather.com", 80 );
            }
        }
    }
    else
    {
         QCString tmp;
         tmp.resize(f.size());
         if(f.open(IO_ReadWrite ))
         {
             f.readBlock(tmp.data(),f.size());
         }
         f.close();
         xmlStr = QString(tmp);
         insertData();
    }

}

void Weather::slotLinkConnected(int linkid, ZLinkInfo&)
{
    showFullScreen();
    linkId = linkid;
    conn->bindSocket2Link( socket->socket(), linkId );
    update();
}

void Weather::socketHostFound()
{
   Title->setText(tr("Host is found"));
}

void Weather::closeConnection()
{
    socket->close();
    if ( socket->state() == QSocket::Closing ) {
        // We have a delayed close.
        connect( socket, SIGNAL(delayedCloseFinished()),
                SLOT(socketClosed()) );
    } else {
        // The socket is closed.
        socketClosed();
    }
}

void Weather::sendToServer()
{
    // write to the server
    Title->setText(tr("Weather-Request..."));
    QString command;
    if((SendTypes=="GetFive")||(SendTypes=="GetCC"))
    {
        command = "GET http://xoap.weather.com/weather/local/";
        command += currentCity;
        command += "?";
        command += "cc=*";
        command += "&prod=xoap&link=xoap&par=";
        command += "1017780104";
        command += "&key=";
        command += "7d248cae7bcf8618";
        if(SendTypes=="GetFive")
        {
            command += "&dayf=";
            command += "5";
        }
        command += "&unit=";
        command += units;
        command += " HTTP/1.1\r\n";
        command += "Accept: */*\r\n";
        command += "Accept-Language: en-us\r\n";
        command += "Accept-Encoding: gzip, deflate\r\n";
        command += "Proxy-Connection: Close\r\n";
        command += "Connection: Close\r\n";
        command += "Host: xoap.weather.com\r\n";
        command += "X-Online-Host: xoap.weather.com\r\n";
        command += "\r\n";
    }
    if(SendTypes=="GetCode")
    {
        command = "GET http://xoap.weather.com/search/search?where=";
        command += inputText->text();
        command += " HTTP/1.1\r\n";
        command += "Accept: */*\r\n";
        command += "Accept-Language: en-us\r\n";
        command += "Accept-Encoding: gzip, deflate\r\n";
        command += "Proxy-Connection: Close\r\n";
        command += "Connection: Close\r\n";
        command += "Host: xoap.weather.com\r\n";
        command += "X-Online-Host: xoap.weather.com\r\n";
        command += "\r\n";
    }
    if(SendTypes=="GetTen")
    {
        command = "GET http://www.weather.com/weather/mpdwcr/tenday?locid=";
        command += currentCity;
        command += " HTTP/1.1\r\n";
        command += "Accept: */*,text/javascript\r\n";
        command += "Accept-Language: en-us\r\n";
        command += "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n";
        command += "Proxy-Connection: Close\r\n";
        command += "Connection: Close\r\n";
        command += "Host: www.weather.com\r\n";
        command += "X-Online-Host: www.weather.com\r\n";
        command += "\r\n";
    }
    socket->writeBlock(command.latin1(),command.length());
    pbr->show();
}

void Weather::socketReadyRead()
{
    // read from the server
    int tmpp = socket->bytesAvailable();
    progress += tmpp;
    dbg_printf(">>> %d bytes available\n", tmpp);
    if(wapFlag)
    {
        QCString s;
        s.resize(socket->bytesAvailable());
        socket->readBlock(s.data(),socket->bytesAvailable());
        dbg_printf("%s\n",s.data());
        if(QString(s).contains("dnxh.cn") == 1)
        {
            wapFlag = false;
            dbg_printf("is wap\n");
            return;
        }
    }

    while ( socket->canReadLine() )
    {
        QString s = socket->readLine();
        //dbg_printf("%s",s.latin1());
        GetData.append(s);
        if((progress==tmpp) && (s.find("Content-Length: ",0,false)!=-1))
        {
            Title->setText(tr("Weather-Accept..."));
            s = s.right(s.length()-QString("Content-Length: ").length());
            pbr->setTotalSteps(s.toInt());
            dbg_printf("%s\n%d\n",s.latin1(),s.toInt());
            progress -= 1;
        }
    }
    pbr->setProgress(progress);
}

void Weather::socketConnected()
{
    Title->setText(tr( "Connected to server" ));
    if(wapFlag)
    {
      QString command;
      command = "GET http://10.0.0.127 HTTP/1.1\r\n";
      command += "Accept: */*\r\n";
      command += "X-Online-Host: www.dnxh.cn\r\n\r\n";
      socket->writeBlock(command.latin1(),command.length());
    }
    sendToServer();
}

void Weather::socketConnectionClosed()
{
    dbg_printf("Connection closed by the server\n");
    //dbg_printf("%s\n",GetData.latin1());
    if(parsResponse()==1)
    {
        insertData();
    }
    progress = 0;
    pbr->setProgress(0);
    pbr->hide();
    GetData = "";
    if((!isfive) && (SendTypes=="GetFive"))
    {
        SendTypes = "GetTen";
        Title->setText(tr( "ten" ));
        socket->connectToHost( "www.weather.com", 80 );
    }
}

void Weather::socketClosed()
{
    Title->setText(tr("Connection closed"));
}

void Weather::socketError( int e )
{
    if(e==QSocket::ErrConnectionRefused){Title->setText( tr("Connection was refused"));return;}else
    if(e==QSocket::ErrHostNotFound){Title->setText( tr("Host was not found"));return;}else
    if(e==QSocket::ErrSocketRead){Title->setText( tr("Read from the socket failed"));return;}else
    {Title->setText(tr("Unknown error occurred: %1").arg(e) );}
}

QPixmap Weather::alphaProcess(QString aName,QColor bgcolor)
{
    QImage aPic,bPic;
    QPixmap resultPic;
    int aPicH,aPicW;
    int x,y,b1,b2,b3,o1,o2,o3,r0,g0,b0,alphaV;
    aPic.load(aName);
    aPicW = aPic.width();
    aPicH = aPic.height();
    QRgb pb;
    o1 = bgcolor.red(); o2 = bgcolor.green(); o3 = bgcolor.blue();
    for (y=0;y<aPicH;y++)
    {
        for (x=0;x<aPicW;x++)
        {
            pb = aPic.pixel(x,y);
            b1 = qRed(pb); b2 = qGreen(pb); b3 = qBlue(pb);
            alphaV = qAlpha(pb);
            r0 = (alphaV*b1+(255-alphaV)*o1) / 255;
            g0 = (alphaV*b2+(255-alphaV)*o2) / 255;
            b0 = (alphaV*b3+(255-alphaV)*o3) / 255;
            aPic.setPixel(x,y,qRgb(r0,g0,b0));
        }
    }
    resultPic.convertFromImage(aPic,QPixmap::Color);
    return resultPic;
}

QString Weather::f2c(QString f)
{
    double fi,ci;
    QString c;
    fi = f.toDouble();
    ci = 5.00/9*(fi-32);
    c=c.setNum(ci,'f',0);
    //printf("%d  %d\n",fi,ci);
    return c;
}
