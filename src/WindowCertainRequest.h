#pragma once
#include <gui/Window.h>
#include "ViewCertainRequest.h"


class WindowCertainRequest : public gui::Window
{
protected:
    ViewCertainRequest* _request;
public:
    WindowCertainRequest(gui::Window* parentWnd, td::String indeks,td::String ime,td::String prezime, td::String tipKarte, td::UINT4 wndID = 0)
        : gui::Window(gui::Size(800, 600), parentWnd, wndID)
    {
        _request = new ViewCertainRequest(ime,prezime,indeks,tipKarte);
        setTitle(tr("Ticket"));
        setCentralView(_request);
    }

    ~WindowCertainRequest()
    {
    }
};