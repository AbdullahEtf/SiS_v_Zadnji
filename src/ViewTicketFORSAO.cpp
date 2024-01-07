#pragma once
#include "ViewTicketForSAO.h"
#include "ViewIDs.h"
#include "WindowCertainRequest.h"

ViewTicketForSAO::ViewTicketForSAO()
	: _ticketslbl(tr("Received tickets from students: "))
	, _hlBtnsDB(2)
	, _btnOpen(tr("Open"), tr("OpenTT"))
	, _gl(3, 2)
	, _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{
	_ticketslbl.setBold();
	_hlBtnsDB.appendSpacer();
	_hlBtnsDB.append(_btnOpen);

	_btnOpen.setType(gui::Button::Type::Default);


	gui::GridComposer gc(_gl);

	gc.appendRow(_ticketslbl);
	gc.appendRow(_tickets, 0);
	gc.appendRow(_hlBtnsDB, 0);
	gui::View::setLayout(&_gl);
	_db = dp::getMainDatabase();

	populateTableData();
}

void ViewTicketForSAO::initTable()
{
	gui::Columns visCols(_tickets.allocBindColumns(6));
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("nameUser"), "", 305) << gui::Header(1, tr("surnameUser"), "", 305) << gui::Header(2, tr("indexUser"), "", 305) << gui::Header(3, tr("typeOfTicket"), "", 305) << gui::Header(4, tr("subject"), "", 305) << gui::Header(5, tr("status"), "", 305);
	_tickets.init(_pDS);
	if (_pDS->getNumberOfRows()) {
		_tickets.selectRow(0, true);
	}
       
}

void ViewTicketForSAO::populateTableData()
{
	td::String setstr = "SELECT Korisnici.Ime as Name, Korisnici.Prezime as Surname,SAOStudentTicket.Indeks as StudentIndex, SAOStudentTicket.Ticket_Tip as TypeOfTicket, SAOStudentTicket.Req_Title as TitleofTicket, SAOStudentTicket.Status as Status From Korisnici, SAOStudentTicket where Korisnici.Indeks=SAOStudentTicket.Indeks";
	_pDS = dp::getMainDatabase()->createDataSet(setstr, dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDS->allocBindColumns(6));
	cols << "Name" << td::string8 << "Surname" << td::string8 << "StudentIndex" << td::string8 << "TypeOfTicket" << td::string8 << "TitleOfTicket" << td::string8 << "Status" << td::string8;
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		showAlert("errorReadingTable", "");
		return;
	}
	initTable();
}

td::String ViewTicketForSAO :: getIndex()
{
	int iRow = _tickets.getFirstSelectedRow();
	/*if (iRow < 0) {
		return;
	}*/
	dp::IDataSet* pDS = _tickets.getDataSet();
	auto& row = pDS->getRow(iRow);

	td::Variant val= row[2];
	return val.strVal();

}
td::String ViewTicketForSAO::getName()
{
	int iRow = _tickets.getFirstSelectedRow();
	/*if (iRow < 0) {
		return;
	}*/
	dp::IDataSet* pDS = _tickets.getDataSet();
	auto& row = pDS->getRow(iRow);

	td::Variant val = row[0];
	return val.strVal();

}
td::String ViewTicketForSAO::getSurname()
{
	int iRow = _tickets.getFirstSelectedRow();
	/*if (iRow < 0) {
		return;
	}*/
	dp::IDataSet* pDS = _tickets.getDataSet();
	auto& row = pDS->getRow(iRow);

	td::Variant val = row[1];
	return val.strVal();

}
td::String ViewTicketForSAO::getTypeOfTicket()
{
	int iRow = _tickets.getFirstSelectedRow();
	/*if (iRow < 0) {
		return;
	}*/
	dp::IDataSet* pDS = _tickets.getDataSet();
	auto& row = pDS->getRow(iRow);

	td::Variant val = row[3];
	return val.strVal();

}
bool ViewTicketForSAO::onClick(gui::Button* pBtn)
{
	if (pBtn = &_btnOpen)
	{
		td::String indeks = getIndex();
		td::String ime = getName();
		td::String prezime = getSurname();
		td::String tipKarte = getTypeOfTicket();
		gui::Window* pParentWnd = getParentWindow();
		auto pWnd = new WindowCertainRequest(pParentWnd,indeks,ime,prezime,tipKarte);
		pWnd->keepOnTopOfParent();
		pWnd->open();
		return true;
	}
}