#pragma once
#include <gui/View.h>
#include <gui/Window.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/SplitterLayout.h>
#include <td/Variant.h>
#include<td/String.h>
#include <algorithm>
#include <gui/DrawableButton.h>
#include<iostream>
#include "GlobalsCanvas.h"
#include "ViewCertainRequest.h"
#include "WindowCertainRequest.h"
#include "ViewTicketForSAO.h"
#include "ViewIDs.h"

class MiddleCanvas : public gui::Canvas
{
private:
protected:
    gui::Image _etf;
<<<<<<< Updated upstream
    td::INT4 brojPoruke=-1;

=======
    gui::Shape _shapeCircle1;
    gui::Circle c;
    gui::Rect r1;
    gui::Rect r2;
    gui::Rect r3;
    gui::Shape _roundedRect1;
    gui::Shape _roundedRect2;
    gui::Shape _roundedRect3;
    gui::Shape _shapeCircle;
    gui::Circle cc;
    int i = 0;
    td::INT4 brojPoruke = -1;
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
>>>>>>> Stashed changes

public:
    MiddleCanvas()
        : _etf(":ETF")
<<<<<<< Updated upstream
=======
        , gui::Canvas({ gui::InputDevice::Event::PrimaryClicks,gui::InputDevice::Event::CursorMove })
        , mousePosition(0, 0)
>>>>>>> Stashed changes
    {

    }

    void onDraw(const gui::Rect& rect) override {
        const bool check = false;
        // pogled za profesora i asistenta ------ grupa 3

<<<<<<< Updated upstream
             if (Globals::_currentUserID == 1 || Globals::_currentUserID == 3) {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;
=======
        if (Globals::_currentUserID == 1 || Globals::_currentUserID == 3) {

            gui::Size sz;
            getSize(sz);


            gui::Point cp1(sz.width / 2, sz.height / 2);
            td::INT4 x1 = cp1.x;
            td::INT4 y1 = cp1.y;

            gui::Rect imgRect(x1 - 15 - x1 / 4, y1 - 15 - y1 / 4, x1 + 15 + x1 / 4, y1 + 15 + y1 / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center); //etf logo


            gui::Point cp(sz.width / 8, sz.height / 8);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect r3(x, 1, 8 * x, y - 1);
            _roundedRect3.createRoundedRect(r3, 20, 1, td::LinePattern::Solid);
            _roundedRect3.drawFill(td::ColorID::SkyBlue); //bar za prikazivanje osobe i sl 

            gui::Rect r2(x, y, 8 * x, 7 * y - 1);
            _roundedRect1.createRoundedRect(r2, 20, 1, td::LinePattern::Solid);
            _roundedRect1.drawFill(td::ColorID::Transparent); //prostor u kojem se nalaze poruke  

            gui::Rect r1(x, 7 * y, 8 * x, 8 * y);
            _roundedRect2.createRoundedRect(r1, 20, 1, td::LinePattern::Solid);
            _roundedRect2.drawFill(td::ColorID::SkyBlue); //prostor u kojem se kucaju poruke

            gui::Circle cc(8 * x - x / 4, 7 * y + y / 2, 20);
            _shapeCircle.createCircle(cc, 15, td::LinePattern::Solid);
            _shapeCircle.drawFill(td::ColorID::MediumBlue); //dugme za slanje poruke 

            //slobodno mijenjati boje po zelji, ja sam odabrao random boje

        }
>>>>>>> Stashed changes

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }
      
        
      
        // pogled za SAO ------ grupa 1
        else if (Globals::isSAO && brojPoruke > -1) {
            DrawChatInfoSAO();
        }



        // pogled za studenta ------ grupa 2 ------ ako ne koristite mozete samo ostaviti da bude nacrtan etf znak
        else if (Globals::_currentUserID == 5) {
            gui::Size sz;
            getSize(sz);
            gui::Point cp(sz.width / 2, sz.height / 2);
            td::INT4 x = cp.x;
            td::INT4 y = cp.y;

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
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

            gui::Rect imgRect(x - 15 - x / 4, y - 15 - y / 4, x + 15 + x / 4, y + 15 + y / 4);
            _etf.draw(imgRect, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center);
        }

    };




    void SetMessageNumSAO(td::INT4 br) {
        brojPoruke = br;
        reset();
    };

    void DrawChatInfoSAO() {

        enableResizeEvent(true);

        gui::Size sz;
        getSize(sz);

        dp::IDataSet* pDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.Ime as Name, Korisnici.Prezime as Surname,SAOStudentTicket.Indeks as StudentIndex,"
            " SAOStudentTicket.Ticket_Tip as TypeOfTicket, SAOStudentTicket.Req_Title as TitleofTicket, SAOStudentTicket.Status_ID as Status_ID,SAOTicket_Status.Status as Status,"
            " SAOStudentTicket.Request as Request From Korisnici, SAOStudentTicket, SAOTicket_Status where Korisnici.Indeks=SAOStudentTicket.Indeks AND SAOTicket_Status.ID=SAOStudentTicket.Status_ID");

        dp::DSColumns cols(pDS->allocBindColumns(8));
        cols << "Name" << td::string8 << "Surname" << td::string8 << "StudentIndex" << td::string8 << "TypeOfTicket" << td::string8 << "TitleOfTicket" << td::string8 << "Status_ID" << td::int4 << "Status" << td::string8 << "Request" << td::string8;
        if (!pDS->execute())
        {
            pDS = nullptr;
            showAlert("errorReadingTable", "");
            return;
        }

        //showAlert("", std::to_string(brojPoruke));


        auto row = pDS->getRow(brojPoruke);

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
        reDraw();
    };

<<<<<<< Updated upstream
    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize.width = 1300;
        modelSize.height = 550;
=======
    void Reset(td::INT4 userID) {
        // id korisnika sa kojim je chat aktivan
        reDraw();
    };

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (Globals::isSAO && inputDevice.getModelPoint().x>rectBottomRight.left && inputDevice.getModelPoint().x < rectBottomRight.right && 
            inputDevice.getModelPoint().y<rectBottomRight.bottom && inputDevice.getModelPoint().y > rectBottomRight.top) {
            gui::Window* pParentWnd = getParentWindow();
            auto pWnd = new WindowCertainRequest(pParentWnd, indeks, Ime, prezime, tipKarte, status, request, title);
            pWnd->keepOnTopOfParent();
            pWnd->open();
        }

        reDraw();

    }
    bool getModelSize(gui::Size& modelSize) const override
    {
        //dodati da se dinamicki pomjera
        gui::Size sz;
        getSize(sz);

        if (Globals::isSAO)
        {
            modelSize.width = sz.width;

            modelSize.height = skrolV + 50;

            return true;
        }


        modelSize.width = sz.width; //
        modelSize.height = sz.height; // definitivno ne moze ovako - promijeniti
>>>>>>> Stashed changes
        return true;

    }

<<<<<<< Updated upstream
=======
    void measure(gui::CellInfo& ci)override {

        gui::Size sz;
        getSize(sz);

        ci.minHor = 500;
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

>>>>>>> Stashed changes
};

