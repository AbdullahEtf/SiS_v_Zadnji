#pragma once
#pragma once
#include "ViewTicket.h"
#include "ViewIDs.h"

ViewTicket::ViewTicket()
	: _typelbl(tr("typeOfMessage"))
	, _subjectlbl(tr("subject"))
	, _bodylbl(tr(""))
	, _hlBtnsDB(2)
	, _btnSend(tr("send"),tr("sendRequestTT"))
	, _gl(5, 6)
	, _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{
	_hlBtnsDB.appendSpacer();
	_hlBtnsDB.append(_btnSend); 

	_btnSend.setType(gui::Button::Type::Default);
	//ok
	gui::GridComposer gc(_gl);

	gc.appendRow(_typelbl);
	gc.appendCol(_type);
	gc.appendCol(_subjectlbl);
	gc.appendCol(_subject);
	gc.appendRow(_bodylbl);
	gc.appendRow(_body,0);
	gc.appendRow(_tableTickets,0);
	gc.appendRow(_hlBtnsDB, 0);
	gui::View::setLayout(&_gl);

	_db = dp::getMainDatabase();

	populateTableData();
}



bool ViewTicket::onClick(gui::Button* pBtn)
{
	if (pBtn == &_btnSend)
	{
		showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSureTicket"), tr("Yes"), tr("No"));
		return true;
	}

	return false;
}



bool ViewTicket::sendTicket() {

	dp::IStatementPtr pInsStat(_db->createStatement("INSERT INTO SAOStudentTicket (Indeks,Ticket_Tip,Req_Title,Request,Status) VALUES (?,?,?,?,?)"));
	dp::Params parDS(pInsStat->allocParams());
	td::String index, request,requestTitle,Ticket_Name,status;
	parDS << dp::toNCh(index, 30) << dp::toNCh(Ticket_Name,30) <<dp::toNCh(requestTitle,100)<< dp::toNCh(request, 5000)<<dp::toNCh(status,50);


	td::Variant var;

	status = tr("OnHold");
	index = GetStudentIndeks().getConstStr();
	_subject.getValue(var);
	requestTitle = var.getConstStr();
	request =_body.getText();
	Ticket_Name =_type.getText();

	dp::Transaction tr(_db);

	if (!pInsStat->execute())
	{
		td::String error;

		pInsStat->getErrorStr(error);
		showAlert(error, "");

		return false;

	}
	//okej
	tr.commit();

	_tableTickets.reload();

	return true;

}

bool ViewTicket::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer) {
	if ((QuestionID)questionID == QuestionID::Save)
	{
		if (answer == gui::Alert::Answer::Yes) {
			sendTicket();
			showAlert(tr("succes"), tr("succesEE"));
		}
		return true;
	}
	return false;
}


td::Variant ViewTicket::GetStudentIndeks() {

	td::INT4 userid = Globals::_currentUserID;

	auto pomDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.ID as ID, Korisnici.Indeks as indeks FROM Korisnici WHERE ID!=0 AND ID!=-1", dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(pomDS->allocBindColumns(2));
	cols << "ID" << td::int4<<"indeks" << td::string8;

	if (!pomDS->execute())
	{
		pomDS = nullptr;
		return td::Variant("errorGettingIndex");
	}

	int n = pomDS->getNumberOfRows();

	for (int i = 0;i < n;i++) {

		auto& row = pomDS->getRow(i);
		if (row[0] == userid)
			return row[1];
	   }



	return td::Variant("Index ne postoji u bazi");

}

void ViewTicket::populateTableData() {

	td::String setstr = "SELECT SAOStudentTicket.Ticket_Tip as type, SAOStudentTicket.Req_Title as title, SAOStudentTicket.Status as status FROM SAOStudentTicket WHERE SAOStudentTicket.Indeks=";
	setstr.append(GetStudentIndeks().getConstStr());

	_pDS = dp::getMainDatabase()->createDataSet(setstr, dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDS->allocBindColumns(3));
	cols << "type" << td::string8 << "title" << td::string8 << "status" << td::string8;
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		showAlert("errorReadingTable", "");
		return;
	}
	//showAlert(std::to_string(_pDS->getNumberOfRows()), "");

	gui::Columns visCols(_tableTickets.allocBindColumns(3));
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("typeOfMessage"), "", 610) << gui::Header(1, tr("subject"), "", 610) << gui::Header(2, tr("Status:"), "", 610);
	  _tableTickets.init(_pDS);
		if (_pDS->getNumberOfRows()) {
			_tableTickets.selectRow(0, true);
             }
		
}
