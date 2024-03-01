#pragma once
#include "ViewTicket.h"
#include "ViewIDs.h"
#include "RequestUpdateWindow.h"
#include "WindowCertainRequest.h"

ViewTicket::ViewTicket()
	: _typelbl(tr("typeOfMessage"))
	, _subjectlbl(tr("subject"))
	, _bodylbl(tr(""))
	, _hlBtns(2)
	, _btnAttach("Dodaj")
	, _attachedFile("Dokument:")
	, _titleFile("")
	, _hlBtnsDB(6)
	, _btnSend(tr("send"), tr("sendRequestTT"))
	, _btnOpenOnHold(tr("open"), tr("openTT"))
	, _btnOpenAnswered(tr("open"), tr("openTT"))
	,_btnRefresh(tr("reload"))
	, _gl(9, 6)
	, _answeredticketslbl(tr("Answered tickets:"))
	, _OnholdticketsLbl(tr("On hold tickets:"))
	, _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{
	 
	_hlBtnsDB.append(_btnRefresh);
	_hlBtnsDB.appendSpacer();
	_hlBtnsDB.append(_btnOpenAnswered);
	//_hlBtnsDB.append(_btnSend);

	_btnSend.setType(gui::Button::Type::Default);
	_btnRefresh.setType(gui::Button::Type::Constructive);

	gui::GridComposer gc(_gl);

	gc.appendRow(_typelbl);
	gc.appendCol(_typeCombo);
	gc.appendCol(_subjectlbl);
	gc.appendCol(_subject);

	gc.appendRow(_bodylbl);

	gc.appendRow(_body, 0);



	gc.appendRow(_attachedFile);
	gc.appendCol(_titleFile, 3);
	//gc.appendEmptyCols(1);
	gc.appendCol(_btnAttach, td::HAlignment::Right);
	gc.appendCol(_btnSend);

	gc.appendRow(_OnholdticketsLbl);
	gc.appendRow(_tableTickets, 0);

	gc.appendRow(_answeredticketslbl);
	gc.appendEmptyCols(4);
	gc.appendCol(_btnOpenOnHold);
	gc.appendRow(_answeredtickets, 0);

	gc.appendRow(_hlBtnsDB, 0);
	gui::View::setLayout(&_gl);

	_db = dp::getMainDatabase();

	populateTypeTicketCombo(_typeCombo);
	populateTableData();
	_titleFile.setAsReadOnly();
	//_answeredtickets.setAsReadOnly();
	populateAnsweredtickets();

	_typeCombo.selectIndex(0);
}

bool ViewTicket::onChangedSelection(gui::ComboBox* pCmb)
{
	if (pCmb == &_typeCombo) {

		int type = _typeCombo.getSelectedIndex();

		switch (type) {
		case 0: _type.setValue(td::Variant("Upis")); break;
		case 1: _type.setValue(td::Variant("Poruka")); break;
		case 2: _type.setValue(td::Variant("Molba")); break;
		case 3: _type.setValue(td::Variant("�alba")); break;
		case 4: _type.setValue(td::Variant("Prijedlog")); break;
		}
		return true;
	}
	return false;
}

void ViewTicket::populateTypeTicketCombo(gui::ComboBox& combo)
{
	combo.addItem("Upit");
	combo.addItem("Poruka");
	combo.addItem("Molba");
	combo.addItem("Zalba");
	combo.addItem("Prijedlog");
}

bool ViewTicket::onClick(gui::Button* pBtn)
{
	if (pBtn == &_btnRefresh)
	{
		_tableTickets.reload();
		_answeredtickets.reload();

		return true;
	}

	if (pBtn == &_btnSend)
	{
		if (_subject.getText().cCompare("")==0)
		{
			showAlert(tr("error"), tr("Unesite naslov!"));
			return true;
		}


		showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSureTicket"), tr("Yes"), tr("No"));
		return true;
	}

	if (pBtn == &_btnAttach)
	{
		selectFiles();
		return true;
	}

	if (pBtn == &_btnOpenOnHold)
	{
		auto rowindex = _tableTickets.getFirstSelectedRow();

		if (rowindex < 0)
			return false;

		auto pomDS = _tableTickets.getDataSet();
		auto& row = pomDS->getRow(rowindex);

		td::String tipKarte = row[0].getConstStr();
		td::String naslov = row[1].getConstStr();
		gui::TextEdit body;
		body.setValue(row[3].getConstStr());
		td::INT4 tableID = row[4].i4Val();
		td::String indeks = row[5].getConstStr();

		gui::Window* pParentWnd = getParentWindow();
		auto pWnd = new RequestUpdateWindow(pParentWnd, tipKarte, naslov, body, tableID, indeks);
		pWnd->keepOnTopOfParent();
		pWnd->open();

		//UpdateTable();

		_tableTickets.reload();

		return true;
	}

	if (pBtn == &_btnOpenAnswered)
	{
		auto rowindex = _answeredtickets.getFirstSelectedRow();

		if (rowindex < 0)
			return false;

		auto pomDS = _answeredtickets.getDataSet();
		auto& row = pomDS->getRow(rowindex);

		td::String tipKarte = row[0].getConstStr();
		td::String naslov = row[1].getConstStr();
		gui::TextEdit body;
		body.setValue(row[3].getConstStr());
		td::INT4 ticketID = row[4].i4Val();
		td::String indeks = row[5].getConstStr();

		dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Korisnici.Ime AS name,Korisnici.Prezime AS sname FROM Korisnici WHERE Korisnici.ID=?");

		dp::Params parDS(pSelect->allocParams());
		parDS << Globals::_currentUserID;

		dp::Columns pCols = pSelect->allocBindColumns(2);
		td::String name, sname;
		pCols << "name" << name << "sname" << sname;
		if (!pSelect->execute()) {
			name = "Greska";
			sname = "Greska";
		}

		pSelect->moveNext();

		gui::Window* pParentWnd = getParentWindow();
		auto pWnd = new WindowCertainRequest(pParentWnd, ticketID, indeks, name, sname, tipKarte, row[2].getConstStr(), row[3].getConstStr(), naslov);
		pWnd->keepOnTopOfParent();
		pWnd->open();

		return true;
	}

	return false;
}

bool ViewTicket::sendTicket()
{

	if (!dp::getMainDatabase()->isConnected())
	{
		gui::Alert::show(tr("Error"), tr("Problem with database..."));
		return false;
	}

	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT COALESCE(MAX(ID), 0) as id FROM SAOStudentTicket");
	dp::Columns pCols = pSelect->allocBindColumns(1);
	td::INT4 id;
	pCols << "id" << id;
	pSelect->execute();
	pSelect->moveNext();

	dp::IStatementPtr pStatIns = dp::getMainDatabase()->createStatement("INSERT INTO SAOStudentTicket (ID, Indeks, Ticket_Tip, Req_Title, Request, Status_ID, Attachment, Name_attachment) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
	dp::Params parDS(pStatIns->allocParams());

	id++;
	td::String index, request, requestTitle, ticketName;
	td::INT4 status;
	parDS << id << dp::toNCh(index, 30) << dp::toNCh(ticketName, 30) << dp::toNCh(requestTitle, 100) << dp::toNCh(request, 5000) << status;

	td::Variant var;

	status = 1;
	index = GetStudentIndeks().getConstStr();
	_subject.getValue(var);
	requestTitle = var.getConstStr();
	request = _body.getText();
	ticketName = _typeCombo.getSelectedText();

	dp::Transaction tr(_db);

	if (!pStatIns->execute())
	{
		td::String errstr;
		pStatIns->getErrorStr(errstr);
		showAlert(errstr, "");
		return false;
	}
	tr.commit();
	_tableTickets.reload();

	return true;
}

void ViewTicket::populateAnsweredtickets() {


	td::String setstr = "SELECT SAOStudentTicket.Ticket_Tip as type, SAOStudentTicket.Req_Title as title, (SELECT SAOTicket_Status.Status as status FROM SAOTicket_Status WHERE SAOStudentTicket.Status_ID=SAOTicket_Status.ID) as status, SAOStudentTicket.Request as request, SAOStudentTicket.ID as reqID, SAOStudentTicket.Indeks as indeks  FROM SAOStudentTicket WHERE SAOStudentTicket.Status_ID=2 AND SAOStudentTicket.Indeks=";
	setstr.append(GetStudentIndeks().getConstStr());

	_pDSAnswered = dp::getMainDatabase()->createDataSet(setstr, dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDSAnswered->allocBindColumns(6));
	cols << "type" << td::string8 << "title" << td::string8 << "status" << td::string8 << "request" << td::string8 << "reqID" << td::int4 << "indeks" << td::string8;

	if (!_pDSAnswered->execute())
	{
		td::String errstr;
		_pDSAnswered->getErrorStr(errstr);
		showAlert(errstr, "U populateAnsweredTableDAta");
		_pDSAnswered = nullptr;
		return;
	}
	initTableAnswered();
}

void ViewTicket::initTableAnswered()
{
	gui::Columns visCols(_answeredtickets.allocBindColumns(3));
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("typeOfMessage"), "", 200) << gui::Header(1, tr("subject"), "", 200) << gui::Header(2, tr("Status:"), "", 200);
	_answeredtickets.init(_pDSAnswered);
	if (_pDSAnswered->getNumberOfRows()) {
		_tableTickets.selectRow(0, true);
	}
}



bool ViewTicket::sendTicketWithAttachment()
{
	if (!dp::getMainDatabase()->isConnected())
	{
		gui::Alert::show(tr("Error"), tr("Problem with database..."));
		return false;
	}

	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT COALESCE(MAX(ID), 0) as id FROM SAOStudentTicket");
	dp::Columns pCols = pSelect->allocBindColumns(1);
	td::INT4 id;
	pCols << "id" << id;
	pSelect->execute();
	pSelect->moveNext();

	dp::IStatementPtr pStatIns = dp::getMainDatabase()->createStatement("INSERT INTO SAOStudentTicket (ID, Indeks, Ticket_Tip, Req_Title, Request, Status_ID, Attachment, Name_attachment) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
	dp::Params parDS(pStatIns->allocParams());

	fo::fs::path filePathNow(_attachedFiles.last().c_str());
	td::String strFileName = filePathNow.filename().string();
	td::String fileExtension = filePathNow.filename().extension().string();

	td::BLOB::Type typeFile = td::BLOB::Type::TYPE_BINARY_UNKNOWN;
	if (fileExtension.compareConstStr(".txt"))
		typeFile = td::BLOB::Type::TYPE_TXT;
	else if (fileExtension.compareConstStr(".pdf"))
		typeFile = td::BLOB::Type::TYPE_PDF;
	else if (fileExtension.compareConstStr(".jpg"))
		typeFile = td::BLOB::Type::TYPE_JPG;
	else if (fileExtension.compareConstStr(".png"))
		typeFile = td::BLOB::Type::TYPE_PNG;

	td::BLOB dataIn(td::BLOB::SRC_FILE, 16384U, typeFile);

	id++;
	td::String index, request, requestTitle, ticketName;
	td::INT4 status;
	parDS << id << dp::toNCh(index, 30) << dp::toNCh(ticketName, 30) << dp::toNCh(requestTitle, 100) << dp::toNCh(request, 5000) << status << dataIn << dp::toNCh(strFileName, 50);

	if (!dataIn.setInFileName(filePathNow))
	{
		gui::Alert::show(tr("Error"), tr("Did you delete selected file?"));
		return true;
	}

	td::Variant var;

	status = 1;
	index = GetStudentIndeks().getConstStr();
	_subject.getValue(var);
	requestTitle = var.getConstStr();
	request = _body.getText();
	ticketName = _typeCombo.getSelectedText();

	dp::Transaction tr(_db);

	if (!pStatIns->execute())
		return false;

	tr.commit();
	_tableTickets.reload();

	_subject.setValue("");
	_body.setValue("");

	_titleFile.setTitle("");
	return true;
}

bool ViewTicket::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer) {
	if ((QuestionID)questionID == QuestionID::Save)
	{
		if (answer == gui::Alert::Answer::Yes) {
			if (_attachedFiles.isEmpty()) {
				sendTicket();
				showAlert(tr("successTicket"), tr("successTicketEE"));
			}
			else {
				sendTicketWithAttachment();
				showAlert(tr("successTicket"), tr("successTicketEE"));
				_attachedFiles.clean();
			}
		}
		return true;
	}
	return false;
}

void ViewTicket::selectFiles()
{
	auto pFD = new gui::OpenFileDialog(this, tr("OpenF"), { {tr("Tekst dokument"),"*.txt"}, {tr("PDF Dokument"),"*.pdf"}, {tr("JPG Slika"),"*.jpg"}, {tr("PNG Slika"),"*.png"} });

#ifdef USE_CALLBACKS
	pFD->openModal(&_callBackOpenFileDlg);
#else
	pFD->openModal([this](gui::FileDialog* pFD) {
		auto status = pFD->getStatus();
		if (status == gui::FileDialog::Status::OK) {

			_attachedFiles.push_back(pFD->getFileName());

			fo::fs::path filePathNow(_attachedFiles.last().c_str());
			td::String strFileName = filePathNow.filename().string();
			_titleFile.setTitle(strFileName);
			_titleFile.setBold();
		}
		});
#endif
}

td::Variant ViewTicket::GetStudentIndeks() {

	td::INT4 userid = Globals::_currentUserID;

	auto pomDS = dp::getMainDatabase()->createDataSet("SELECT Korisnici.ID as ID, Korisnici.Indeks as indeks FROM Korisnici WHERE ID!=0 AND ID!=-1", dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(pomDS->allocBindColumns(2));
	cols << "ID" << td::int4 << "indeks" << td::string8;

	if (!pomDS->execute())
	{
		pomDS = nullptr;
		return td::Variant("errorGettingIndex");
	}

	int n = pomDS->getNumberOfRows();

	for (int i = 0; i < n; i++) {

		auto& row = pomDS->getRow(i);
		if (row[0] == userid)
			return row[1];
	}

	return td::Variant("Index ne postoji u bazi");

}

void ViewTicket::initTable()
{
	gui::Columns visCols(_tableTickets.allocBindColumns(3));
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("typeOfMessage"), "", 200) << gui::Header(1, tr("subject"), "", 200) << gui::Header(2, tr("Status:"), "", 200);
	_tableTickets.init(_pDS);
	if (_pDS->getNumberOfRows()) {
		_tableTickets.selectRow(0, true);
	}
}

void ViewTicket::populateTableData()
{

	td::String setstr = "SELECT SAOStudentTicket.Ticket_Tip as type, SAOStudentTicket.Req_Title as title, (SELECT SAOTicket_Status.Status as status FROM SAOTicket_Status WHERE SAOStudentTicket.Status_ID=SAOTicket_Status.ID) as status, SAOStudentTicket.Request as request, SAOStudentTicket.ID as reqID, SAOStudentTicket.Indeks as indeks  FROM SAOStudentTicket WHERE SAOStudentTicket.Status_ID=1 AND SAOStudentTicket.Indeks=";
	setstr.append(GetStudentIndeks().getConstStr());

	_pDS = dp::getMainDatabase()->createDataSet(setstr, dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDS->allocBindColumns(6));
	cols << "type" << td::string8 << "title" << td::string8 << "status" << td::string8 << "request" << td::string8 << "reqID" << td::int4 << "indeks" << td::string8;

	if (!_pDS->execute())
	{
		td::String errstr;
		_pDS->getErrorStr(errstr);
		showAlert(errstr, "U populateTableDAta");
		_pDS = nullptr;
		return;
	}

	initTable();
}


void ViewTicket::UpdateTable() {

	td::String setstr = "SELECT SAOStudentTicket.Ticket_Tip as type, SAOStudentTicket.Req_Title as title, (SELECT SAOTicket_Status.Status as status FROM SAOTicket_Status WHERE SAOStudentTicket.Status_ID=SAOTicket_Status.ID)as status, SAOStudentTicket.Request as request, SAOStudentTicket.ID as reqID, SAOStudentTicket.Indeks as indeks  FROM SAOStudentTicket WHERE SAOStudentTicket.Indeks=";
	setstr.append(GetStudentIndeks().getConstStr());

	auto pompDS = dp::getMainDatabase()->createDataSet(setstr, dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(pompDS->allocBindColumns(6));
	cols << "type" << td::string8 << "title" << td::string8 << "status" << td::string8 << "request" << td::string8 << "reqID" << td::int4 << "indeks" << td::string8;
	if (!pompDS->execute())
	{
		pompDS = nullptr;
		showAlert("errorReadingTable", "");
		return;
	}


	_tableTickets.clean();


	size_t nRows = pompDS->getNumberOfRows();
	for (size_t i = 0; i < nRows; i++) {
		_tableTickets.beginUpdate();
		auto& rowpom = pompDS->getRow(i);
		auto& row = _tableTickets.getEmptyRow();
		row = rowpom;

		_tableTickets.push_back();

		_tableTickets.endUpdate();
	}

}
