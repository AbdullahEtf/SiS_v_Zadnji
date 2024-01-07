#pragma once
#include "ViewCertainRequest.h"
#include "ViewIDs.h"

//td::INT4 current_index=5542;

ViewCertainRequest::ViewCertainRequest(td::String ime,td::String prezime, td::String indeks, td::String tipKarte)
    : _lblstudentsTicket(tr("Student's ticket"))
    , _namelbl(tr("nameUser"))
    , _surnamelbl(tr("surnameUser"))
    , _indekslbl(tr("indexUser"))
    , _typeOfTicketlbl(tr("typeOfTicket"))
    , _hlBtnsDB(4)
    , _btnAnswer(tr("Answer"), tr("AnswerTT"))
    , _btnDownloadAttachment(tr("Download Attachment"), tr("Download AtachmentTT"))
    , _gl(5, 4)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
    , indeks(indeks)
{
    _hlBtnsDB.append(_btnAnswer);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnDownloadAttachment);

    _btnAnswer.setType(gui::Button::Type::Default);
    _btnDownloadAttachment.setType(gui::Button::Type::Default);

    gui::GridComposer gc(_gl);

    gc.appendRow(_lblstudentsTicket);
    gc.appendRow(_namelbl);
    gc.appendCol(_name);
    gc.appendCol(_surnamelbl);
    gc.appendCol(_surname);
    gc.appendRow(_indekslbl);
    gc.appendCol(_indeks);
    gc.appendCol(_typeOfTicketlbl);
    gc.appendCol(_typeOfTicket);
    //gc.appendRow(_tableStudent, 0);
    gc.appendRow(_tableTicket, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

    _db = dp::getMainDatabase();

    _name.setValue(ime);
    _surname.setValue(prezime);
    _indeks.setValue(indeks);
    _typeOfTicket.setValue(tipKarte);

    //populateDataForStudent();

    _name.setAsReadOnly();
    _surname.setAsReadOnly();
    _indeks.setAsReadOnly();
    _typeOfTicket.setAsReadOnly();

}


//void ViewCertainRequest::initTableStudent()
//{
//    gui::Columns visCols(_tableStudent.allocBindColumns(4));
//    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("nameUser"), "", 305) << gui::Header(1, tr("surnameUser"), "", 305) << gui::Header(2, tr("indexUser"), "", 305) << gui::Header(3, tr("typeOfTicket"), "", 305);
//    _tableStudent.init(_pDS);
//    if (_pDS->getNumberOfRows()) {
//        _tableStudent.selectRow(0, true);
//    }
//}
//
//void ViewCertainRequest::populateDataForStudent() {   //iz nekog razloga ova funkcija ne postavi vrijednosti za _name,_surname itd
//    td::String setstr= "SELECT k.Ime as Name, k.Prezime as Surname, k.Indeks as Ind, p.Ticket_Tip as TicketType FROM Korisnici k JOIN SAOStudentTicket p ON k.Indeks = p.Indeks WHERE k.Indeks=";
//    setstr.append(indeks);
//    td::Variant ime, prezime, ind, tipKarte;
//    dp::IStatement* pSelect= dp::getMainDatabase()->createStatement(setstr);
//    dp::Columns cols(pSelect->allocBindColumns(4));
//    cols << "Name" << ime << "Surname" << prezime << "Ind" << ind << "TicketType" << tipKarte;
//    if (!pSelect->execute())
//    {
//        pSelect= nullptr;
//        showAlert("errorReading", "");
//        return;
//    }
//    _name.setValue(ime);
//    _surname.setValue(prezime);
//    _indeks.setValue(indeks);
//    _typeOfTicket.setValue(tipKarte);
//}



