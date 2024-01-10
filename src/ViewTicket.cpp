#pragma once
#include "ViewTicket.h"
#include "ViewIDs.h"

ViewTicket::ViewTicket()
	: _typelbl(tr("typeOfMessage"))
	, _subjectlbl(tr("subject"))
	, _bodylbl(tr(""))
	, _hlBtns(2)
	, _btnAttach("Dodaj")
	, _attachedFile("Dokument:")
	, _titleFile("")
	, _hlBtnsDB(4)
	, _btnSend(tr("send"), tr("sendRequestTT"))
	, _gl(6, 6)
	, _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{

	_hlBtnsDB.appendSpacer();
	_hlBtnsDB.append(_btnOpen);
	_hlBtnsDB.append(_btnSend);

	_btnSend.setType(gui::Button::Type::Default);

	gui::GridComposer gc(_gl);

	gc.appendRow(_typelbl);
	gc.appendCol(_typeCombo);
	gc.appendCol(_subjectlbl);
	gc.appendCol(_subject);

	gc.appendRow(_bodylbl);

	gc.appendRow(_body, 0);

	gc.appendRow(_attachedFile);
	gc.appendCol(_titleFile, 3);
	gc.appendEmptyCols(1);
	gc.appendCol(_btnAttach, td::HAlignment::Right);

	gc.appendRow(_tableTickets, 0);

	gc.appendRow(_hlBtnsDB, 0);
	gui::View::setLayout(&_gl);

	_db = dp::getMainDatabase();

	populateTypeTicketCombo(_typeCombo);
	populateTableData();
	_titleFile.setAsReadOnly();
}

bool ViewTicket::onChangedSelection(gui::ComboBox* pCmb)
{
	if (pCmb == &_typeCombo) {

		int type = _typeCombo.getSelectedIndex();

		switch (type) {
		case 0: _type.setValue(td::Variant("Upis")); break;
		case 1: _type.setValue(td::Variant("Poruka")); break;
		case 2: _type.setValue(td::Variant("Molba")); break;
		case 3: _type.setValue(td::Variant("Žalba")); break;
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
	if (pBtn == &_btnSend)
	{
		showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSureTicket"), tr("Yes"), tr("No"));
		return true;
	}

	if (pBtn == &_btnAttach)
	{
		selectFiles();
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

	dp::IStatementPtr pStatIns = dp::getMainDatabase()->createStatement("INSERT INTO SAOStudentTicket (ID, Indeks, Ticket_Tip, Req_Title, Request, Status, Attachment, Name_attachment) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
	dp::Params parDS(pStatIns->allocParams());

	id++;
	td::String index, request, requestTitle, ticketName, status;
	parDS << id << dp::toNCh(index, 30) << dp::toNCh(ticketName, 30) << dp::toNCh(requestTitle, 100) << dp::toNCh(request, 5000) << dp::toNCh(status, 50);

	td::Variant var;

	status = tr("OnHold");
	index = GetStudentIndeks().getConstStr();
	_subject.getValue(var);
	requestTitle = var.getConstStr();
	request = _body.getText();
	ticketName = _typeCombo.getSelectedText();

	dp::Transaction tr(_db);

	if (!pStatIns->execute())
	{
		return false;
	}
	tr.commit();
	_tableTickets.reload();

	return true;
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

	dp::IStatementPtr pStatIns = dp::getMainDatabase()->createStatement("INSERT INTO SAOStudentTicket (ID, Indeks, Ticket_Tip, Req_Title, Request, Status, Attachment, Name_attachment) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
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
	td::String index, request, requestTitle, ticketName, status;
	parDS << id << dp::toNCh(index, 30) << dp::toNCh(ticketName, 30) << dp::toNCh(requestTitle, 100) << dp::toNCh(request, 5000) << dp::toNCh(status, 50) << dataIn << dp::toNCh(strFileName, 50);

	if (!dataIn.setInFileName(filePathNow))
	{
		gui::Alert::show(tr("Error"), tr("Did you delete selected file?"));
		return true;
	}

	td::Variant var;

	status = tr("OnHold");
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
				showAlert(tr("succes"), tr("succesEE"));
			}
			else {
				sendTicketWithAttachment();
				showAlert(tr("succes"), tr("succesEE"));
				_attachedFiles.clean();
			}
		}
		return true;
	}
	return false;
}

void ViewTicket::selectFiles()
{
	auto pFD = new gui::OpenFileDialog(this, tr("OpenF"), { {tr("TxtDocs"),"*.txt"}, {tr("PDFDocs"),"*.pdf"}, {tr("JPGSlike"),"*.jpg"}, {tr("PNGSlike"),"*.png"} });

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
	visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("typeOfMessage"), "", 610) << gui::Header(1, tr("subject"), "", 610) << gui::Header(2, tr("Status:"), "", 610);
	_tableTickets.init(_pDS);
	if (_pDS->getNumberOfRows()) {
		_tableTickets.selectRow(0, true);
	}
}

void ViewTicket::populateTableData()
{
	td::String setstr = "SELECT SAOStudentTicket.Ticket_Tip as type, SAOStudentTicket.Req_Title as title, SAOStudentTicket.Status, SAOStudentTicket.Request as request, SAOStudentTicket.ID as reqID, SAOStudentTicket.Indeks as indeks  FROM SAOStudentTicket WHERE SAOStudentTicket.Indeks=";
	setstr.append(GetStudentIndeks().getConstStr());

	_pDS = dp::getMainDatabase()->createDataSet(setstr, dp::IDataSet::Execution::EX_MULT);
	dp::DSColumns cols(_pDS->allocBindColumns(6));
	cols << "type" << td::string8 << "title" << td::string8 << "status" << td::string8 << "request" << td::string8 << "reqID" << td::int4 << "indeks" << td::string8;
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		showAlert("errorReadingTable", "");
		return;
	}


	initTable();
}


void ViewTicket::UpdateTable() {

	td::String setstr = "SELECT SAOStudentTicket.Ticket_Tip as type, SAOStudentTicket.Req_Title as title, SAOStudentTicket.Status, SAOStudentTicket.Request as request, SAOStudentTicket.ID as reqID, SAOStudentTicket.Indeks as indeks  FROM SAOStudentTicket WHERE SAOStudentTicket.Indeks=";
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