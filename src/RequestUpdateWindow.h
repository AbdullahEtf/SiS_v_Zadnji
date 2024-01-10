#pragma once
#include <gui/Window.h>
#include "ViewRequestUpdate.h"


class RequestUpdateWindow : public gui::Window
{
protected:
    ViewRequestUpdate* _request;
public:
    RequestUpdateWindow(gui::Window* parentWnd, td::String tipKarte, td::String naslov, gui::TextEdit body,td::INT4 id,td::String indeks,td::UINT4 wndID = 0)
        : gui::Window(gui::Size(800, 600), parentWnd, wndID)
    {
            _request = new ViewRequestUpdate(tipKarte, naslov, body, id, indeks);
            setTitle(tr("Ticket"));
            setCentralView(_request);
      
   }


   // void showOpenFileDialog();

    ~RequestUpdateWindow()
    {
    }
};



