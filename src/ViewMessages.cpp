#pragma once
#include "ViewMessages.h"
#include "ViewIDs.h"
#include "Globals.h"


ViewMessages::ViewMessages() : _db(dp::getMainDatabase())
, _lblName(tr("Name:"))
, _lblLastName(tr("Last Name:"))
, _lblIndex(tr("Index:"))
, _gl(2, 6)
{

    gui::GridComposer gc(_gl);
    gc.appendRow(_lblName);
    gc.appendCol(_name);
    gc.appendCol(_lblLastName);
    gc.appendCol(_lastname);
    gc.appendCol(_lblIndex);
    gc.appendCol(_index);

    gc.appendRow(_table, 0);
    gui::View::setLayout(&_gl);

    SetCurrentData();
    _name.setAsReadOnly();
    _lastname.setAsReadOnly();
    _index.setAsReadOnly();
    populateData();

    initTable(1);
}

ViewMessages::~ViewMessages()
{
}

void ViewMessages::SetCurrentData() {
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Ime, Prezime, Indeks FROM Korisnici WHERE ID = ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << Globals::_currentUserID;
    dp::Columns pCols = pSelect->allocBindColumns(3);
    td::String Ime, Prezime, Indeks;
    pCols << "Ime" << Ime<<"Prezime"<<Prezime<<"Indeks"<<Indeks;
    if (!pSelect->execute()) {
        Ime = "Greska";
        Prezime = "Greska";
        Indeks = "Greska";
    }
    while (pSelect->moveNext())
    {
        td::Variant val, val1, val2;
        val = Ime;
        val1 = Prezime;
        val2 = Indeks;
        _name.setValue(val);
        _lastname.setValue(val1);
        _index.setValue(val2);

    }

}
void ViewMessages::populateData() {
    auto pDB = dp::getMainDatabase();
    int currentUserID = Globals::_currentUserID;

    _pDS = _db->createDataSet(
        "SELECT a.ID, a.Subject, a.Poruke, a.AuthorID, p.Prezime, p.Ime, a.Datum, a.Vrijeme "
        "FROM Messages a "
        "JOIN Korisnici p ON p.ID = a.AuthorID "
        "JOIN MsgReceivers mr ON mr.MsgID = a.ID "
        "WHERE mr.UserID = ?",
        dp::IDataSet::Execution::EX_MULT);

    dp::Params params(_pDS->allocParams());
    params << currentUserID;

    // Specify columns to obtain from the data provider
    dp::DSColumns cols(_pDS->allocBindColumns(8));
    cols << "ID" << td::int4
        << "Subject" << td::string8
        << "Poruke" << td::string8
        << "AuthorID" << td::int4
        << "Prezime" << td::string8
        << "Ime" << td::string8
        << "Datum" << td::int4
        << "Vrijeme" << td::int4;

    if (!_pDS->execute())
    {
        // Show log
        _pDS = nullptr;
        return;
    }
}


void ViewMessages::initTable(int type)
{
    //-----------moguci razliciti nacini ispisa, estetski ljepse

    if (type == 0)
    {
        _table.init(_pDS, { 0,1,2,3,4,5,6,7});
    }
    else
    {
        gui::Columns visCols(_table.allocBindColumns(8));
        visCols
            << gui::Header(0, tr("ID"), tr("IDTT"), 50, td::HAlignment::Center)
            << gui::Header(1, tr("Subject"), tr("SubjectTT"), 200, td::HAlignment::Left)
            << gui::Header(2, tr("Poruke"), tr("PorukeTT"), 450, td::HAlignment::Left)
            << gui::Header(3, tr("AuthorID"), tr("AuthorIDTT"), 50, td::HAlignment::Center)
            << gui::Header(4, tr("Prezime"), tr("PrezimeTT"), 80, td::HAlignment::Center)
            << gui::Header(5, tr("Ime"), tr("ImeTT"), 80, td::HAlignment::Center)
            << gui::Header(6, tr("Datum"), tr("DatumTT"), 80, td::HAlignment::Center)
            << gui::Header(7, tr("Vrijeme"), tr("VrijemeTT"), 80, td::HAlignment::Center);
        //promijeniti izborni u status ako se promijeni ispis sa 0/1
        _table.init(_pDS);
    }


}
