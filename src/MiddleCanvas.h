#pragma once
#include <gui/View.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include <td/Variant.h>
#include<td/String.h>
#include <algorithm>
#include <gui/DrawableButton.h>
#include <gui/ViewScroller.h>
#include <td/Types.h>
#include "SendMessage.h"
#include "GlobalsCanvas.h"
#include "ViewCertainRequest.h"
#include "WindowCertainRequest.h"
#include "ViewTicketForSAO.h"
#include "ViewIDs.h"
#include <vector>
#include <gui/DrawableString.h>
#include <gui/Transformation.h>

class MiddleCanvas : public gui::Canvas
{
private:


   td::INT4 predmetID = -1; // grupa 2 
   td::String _subjectname;
protected:
    gui::Image _etf;
    gui::Shape _shapeCircle1;
    gui::Circle c;
    gui::Rect r;
    gui::Rect r1;
    gui::Rect r2;
    gui::Rect r3;
    gui::Rect r4;
    gui::Rect r5;
    gui::Shape _roundedRect1;
    gui::Shape _roundedRect2;
    gui::Shape _roundedRect3;
    gui::Shape _roundedRect4;
    gui::Shape _roundedRect5;
    gui::Shape _shapeCircle;
    gui::Circle cc;
    gui::Circle ccc;
    gui::Rect chat1;
    gui::Shape _chat1;
    gui::Rect chat2;
    gui::Shape _chat2;
    int i = 0;
    td::INT4 brojPoruke = -1;
    td::INT4 _chatUserID;
    td::String _name;
    td::INT4 _h;
    td::INT4 x;
    td::INT4 y;
    std::string str;
    gui::DrawableString dr;
    dp::IDatabase* _db;
    MsgSender msg;
    gui::Image img;
    td::INT4 red=1;
    td::INT4 skrolV = 0;
    td::String indeks;
    td::String Ime;
    td::String prezime;
    td::String tipKarte;
    td::String request;
    td::String title;
    td::String status;
    gui::Rect rectBottomRight;
    gui::Point mousePosition;



    std::vector <gui::Circle> _emojis;


    

    bool _otvoreno;
    bool _blob;

    gui::Image _add;
    gui::Image _e1;
    gui::Image _e2;
    gui::Image _e3;
    gui::Image _e4;
    gui::Image _e5;
    gui::Image _e6;
    gui::Image _e7;
    gui::Image _e8;
    gui::Image _e9;
    gui::Image _e10;
    gui::Image _ee;

    td::INT4 IDTicket=-1;

    gui::Canvas* farleft;



    td::INT4 openChatButtonPressed = -1;


public:
    MiddleCanvas()
     : _etf(":ETF")
     , gui::Canvas({ gui::InputDevice::Event::PrimaryClicks,  gui::InputDevice::Event::Keyboard ,gui::InputDevice::Event::CursorMove})
     , _chatUserID(-2)
     , _h(450)
     , _db(dp::getMainDatabase())
     , img(":circ")
     , mousePosition(0, 0)
        , _add(":add2")
        , _otvoreno(false)
        , _e1(":emoji1")
        , _e2(":emoji2")
        , _e3(":emoji3")
        , _e4(":emoji4")
        , _e5(":emoji5")
        , _e6(":emoji6")
        , _e7(":emoji7")
        , _e8(":emoji8")
        , _e9(":emoji9")
        , _e10(":emoji10")
        , _blob(false)
 {


 }

    void setSubjectID(td::INT4 id) { //grupa 2 - potreban subjectID

        predmetID = id;  //linije 53, 54

        reset();//da bi se nakon klika refreshao middle canvas
    }
    void setSubjectName(td::String ime) {
        _subjectname = ime;
    }

    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3


        gui::Size sz1;
        getSize(sz1);

        if ((Globals::_currentUserRole == 1 || Globals::_currentUserRole == 3 || openChatButtonPressed!=-1) && _chatUserID != -2) {             //---> Dodano za chat


            gui::Size sz;
            getSize(sz);


            gui::Point cp1(sz.width / 2, sz.height / 2);
            td::INT4 x1 = cp1.x;
            td::INT4 y1 = cp1.y;

            gui::Rect imgRect(x1 - 15 - x1 / 4, y1 - 15 - y1 / 4, x1 + 15 + x1 / 4, y1 + 15 + y1 / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center); //etf logo


            gui::Point cp(sz.width / 8, sz.height / 8);
            x = cp.x;
            y = cp.y;



            gui::Rect r3(0, 1, 8 * x, y - 1);
            _roundedRect3.createRoundedRect(r3, 20, 1, td::LinePattern::Solid);
            _roundedRect3.drawFill(td::ColorID::SkyBlue); //bar za prikazivanje osobe i sl 

            if (Globals::_currentUserID == _chatUserID) {
                gui::DrawableString nesto = "Biljeske"; 
                nesto.draw(r3, gui::Font::ID::SystemLargestBold, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
            }
            else {
                gui::DrawableString nesto = td::String(_name);
                nesto.draw(r3, gui::Font::ID::SystemLargestBold, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
            }
            showChat();


        
            if (_h > sz.height) {
                gui::Rect r1(0, _h - y, sz.width, _h);

                td::INT4 pom = _h;
                _roundedRect2.createRoundedRect(r1, 20, 1, td::LinePattern::Solid);
                _roundedRect2.drawFill(td::ColorID::SkyBlue); //prostor u kojem se kucaju poruke

                // gui::Rect r2(10, 7 * y, 8 * x - (16 + x / 6)*2, 8 * y);
                gui::Rect r2((5 * x) / 12 + 10, _h - y, 8 * x - (16 + x / 6) * 2, _h);

                //_roundedRect1.createRoundedRect(r2, 20, 1, td::LinePattern::Solid);
                //_roundedRect1.drawFill(td::ColorID::Transparent); //prostor u kojem se nalaze poruke  

              //  gui::Rect r4(0, _h - 100, x/2, _h);
              //  _roundedRect4.createRoundedRect(r2, 20, 1, td::LinePattern::Solid);
              ////  _roundedRect4.drawFill(td::ColorID::Transparent); //prostor u kojem se nalaze poruke  



                dr = str;
                dr.draw(r2, gui::Font::ID::SystemNormal, td::ColorID::Black, td::TextAlignment::Left, td::VAlignment::Center, td::TextEllipsize::End); //no
                if (str.size() == 0)
                {
                    gui::DrawableString mes = "Message...";
                    mes.draw(r2, gui::Font::ID::SystemItalic, td::ColorID::Black, td::TextAlignment::Left, td::VAlignment::Center, td::TextEllipsize::End);
                }

                gui::Circle cc(8 * x - x / 4 - 7, _h - y / 2, 9 + x / 6);
                //   gui::Circle cc(8 * x - x / 4 - 7, 7 * (_h/8) + (_h/8) / 2, 9 + x / 6);
                   // _shapeCircle.createCircle(cc, 15, td::LinePattern::Solid);
                   // _shapeCircle.drawFill(td::ColorID::MediumBlue); //dugme za slanje poruke 
                img.draw(cc);

                gui::Circle ccc(x / 4, _h - y / 2, x / 6);
                _add.draw(ccc);

                if (_otvoreno) {


                    gui::Rect r5(x / 4, _h - y - 2, x / 4 + 200, _h - y - 102);
                    _roundedRect5.createRoundedRect(r5, 20, 1, td::LinePattern::Solid);
                    _roundedRect5.drawFill(td::ColorID::Gray);

                    gui::Circle e1(x / 4 + 20, _h - y - 77, 20);
                    _e1.draw(e1);

                    gui::Circle e2(x / 4 + 60, _h - y - 77, 20);
                    _e2.draw(e2);

                    gui::Circle e3(x / 4 + 100, _h - y - 77, 20);
                    _e3.draw(e3);

                    gui::Circle e4(x / 4 + 140, _h - y - 77, 20);
                    _e4.draw(e4);

                    gui::Circle e5(x / 4 + 180, _h - y - 77, 20);
                    _e5.draw(e5);

                    gui::Circle e6(x / 4 + 20, _h - y - 27, 20);
                    _e6.draw(e6);

                    gui::Circle e7(x / 4 + 60, _h - y - 27, 20);
                    _e7.draw(e7);

                    gui::Circle e8(x / 4 + 100, _h - y - 27, 20);
                    _e8.draw(e8);

                    gui::Circle e9(x / 4 + 140, _h - y - 27, 20);
                    _e9.draw(e9);

                    gui::Circle e10(x / 4 + 180, _h - y - 27, 20);
                    _e10.draw(e10);

                }
            }
            else {
                gui::Rect r1(0, sz.height - y, sz.width, sz.height);
                td::INT4 pom = _h;
                _roundedRect2.createRoundedRect(r1, 20, 1, td::LinePattern::Solid);
                _roundedRect2.drawFill(td::ColorID::SkyBlue); //prostor u kojem se kucaju poruke

                // gui::Rect r2(10, 7 * y, 8 * x - (16 + x / 6)*2, 8 * y);
                gui::Rect r2((5 * x) / 12 + 10, 7 * y, 8 * x - (16 + x / 6) * 2, 8 * y);

                //_roundedRect1.createRoundedRect(r2, 20, 1, td::LinePattern::Solid);
                //_roundedRect1.drawFill(td::ColorID::Black); //prostor u kojem se nalaze poruke



                dr = str;
                dr.draw(r2, gui::Font::ID::SystemNormal, td::ColorID::Black, td::TextAlignment::Left, td::VAlignment::Center, td::TextEllipsize::End); //no
                if (str.size() == 0)
                {
                    gui::DrawableString mes = "Message...";
                    mes.draw(r2, gui::Font::ID::SystemItalic, td::ColorID::Black, td::TextAlignment::Left, td::VAlignment::Center, td::TextEllipsize::End);
                }

                gui::Circle cc(8 * x - x / 4 - 7, sz.height - y / 2, 9 + x / 6);
                // _shapeCircle.createCircle(cc, 15, td::LinePattern::Solid);
                // _shapeCircle.drawFill(td::ColorID::MediumBlue); //dugme za slanje poruke 
                img.draw(cc);


                gui::Circle ccc(x / 4, sz.height - y / 2, x / 6);
                _add.draw(ccc);





                if (_otvoreno) {


                    gui::Rect r5(x / 4, sz.height - y - 2, x / 4 + 200, sz.height - y - 102);
                    _roundedRect5.createRoundedRect(r5, 20, 1, td::LinePattern::Solid);
                    _roundedRect5.drawFill(td::ColorID::Gray);

                    gui::Circle e1(x / 4 + 20, sz.height - y - 77, 20);
                    _e1.draw(e1);

                    gui::Circle e2(x / 4 + 60, sz.height - y - 77, 20);
                    _e2.draw(e2);

                    gui::Circle e3(x / 4 + 100, sz.height - y - 77, 20);
                    _e3.draw(e3);

                    gui::Circle e4(x / 4 + 140, sz.height - y - 77, 20);
                    _e4.draw(e4);

                    gui::Circle e5(x / 4 + 180, sz.height - y - 77, 20);
                    _e5.draw(e5);

                    gui::Circle e6(x / 4 + 20, sz.height - y - 27, 20);
                    _e6.draw(e6);

                    gui::Circle e7(x / 4 + 60, sz.height - y - 27, 20);
                    _e7.draw(e7);

                    gui::Circle e8(x / 4 + 100, sz.height - y - 27, 20);
                    _e8.draw(e8);

                    gui::Circle e9(x / 4 + 140, sz.height - y - 27, 20);
                    _e9.draw(e9);

                    gui::Circle e10(x / 4 + 180, sz.height - y - 27, 20);
                    _e10.draw(e10);

                }













            }



                //slobodno mijenjati boje po zelji, ja sam odabrao random boje 
               // showChat();
                getScroller()->setContentSize(sz);
        }

        // pogled za SAO ------ grupa 1
        else if (Globals::isSAO && brojPoruke > -1 && sz1.width > 300) {
            DrawChatInfoSAO();
        }



        // pogled za studenta ------ grupa 2 ------ ako ne koristite mozete samo ostaviti da bude nacrtan etf znak
        else if (Globals::isStudent) {

            //td::INT4 broj = predmetID;  //uzima random vrijednost, a u funkciji setSubjectID dobro ocita
            //td::INT4 broj = 1;  //  1 za provjeru select-a
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            cp.x = 10;
            cp.y = 10;
            gui::Point cp2;
            cp2.x = 65;
            td::String text, text1, text2, text3, text4;
            text = "Detalji o odabranom predmetu ";

            /* funkcije:
            static void draw(const td::String& txt, const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top,  td::TextEllipsize ellips = td::TextEllipsize::End);
            static void draw(const td::String& txt, const gui::Point& pt, gui::Font::ID fntID, td::ColorID clrID);
            */
            if (predmetID == -1) { //ovaj text2 bi trebao da piše čim se student uloguje, prije nego odabere neki predmet
                gui::DrawableString text1 = "Odaberite predmet da biste vidjeli detalje ";
                text1.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro);
            }
            else {
                gui::DrawableString text2 = "Predmet: ";
                text2.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro);
                int razmak = 40;
                cp.y = cp.y + razmak; // trebalo bi jos + visina slova prethodnog
                // + sta ako tekst zauzima npr 5 redova(teoretski )...
/*                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Coral);
                dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("Select Naziv_Predmeta AS Naziv FROM Predmet WHERE ID_Predmeta = ?");
                dp::Params pParams(pSelect->allocParams());
                pParams << broj;
                dp::Columns pCols = pSelect->allocBindColumns(1);
                pCols << "Naziv" << text1;
                if (!pSelect->execute())
                    return;
                while (pSelect->moveNext());    */
                cp.x = 10;
                gui::DrawableString::draw(_subjectname, cp, gui::Font::ID::SystemLargerBold, td::ColorID::DimGray);

                razmak += 5;
                cp.y += razmak;
                cp.x = 10;
                text = "Odgovorni nastavni ansambl: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("Select a.Ime AS ime, a.Prezime AS prezime, Pozicija.Naziv as Pozicija FROM Korisnici a, PredmetStaff b, Pozicija WHERE b.ID_Korisnika = a.ID AND a.PozicijaID=Pozicija.ID and b.ID_Predmeta = ?");
                dp::Params pParams1(pSelect1->allocParams());
                pParams1 << predmetID;
                dp::Columns pCols1 = pSelect1->allocBindColumns(3);
                td::String name, surname, pozicija;
                pCols1 << "ime" << name << "prezime" << surname << "Pozicija" << pozicija;
                // text1 += b;
                if (!pSelect1->execute())
                    return;
                while (pSelect1->moveNext());
                {   cp.x = 10; //+ text.length()
                cp.y += 25;
                pozicija += " : ";
                pozicija += surname;
                pozicija += " ";
                pozicija += name;
                gui::DrawableString Pozicija = pozicija;
                Pozicija.draw(cp, gui::Font::ID::SystemLargerBold, td::ColorID::DimGray);
                }
                razmak += 10;
                cp.y += razmak;
                td::INT4 Ypos = cp.y;
                cp.y += 50;
                cp.x = 10;
                text = "Aktivnosti na predmetu: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("SELECT Naziv_Aktivnosti AS Naziv FROM Aktivnosti WHERE ID_Predmeta = ?");
                dp::Params pParams2(pSelect2->allocParams());
                pParams2 << predmetID;
                dp::Columns pCols2 = pSelect2->allocBindColumns(1);
                td::String naziv;
                pCols2 << "Naziv" << naziv;
                if (!pSelect2->execute())
                    return;
                cp.x = cp.x + 75;
                while (pSelect2->moveNext()) {
                    cp.y += 20;
                    gui::DrawableString text3 = naziv;
                    text3.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::DimGray);
                }
                razmak += 10;
                cp.y += razmak;
                cp.x = 10;
                td::INT4 id = Globals::_currentUserID;;
                text = "Ocijenjeno: ";
                gui::DrawableString::draw(text, cp, gui::Font::ID::SystemLargerBold, td::ColorID::Gainsboro);
                dp::IStatementPtr pSelect3 = dp::getMainDatabase()->createStatement("Select a.Ocjena as ocjena, d.Naziv as tip FROM OcjeneIspita a, Aktivnosti c, VrstaAktivnosti d WHERE c.ID_Aktivnosti=a.ID_Aktivnosti and d.ID=a.ID_Aktivnosti and a.ID_Korisnika=? AND c.ID_Predmeta=?");
                //select radi (npr id korisnika = 5 i id predmeta = 1
                dp::Params pParams3(pSelect3->allocParams());
                pParams3 << id << predmetID;
                dp::Columns pCols3 = pSelect3->allocBindColumns(2);
                td::String ocjena;
                td::String tip;
                pCols3 << "ocjena" << ocjena << "tip" << tip;
                if (!pSelect3->execute())
                    return;
                cp.x = cp.x + 75;
                td::INT4 prosjek = 0.; int brojac = 0; std::vector<td::INT4> Ocjene;
                while (pSelect3->moveNext()) {
                    cp.y += 20;
                    Ocjene.push_back(ocjena.toINT4());
                    gui::DrawableString text4 = tip;
                    text4.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::DimGray);
                    cp.x = cp.x + tip.length() + 55;
                    gui::DrawableString text5 = ocjena;
                    text5.draw(cp, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::DimGray);
                    cp.x = cp.x - tip.length() - 55;
                }
                for (int i = 0; i < Ocjene.size(); i++)
                {
                    prosjek += Ocjene[i] * 20; //skalirano
                }
                if (Ocjene.size() != 0)
                    prosjek = prosjek / Ocjene.size();


                {  //Progres bar
                    gui::Point prog;

                    gui::Size sz2;
                    getSize(sz2);
                    prog.x = sz.width / 2 - 100;
                    prog.y = Ypos - 25;
                    gui::DrawableString osvojeno = "Osvojili ste...";
                    osvojeno.draw(prog, gui::Font::ID::SystemBoldItalic, td::ColorID::Gainsboro);
                    prog.y = Ypos + 25;
                    gui::DrawableString moguce = "Od ukupno mogućih 100 bodova.";
                    moguce.draw(prog, gui::Font::ID::SystemBoldItalic, td::ColorID::Gainsboro);
                    gui::Rect prazan(0, 0, 200, 20);
                    gui::Rect fill(0, 0, prosjek, 20);
                    gui::DrawableString postotak;
                    prazan.translate(sz2.width / 2 - 100, Ypos);
                    fill.translate(sz2.width / 2 - 100, Ypos);
                    gui::Shape::drawRect(prazan, td::ColorID::White, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    if (prosjek <= 108) //zato sto je skalirano 
                        gui::Shape::drawRect(fill, td::ColorID::Red, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    else if (prosjek <= 148)
                        gui::Shape::drawRect(fill, td::ColorID::Yellow, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    else
                        gui::Shape::drawRect(fill, td::ColorID::Green, td::ColorID::Black, 0.5, td::LinePattern::Solid);

                    prosjek /= 2;
                    td::String pom = std::to_string(prosjek);
                    pom += "%";
                    //std::string pom1 = std::to_string(prosjek);
                    postotak = pom;
                    postotak.draw(prazan, gui::Font::ID::SystemBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
                }

                {   // TABELA PRISUSTVA

                    gui::Transformation zoomTr;
                    double zoom = 1.; //experiment with this and see impact
                    double scrollX = 10;
                    double scrollY = cp.y + 70;
                    zoomTr.translate(scrollX, scrollY);
                    zoomTr.scale(zoom);
                    zoomTr.appendToContext(); //this is transformationfor all objects

                    gui::Point privremena;
                    privremena.x = 0;
                    privremena.y = -25;
                    gui::DrawableString prisustvolabovima = "Prisustvo lab. vjezbama: ";
                    prisustvolabovima.draw(privremena, gui::Font::ID::SystemBoldItalic, td::ColorID::Gainsboro);


                    int duzina = 100;
                    int visina = 30;
                    gui::Rect sedmica(0, 0, duzina, visina);
                    gui::Rect vrijeme(0, 0, duzina, visina);
                    gui::Rect prisutan(0, 0, duzina, visina);
                    gui::DrawableString sedm = "Sedmica";
                    gui::DrawableString time = "Vrijeme";
                    gui::DrawableString prisustvo = "Prisutan";
                    vrijeme.translate(0, visina);
                    prisutan.translate(0, 2 * visina);
                    gui::Shape::drawRect(sedmica, td::ColorID::DimGray, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    gui::Shape::drawRect(vrijeme, td::ColorID::DimGray, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    gui::Shape::drawRect(prisutan, td::ColorID::DimGray, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                    sedm.draw(sedmica, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro, td::TextAlignment::Center, td::VAlignment::Center);
                    time.draw(vrijeme, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro, td::TextAlignment::Center, td::VAlignment::Center);
                    prisustvo.draw(prisutan, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Gainsboro, td::TextAlignment::Center, td::VAlignment::Center);

                    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select Br_sedmice AS brsedmice FROM Prisustvo, TerminiStudenti, Termini WHERE TerminiStudenti.ID_Studenta = ? AND  TerminiStudenti.ID_Termina = Prisustvo.ID_termina AND Termini.ID = TerminiStudenti.ID_Termina AND Termini.TipPredavanjaID = 3 AND Termini.Predmet_ID=?");
                    dp::Params pParams(pSelect->allocParams());
                    pParams << Globals::_currentUserID << predmetID;
                    dp::Columns pCols = pSelect->allocBindColumns(1);
                    td::INT4 brsedmice;
                    pCols << "brsedmice" << brsedmice;
                    std::vector<td::INT4> prisutnesedmice;
                    if (pSelect->execute()) {
                        prisutnesedmice.resize(0);
                        while (pSelect->moveNext()) {
                            //gui::DrawableString drawableName = name;
                            prisutnesedmice.push_back(brsedmice);
                        }
                    }

                    // UZIMANJE SAMO VREMENA LAB.VJEZBE
                    dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("select Vrijeme as vrijeme FROM TerminiStudenti, Termini WHERE TerminiStudenti.ID_Studenta = ? AND Termini.ID = TerminiStudenti.ID_Termina AND Termini.TipPredavanjaID = 3 AND Termini.Predmet_ID=?");
                    dp::Params pParams1(pSelect1->allocParams());
                    pParams1 << Globals::_currentUserID << predmetID;
                    dp::Columns pCols1 = pSelect1->allocBindColumns(1);
                    td::Time Vrijeme;
                    pCols1 << "vrijeme" << Vrijeme;
                    if (pSelect1->execute()) {
                        while (pSelect1->moveNext());
                    }

                    for (int i = 0; i < 15; i++) {
                        int novaduzina = 50;
                        gui::Rect red_sedmica(0, 0, novaduzina, visina);
                        td::String broj = std::to_string(i + 1);
                        gui::DrawableString rednibroj = broj;
                        red_sedmica.translate(i * novaduzina + duzina, 0);
                        gui::Shape::drawRect(red_sedmica, td::ColorID::White, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                        rednibroj.draw(red_sedmica, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);

                        gui::Rect time1(0, 0, novaduzina, visina);
                        time1.translate(i * novaduzina + duzina, visina);
                        td::INT4 hour = Vrijeme.getHour();
                        td::String temp;
                        if (hour / 10 == 0)
                            temp += "0";

                        temp += std::to_string(Vrijeme.getHour());
                        temp += ":";
                        temp += std::to_string(Vrijeme.getMinute());
                        td::INT4 min = Vrijeme.getMinute();
                        if (min / 10 == 0)
                            temp += "0";
                        gui::DrawableString vrijemezaispis = temp;
                        gui::Shape::drawRect(time1, td::ColorID::White, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                        vrijemezaispis.draw(time1, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);

                        gui::Rect prisutan(0, 0, novaduzina, visina);
                        prisutan.translate(i * novaduzina + duzina, 2 * visina);
                        bool jeprisutan = false;
                        for (auto x : prisutnesedmice) {
                            if (x == i + 1) {
                                jeprisutan = true;
                                break;
                            }
                        }
                        if (jeprisutan) {
                            gui::DrawableString jestprisutan = "DA";
                            gui::Shape::drawRect(prisutan, td::ColorID::Green, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                            jestprisutan.draw(prisutan, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
                        }
                        else {
                            gui::DrawableString jestprisutan = "NE";
                            gui::Shape::drawRect(prisutan, td::ColorID::Red, td::ColorID::Black, 0.5, td::LinePattern::Solid);
                            jestprisutan.draw(prisutan, gui::Font::ID::SystemLargerBoldItalic, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
                        }
                    }
                }
            }
        }
        // kada nema ulogovane osobe
       /* else if(Globals::isLogged==check){

        }*/
        //samo srednji canvas crta etf znak
        else {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 30 - x / 4, y - 30 - y / 4, x + 30 + x / 4, y + 30 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }

    };




    void SetMessageNumSAO(td::INT4 br) {
        brojPoruke = br;
        reDraw();
    };




    void DrawChatInfoSAO() {

        enableResizeEvent(true);;

        gui::Size sz;
        getSize(sz);

        dp::IDataSet* pDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.Ime as Name, Korisnici.Prezime as Surname,SAOStudentTicket.Indeks as StudentIndex,"
            " SAOStudentTicket.Ticket_Tip as TypeOfTicket, SAOStudentTicket.Req_Title as TitleofTicket, SAOStudentTicket.Status_ID as Status_ID,SAOTicket_Status.Status as Status,"
            " SAOStudentTicket.Request as Request,SAOStudentTicket.ID as IDTicket From Korisnici, SAOStudentTicket, SAOTicket_Status where Korisnici.Indeks=SAOStudentTicket.Indeks AND SAOTicket_Status.ID=SAOStudentTicket.Status_ID");

        dp::DSColumns cols(pDS->allocBindColumns(9));
        cols << "Name" << td::string8 << "Surname" << td::string8 << "StudentIndex" << td::string8 << "TypeOfTicket" << td::string8 << "TitleOfTicket" << td::string8 << "Status_ID" << td::int4 << "Status" << td::string8 << "Request" << td::string8 << "IDTicket" << td::int4;
        if (!pDS->execute())
        {
            pDS = nullptr;
            showAlert("errorReadingTable", "");
            return;
        }

        //showAlert("", std::to_string(brojPoruke));


        auto row = pDS->getRow(brojPoruke);

        IDTicket = row[8].i4Val();


        td::String ime = row[0].getConstStr();
        ime.append(" ");
        ime.append(row[1].getConstStr());

        td::String req = row[7].getConstStr();



        td::INT4 maxLength = (int(sz.width - 200) / 8);


        if (req.length() > maxLength) {

            td::String pom = "";
            auto itA = req.begin();
            auto itB(itA);
            bool SpaceExists = false;
            auto pomit = itA;

            if (req.length() > 0)
                pom.append(req.subStr(0, 0));


            while (itA != req.end())
            {
                pomit = itA;
                //GET MAX RANGE FOR CHARS IN A ROW
                while (itB != req.end() && itB <= itA + maxLength)
                    itB++;
                //DOES SPACE EXIST IN CURRENT ROW?    
                while (pomit != itB)
                {
                    if (*pomit == ' ')
                    {
                        SpaceExists = true;  break;
                    }
                    pomit++;
                }
                //FIND SUITABLE PLACE TO BREAK REQUEST APART
                while (itB != req.end() && SpaceExists && *(itB) != ' ')
                    itB--;

                if (itB > ++itA)
                    pom.append(req.subStr(itA - req.begin(), itB - req.begin()));

                pom.append("\n");

                //showAlert(std::to_string(itA-req.begin()), std::to_string(itB-req.begin()));

                while (itA < itB)
                    itA++;


            }

            req = pom;

        }

        //TREBA MI ZA GLOBALNE VARIJABLE RADI PROSLIJEDJIVANJA

        //**************//
        Ime = row[0].getConstStr();
        prezime = row[1].getConstStr();
        indeks = row[2].getConstStr();
        tipKarte = row[3].getConstStr();
        request = row[7].getConstStr();
        title = row[4].getConstStr();
        status = row[6].getConstStr();
        //****************//



       // showAlert("",std::to_string(sz.width));


        gui::DrawableString Ime = ime;
        gui::DrawableString Tip = row[3].getConstStr();
        gui::DrawableString Title = row[4].getConstStr();
        gui::DrawableString Status = row[6].getConstStr();
        gui::DrawableString Request = req;

        gui::Size szpom;
        Request.measure(gui::Font::ID::SystemLarger, szpom);

        skrolV = szpom.height + 200;

        gui::Rect rectt(0, 0, sz.width - 100, skrolV);//

        if (skrolV < sz.height)
            rectt.setHeight(sz.height - 50);


        rectt.translate(50, 25);

        // gui::Shape::drawRect(rectt, td::ColorID::Silver, td::ColorID::MidnightBlue, 2, td::LinePattern::Solid);

        gui::Shape visibleRect;

        visibleRect.createRoundedRect(rectt, 10);
        visibleRect.drawFillAndWire(td::ColorID::Silver, td::ColorID::MidnightBlue);


        Ime.draw({ 75,50 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
        Tip.draw({ sz.width - 150,50 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
        Title.draw({ 75,75 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
        Status.draw({ sz.width - 150,75 }, gui::Font::ID::SystemBold, td::ColorID::Black);//
        if (!req.isNull())
            Request.draw({ 100,125 }, gui::Font::ID::SystemLarger, td::ColorID::Black);//

        szpom.height = skrolV;

        this->getScroller()->setContentSize(szpom);

        int bottomRightX = sz.width - 100; // X koordinata
        int bottomRightY = sz.height - 70; // Y koordinata
        int rectWidth = 80;
        int rectHeight = 30;

        szpom.width = rectWidth;
        szpom.height = rectHeight;


        if (skrolV > sz.height)
            bottomRightY = skrolV - 20;

        gui::Point origin(bottomRightX - rectWidth, bottomRightY - rectHeight);


        // Nacrtaj pravokutnik
        gui::DrawableString answer = tr("Odgovori");
        rectBottomRight.setOriginAndSize(origin, szpom);

        // gui::Shape::drawRect(rectBottomRight, td::ColorID::MidnightBlue, td::ColorID::MidnightBlue, 2, td::LinePattern::Solid);

        gui::Shape buttonSAO;
        buttonSAO.createRoundedRect(rectBottomRight, 10);

        gui::Point  p(bottomRightX - 73, bottomRightY - 27);

        int x = mousePosition.x;
        int y = mousePosition.y;

        if (x > rectBottomRight.left && x < rectBottomRight.right && y < rectBottomRight.bottom && y > rectBottomRight.top)
        {
            buttonSAO.drawFillAndWire(td::ColorID::AquaMarine, td::ColorID::MidnightBlue);
            answer.draw(p, gui::Font::ID::SystemBold, td::ColorID::Blue);
        }
        else
        {
            buttonSAO.drawFillAndWire(td::ColorID::Blue, td::ColorID::MidnightBlue);
            answer.draw(p, gui::Font::ID::SystemBold, td::ColorID::White);
        }


    };

    void reset() {
        _chatUserID = -2;
        brojPoruke = -1;         //-----Dodano za chat
        reDraw();
    }

    void Reset(td::INT4 userID, td::String s) {
        // id korisnika sa kojim je chat aktivan
        red = 1;
        _name = s;
        str = "";
        _chatUserID = userID;
        _otvoreno = false;
        _h = 450;

        reDraw();
    }

    void setChatButtonPressed(td::INT4 openChatPressed) {
        openChatButtonPressed = openChatPressed;
    }

    //bool insertMessage() {
    //    td::Date d(true);
    //   // td:time t(true);
    //    dp::IDatabase* _db;
    //    dp::IStatementPtr pInsStat(_db->createStatement("INSERT INTO Messages (AuthorID, Subject, Poruke, Datum, Vrijeme) VALUES (?, 'chat',?, ?, ?)"));
    //    dp::Params parDS(pInsStat->allocParams());
    //    parDS << Globals::_currentUserID << ovde treba uneseni string  <<d<<t;
    //    dp::Transaction tr(_db);
    //    tr.commit();
    //    
    //}


    bool insertMessage() {
        return msg.sendMsgtoUser("chat", str, _chatUserID,1);

        //ovdje treba dodati u neki pair chat user id i current user id i svakoj osobi kojoj je korisnik poslao poruku mora doci obavijest kada udje u aplikaciju, od koga je dosla
        //recimo da iskoci prozorcic nakon logina koji kaze da imate neprocitanu poruku od osobe ciji je userid koji vec
    }


    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override {
        if (Globals::_currentUserRole == 1 || Globals::_currentUserRole == 3) {
            gui::Size sz;
            getSize(sz);
            td::INT4 h1;
            if (_h > sz.height) {
                h1 = _h;
            }
            else {
                h1 = sz.height;
            }
            double tempk = 0;
            double x1 = 8 * x - x / 4;
            double y1 = h1 - y / 2;
            double yid = inputDevice.getModelPoint().y;
            double xid = inputDevice.getModelPoint().x;
            double xd = pow(xid - x1, 2);
            double yd = pow(yid - y1, 2);
            double d = sqrt(xd + yd);

            // if (tempk < int(inputDevice.getFramePoint().y) && inputDevice.getFramePoint().y < (tempk + _visinaChata)) {
            if (d <= 9 + x / 6) {
                insertMessage();
                str = "";
                //  showChat();
                red = 1;
                reDraw();
                // showAlert("OK","PRITISNUTO DUGME");
            }



            x1 = x / 4;
            y1 = h1 - y / 2;
            xd = pow(xid - x1, 2);
            yd = pow(yid - y1, 2);
            d = sqrt(xd + yd);

            if (d <= x / 6) {
                _otvoreno = !_otvoreno;
                reDraw();
            }
            //za add dugme kad se klikne
            //_otvoreno=!_otvoreno;
            //reDraw()

            if (_otvoreno) {

                td::INT4 x1 = x / 4 + 20;
                td::INT4 y1 = h1 - y - 77;


                for (int i = 0; i < 5; i++) {



                    xd = pow(xid - x1, 2);
                    yd = pow(yid - y1, 2);
                    d = sqrt(xd + yd);
                    x1 += 40;
                    if (d <= 20) {
                        /*  char l[100];
                          sprintf(l, "pritisnuto %d", i + 1);
                          showAlert("OK", l);*/
                          // insertBlob(i + 1);
                        char l1[100];
                        sprintf(l1, "blob%d", i + 1);
                        msg.sendMsgtoUser("chat", l1, _chatUserID, 1);
                        reDraw();
                        return;
                    }

                }
                x1 = x / 4 + 20;
                y1 = h1 - y - 27;
                for (int i = 5; i < 10; i++) {

                    xd = pow(xid - x1, 2);
                    yd = pow(yid - y1, 2);
                    d = sqrt(xd + yd);
                    x1 += 40;
                    if (d <= 20) {
                        /*   char l[100];
                           sprintf(l, "pritisnuto %d", i + 1);
                           showAlert("OK", l);*/
                           //insertBlob(i + 1);
                        char l1[100];
                        sprintf(l1, "blob%d", i + 1);
                        msg.sendMsgtoUser("chat", l1, _chatUserID, 1);
                        reDraw();
                        return;
                    }

                }



            }




            reDraw();

        }
        
      if (Globals::isSAO && inputDevice.getModelPoint().x>rectBottomRight.left && inputDevice.getModelPoint().x < rectBottomRight.right && 
      inputDevice.getModelPoint().y<rectBottomRight.bottom && inputDevice.getModelPoint().y > rectBottomRight.top) {
      gui::Window* pParentWnd = getParentWindow();
      auto pWnd = new WindowCertainRequest(pParentWnd,IDTicket, indeks, Ime, prezime, tipKarte, status, request, title);
      pWnd->keepOnTopOfParent();
      pWnd->open();
      reDraw();
  }
    }

    bool getModelSize(gui::Size& modelSize) const override
    {

        gui::Size sz;
        getSize(sz);


        if (Globals::isSAO) {

            modelSize.width = sz.width; //
            modelSize.height = skrolV+50;
            return true;
        }

        modelSize.width = sz.width; //
        modelSize.height = _h;
        return true;
    }

    void setModelHeight(gui::Size& modelSize, td::INT4 h) {
        modelSize.height = h;
    }

    td::INT4 getChatUserID() {

    }



    void showChat() {

        //  _h = 800;


        gui::Size sz;
        getSize(sz);

        gui::Point cp(sz.width / 8, sz.height / 8);

        td::INT4 x = cp.x;
        td::INT4 y = cp.y;

        td::INT4 dy = y;




        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Messages.Poruke, MsgReceivers.UserID FROM Messages JOIN MsgReceivers ON Messages.ID = MsgReceivers.MsgID WHERE ((MsgReceivers.UserID = ? AND Messages.AuthorID=?) OR (MsgReceivers.UserID = ? AND Messages.AuthorID=?)) AND Messages.Subject = 'chat' ORDER BY MsgReceivers.MsgID ASC;");
        dp::Columns pCols = pSelect->allocBindColumns(2);
        dp::Params parDS(pSelect->allocParams());
        parDS << _chatUserID << Globals::_currentUserID << Globals::_currentUserID << _chatUserID;

        td::String s;
        td::INT4 user_id;
        pCols << "Poruke" << s << "UserID" << user_id;

        if (!pSelect->execute()) {
            s = "Greska";
            user_id = -1;

        }
        while (pSelect->moveNext()) {

            std::string s1 = s.c_str();
            td::INT4 i = 0;
            td::INT4 id = user_id;
            if (s1 == "blob1" || s1 == "blob2" || s1 == "blob3" || s1 == "blob4" || s1 == "blob5" || s1 == "blob6" || s1 == "blob7" || s1 == "blob8" || s1 == "blob9" || s1 == "blob10") {
                _blob = true;
                if (s1 == "blob1") i = 1;
                if (s1 == "blob2") i = 2;
                if (s1 == "blob3") i = 3;
                if (s1 == "blob4") i = 4;
                if (s1 == "blob5") i = 5;
                if (s1 == "blob6") i = 6;
                if (s1 == "blob7") i = 7;
                if (s1 == "blob8") i = 8;
                if (s1 == "blob9") i = 9;
                if (s1 == "blob10") i = 10;

                switch (i) {
                case 1: _ee = _e1; break;
                case 2: _ee = _e2; break;
                case 3: _ee = _e3; break;
                case 4: _ee = _e4; break;
                case 5: _ee = _e5; break;
                case 6: _ee = _e6; break;
                case 7: _ee = _e7; break;
                case 8: _ee = _e8; break;
                case 9: _ee = _e9; break;
                case 10: _ee = _e10; break;

                }


            }



            int br = s1.length() / 31 + 1;

            /*  if (s1.length() > 30) {
                  std::string s2="";
                  for (int i = 0; i < s1.length() / 30; i++) {
                      int k = 0;
                      if (k + 30 < s1.length()) {
                          s2 += s1.substr(k, k + 29) + "\n";
                          k += 30;
                      }
                      else s2 += s1.substr(k, s1.length());
                  }

                  s1 = s2;
              }*/
            if (id == _chatUserID && id == Globals::_currentUserID) {
                if (!_blob) {
                    gui::Rect chat2(8 * x - 250, dy + 10, 8 * x - 10, dy + 10 + 25 * br);
                    _chat2.createRoundedRect(chat2, 5, 1, td::LinePattern::Solid);
                    _chat2.drawFill(td::ColorID::WhiteSmoke); //user
                    gui::DrawableString nesto2 = s;
                    nesto2.draw(chat2, gui::Font::ID::SystemNormal, td::ColorID::Black);
                    dy += 25 * br + 15;
                }
                if (_blob) {
                    td::INT4 c1 = (8 * x - 250 + 8 * x - 10) / 2;
                    td::INT4 c2 = (dy + 10 + dy + 10 + 25 * 3) / 2;
                    gui::Circle ee(c1, c2, 20);
                    _ee.draw(ee);
                    dy += 25 * 3 + 15;
                    _blob = false;
                }
            }
            if (id == _chatUserID && id != Globals::_currentUserID) {

                if (!_blob) {
                    // gui::Rect chat2(6 * x + 10, dy*y + 10, 8 * x - 2, (dy+1)* y - 2);
                    gui::Rect chat2(8 * x - 250, dy + 10, 8 * x - 10, dy + 10 + 25 * br);
                    _chat2.createRoundedRect(chat2, 5, 1, td::LinePattern::Solid);
                    _chat2.drawFill(td::ColorID::WhiteSmoke); //user
                    gui::DrawableString nesto2 = s;
                    nesto2.draw(chat2, gui::Font::ID::SystemNormal, td::ColorID::Black);
                    dy += 25 * br + 15;
                }
                if (_blob) {
                    td::INT4 c1 = (8 * x - 250 + 8 * x - 10) / 2;
                    td::INT4 c2 = (dy + 10 + dy + 10 + 25 * 3) / 2;
                    gui::Circle ee(c1, c2, 20);
                    _ee.draw(ee);
                    dy += 25 * 3 + 15;
                    _blob = false;
                }

            }

            if (id == Globals::_currentUserID && id != _chatUserID) {
                //  gui::Rect chat1(0 + 10, dy*y + 10, 2 * x - 2, (dy+1)* y - 2);
                if (!_blob) {
                    gui::Rect chat1(10, dy + 10, 250, dy + 10 + 25 * br);
                    _chat1.createRoundedRect(chat1, 5, 1, td::LinePattern::Solid);
                    _chat1.drawFill(td::ColorID::Gainsboro); //onaj sa kim se dopisujemo 
                    gui::DrawableString nesto = s;
                    nesto.draw(chat1, gui::Font::ID::SystemNormal, td::ColorID::Black);
                    dy += 25 * br + 15;
                }
                if (_blob) {
                    td::INT4 c1 = (10 + 250) / 2;
                    td::INT4 c2 = (dy + 10 + dy + 10 + 25 * 3) / 2;
                    gui::Circle ee(c1, c2, 20);
                    _ee.draw(ee);
                    dy += 25 * 3 + 15;
                    _blob = false;
                }
            }

            if (dy > _h - y) {
                _h += dy - _h + y + 5; //bilo +20

            }

        }

    }


    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.getVirtual() == gui::Key::Virtual::Delete)
        {

            if (!str.empty()) {
                str.erase(str.size() - 1);
                reDraw();
                return true;
            }
        }
        else if (key.getVirtual() == gui::Key::Virtual::BackSpace)
        {
            if (!str.empty()) {
                str.erase(str.size() - 1);
                reDraw();
                return true;
            }
        }
        else if (key.getVirtual() == gui::Key::Virtual::NumEnter)
            // else if (key.getCode() == 13)
        {
            //potrebna implementacija -- koje dugme ulazi u ovaj if
            if (!str.empty()) {
                str.erase(str.size() - 1);
                reDraw();
                return true;
            }
        }
        else if (key.getChar() >= 32 && key.getChar() <= 127) {
            for (char i = 32; i <= 127; i++) {
                if (key.getChar() == i)
                {
                    str = str + i;
                    reDraw();
                    return true;
                }
            }
        }
        // ako se promijeni na td::String pogledati da li rade afrikati
        // u tom slucaju dodati njihovu implementaciju rucno - za svaki afrikat



        return false;

    }

    
        void onCursorMoved(const gui::InputDevice& inputDevice)override {
      
        
        if (Globals::isSAO) {
            gui::Size sz;
            getSize(sz);

            int x = inputDevice.getModelPoint().x;
            int y = inputDevice.getModelPoint().y;

            mousePosition.x = x;
            mousePosition.y = y;

            if (x > rectBottomRight.left && x < rectBottomRight.right && y < rectBottomRight.bottom && y > rectBottomRight.top)
                setCursor(gui::Cursor::Type::Finger);
            else
                setCursor(gui::Cursor::Type::Default);

            reDraw();
        }
    }
        


};
