#pragma once
#include "ViewGradeLabHomework.h"
#include <td/Types.h>
#include "Reports.h"
#include "SendMessage.h"
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <fo/FileOperations.h>
#include <td/BLOB.h>
#include "Globals.h"
#include <utility>
#include <set>


ViewGradeLabHomework::ViewGradeLabHomework(td::INT4 SubjectID) : _db(dp::getMainDatabase())
, _lblName(tr("userName:"))
, _lblLName(tr("userLName:"))
, _lblIndex(tr("indeksUser:"))
, _lblGrade(tr("grade:")) // napomena za Eminu - poslije dodati send message
, _lblActivityName(tr("activityName:"))
, _activityName(td::int4) // gui::LineEdit::Messages::Send
, _lblCName(tr("courseName:"))
, _btnAdd(tr("add"))
, _btnSave(tr("save"))
, _btnDelete(tr("Delete"))
, _btnUpdate(tr("Update"))
, _btnReport(tr("Report"))
, _btnHWL(tr("Documents"))
, _hlBtns(5)
, _gl(6, 4) // pazi na brojeve----neka budu tri reda ovih labela (naziv aktivnosti i naziv predmeta, ime i prezime, indeks i ocjena)
, _SubjectID(SubjectID)
, _report(3)
, _imgHWGrades(":complex")

{
	_hlBtns.appendSpacer();
	_hlBtns.append(_btnDelete);
	// _hlBtnsDB.append(_btnUpdate); 
	_hlBtns.append(_btnSave);
	_hlBtns.append(_btnAdd);
	_hlBtns.append(_btnUpdate);
	_hlBtns.appendSpacer();

	_report.appendSpacer();
	_report.append(_btnHWL, td::HAlignment::Right);
	_report.append(_btnReport, td::HAlignment::Right);


	//  _btnUpdate.setType(gui::Button::Type::Default);
	_btnSave.setType(gui::Button::Type::Default);
	_btnDelete.setType(gui::Button::Type::Destructive);
	_btnAdd.setType(gui::Button::Type::Constructive);
	gui::GridComposer gc(_gl);

	SetCurrentSubject();
	_cName.setAsReadOnly();  // postavlja se u funkciji setcurrentsubject
	_name.setAsReadOnly();
	_lName.setAsReadOnly();
	_index.setAsReadOnly();

	gc.appendRow(_lblCName);
	gc.appendCol(_cName);

	gc.appendCol(_lblActivityName);
	gc.appendCol(_activityName);

	gc.appendRow(_lblName);
	gc.appendCol(_name);

	gc.appendCol(_lblLName);
	gc.appendCol(_lName);

	gc.appendRow(_lblIndex);
	gc.appendCol(_index);

	gc.appendCol(_lblGrade);
	gc.appendCol(_grade);

	gc.appendRow(_report, 0);


	gc.appendRow(_table, 0);

	gc.appendRow(_hlBtns, 0);
	gui::View::setLayout(&_gl);
	insertValues(_SubjectID);
	//populateData();
	td::String s = "select c.ID_Aktivnosti as ID, c.Naziv_Aktivnosti as Name, b.Datum_Predaje as d, b.Vrijeme_Predaje as t from OpenPredaja b, Aktivnosti c, VrstaAktivnosti d where (d.ID=2 OR d.ID=5) and d.ID = c.Tip_Aktivnosti and b.ID_Aktivnosti = c.ID_Aktivnosti and c.ID_Predmeta = ?";
	td::Date d(true);
	//----Adnan
		//----------------------------------dodati provjeru datuma i vremena: da li je kraj vremena predaje prosao u odnosu na trenutno vrijeme (ucitavati SAMO ako jeste)
		//-------------------logicko pitanje da li ucitavati naziv aktivnosti ili naziv iz tabele openpredaja????????---- mozes ostaviti i ovako kako je dato 
	//-----------------------popraviti i populatedsrow i mozda onChangedSelection
	// id_aktivnosti ucitati u varijablu _ActivityID da se moze koristiti za druge provjere - potrebno za ostatak koda
	// -------Emir
	//-------------------------dodati on finishedit za combobox i iz njega pozivati populateData 
	loadComboBox(s, _activityName);
	populateData();

	/*td::Variant val;
	_activityName.getValue(val);
	_actID.setValue(val, true);*/
	//onChangedSelection(&_table);
}

bool ViewGradeLabHomework::onChangedSelection(gui::DBComboBox* pCmb)
{
	if (pCmb == &_activityName) {
		td::Variant val;
		_activityName.getValue(val);
		_ActivityID = val.i4Val();
		_table.reload();
	//	_table.disableMessaging();
		onChangedSelection(&_table);
		//_table.enableMessaging();

	}
	return false;
}
void ViewGradeLabHomework::populateData()
{
	//-----Adnan
	//popraviti populate da ucitava studente koji su na aktivnosti izabranoj u comboboxu
	// za to ce trebati parDS ucitati jos jednu varijablu tj ucitati id_aktivnosti iz comboboxa - nista ostalo u selectu ne treba mijenjati
	_pDS = _db->createDataSet("SELECT d.ID_Korisnika, d.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime, d.Ocjena as Procenat, d.ID FROM Aktivnosti b, Korisnici c, OcjeneLabZadace d WHERE d.ID_Aktivnosti = b.ID_Aktivnosti and d.ID_Korisnika = c.ID AND b.ID_Predmeta = ? AND b.ID_Aktivnosti=? AND b.Tip_Aktivnosti IN(5, 2) ORDER BY b.Naziv_Aktivnosti DESC", dp::IDataSet::Execution::EX_MULT);

	dp::Params parDS(_pDS->allocParams());
	parDS << _SubjectID << _ActivityID;

	dp::DSColumns cols(_pDS->allocBindColumns(8));
	cols << "ID_Korisnika" << td::int4 << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8 << "Procenat" << td::string8 << "ID" << td::int4;

	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	_table.init(_pDS, { 4,5,3,2,6 });
}

bool ViewGradeLabHomework::onChangedSelection(gui::TableEdit* pTE)
{
	if (pTE == &_table) {
		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0) {
			return true;
		}
		td::Variant val;
		dp::IDataSet* pDS = _table.getDataSet();
		auto& row = pDS->getRow(iRow);

		/*_Aktivnost.setValue(val);*/

		val = row[6];
		_grade.setValue(val);

		val = row[4];
		_name.setValue(val);

		val = row[5];
		_lName.setValue(val);

		val = row[3];
		_index.setValue(val);
		//------------------popraviti
		val = row[1];
		_activityName.setValue(val, false);

		return true;
	}
	return false;
}

void ViewGradeLabHomework::populateDSRow(dp::IDataSet::Row& row, td::INT4 id)
{
	td::Variant val;
	_grade.getValue(val);
	row[6].setValue(val);

	_name.getValue(val);
	row[4].setValue(val);

	_lName.getValue(val);
	row[5].setValue(val);

	_index.getValue(val);
	row[3].setValue(val);

	_activityName.getValue(val);
	row[1].setValue(val);

	/*_cName.getValue(val);
	row[8].setValue(val);*/

	td::Variant x = id;
	row[7].setValue(x);


}
bool ViewGradeLabHomework::canAdd()
{
	// nema provjera za sada
	return true;
}

bool ViewGradeLabHomework::eraseLabHomeworkGrade()
{
	td::INT4 id;
	dp::IStatementPtr pDeleteGrade(_db->createStatement("update OcjeneLabZadace SET Ocjena = NULL where ID = ?"));
	dp::Params pParams2(pDeleteGrade->allocParams());
	pParams2 << id;

	for (auto itd : _itemsToDelete)
	{
		id = itd;
		if (!pDeleteGrade->execute())
		{
			// showAlert(tr("alert"), tr("alertDR")); ??? 
			return false;
		}
	}
	return true;

}

bool ViewGradeLabHomework::insertLabHomeworkGrade()
{
	dp::IStatementPtr pUpdateGrade(_db->createStatement("UPDATE OcjeneLabZadace SET Ocjena=? WHERE ID=?"));
	dp::Params pParams2(pUpdateGrade->allocParams());
	td::String grade;
	td::INT4 id;
	pParams2 << dp::toNCh(grade, 100) << id;

	dp::IDataSet* pDS = _table.getDataSet();
	auto rowCnt = pDS->getNumberOfRows();
	for (size_t iRow = 0; iRow < rowCnt; ++iRow)
	{
		auto& row = pDS->getRow(iRow);
		id = row[7].i4Val();
		if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), id) == _itemsToInsert.end())
			continue;

		grade = row[6];

		if (!pUpdateGrade->execute())
		{
			return false;
		}
	}
	return true;
}

bool ViewGradeLabHomework::updateLabHomeworkGrade()
{
	dp::IStatementPtr pUpdateGrade(_db->createStatement("UPDATE OcjeneLabZadace SET Ocjena=? WHERE ID=?"));
	dp::Params pParams2(pUpdateGrade->allocParams());
	td::String grade;
	td::INT4 id;
	pParams2 << dp::toNCh(grade, 100) << id;


	dp::IDataSet* pDS = _table.getDataSet();
	auto rowCnt = pDS->getNumberOfRows();
	for (size_t iRow = 0; iRow < rowCnt; ++iRow)
	{
		auto& row = pDS->getRow(iRow);
		id = row[7].i4Val();
		if (std::find(_itemsToUpdate.begin(), _itemsToUpdate.end(), id) == _itemsToUpdate.end())
			continue;

		grade = row[6];

		if (!pUpdateGrade->execute())
		{
			return false;
		}
	}
	return true;
}
bool ViewGradeLabHomework::saveData()
{
	dp::Transaction tran(_db);
	if (!eraseLabHomeworkGrade())
		return false;

	if (!insertLabHomeworkGrade())
		return false;

	if (!updateLabHomeworkGrade())
		return false;

	if (tran.commit())
	{
		_itemsToDelete.clear();
		_itemsToInsert.clear();
		_itemsToUpdate.clear();
	}

	for (auto i : _useractivityids) {


		td::String naslov = "Ocjena!";
		td::String poruka = "Unesena je ocjena ";
		dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("SELECT Ocjena FROM OcjeneLabZadace WHERE ID_Korisnika = ? AND ID_Aktivnosti = ?");
		dp::Params parDS1(pSelect2->allocParams());
		parDS1 << i.first << i.second;
		dp::Columns pCols1 = pSelect2->allocBindColumns(1);
		td::String ocjena;
		pCols1 << "Ocjena" << ocjena;
		if (!pSelect2->execute())
			return false;
		if (!pSelect2->moveNext())
			return false;
		td::String poruka3 = " iz predmeta ";

		dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
		dp::Params parDS(pSelect1->allocParams());
		parDS << _SubjectID;
		dp::Columns pCols = pSelect1->allocBindColumns(1);
		td::String naziv_predmeta;
		pCols << "Naziv_Predmeta" << naziv_predmeta;
		if (!pSelect1->execute())
			return false;
		if (!pSelect1->moveNext())
			return false;

		poruka += ocjena;
		poruka += poruka3;
		poruka += naziv_predmeta;
		MsgSender msg;
		msg.sendSystemMsgtoUser(naslov, poruka, i.first,1);

	}
	_useractivityids.clear();

	return true;
}

bool ViewGradeLabHomework::onClick(gui::Button* pBtn)
{
	if (pBtn == &_btnDelete)
	{

		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0)
			return true;
		//-------------uzimamo id reda kojeg je potrebno obrisati
		td::INT4 itemid = getIDfromTable(iRow);

		_table.beginUpdate();
		auto& row = _table.getCurrentRow();
		row[6].toZero();
		td::INT4 a = row[0].i4Val();
		//ovo je brisanje ako je uklonjena ocjena
		_useractivityids.erase({ a, _ActivityID });

		//	_table.updateRow(iRow);
		_table.endUpdate();
		onChangedSelection(&_table);

		_itemsToDelete.push_back(itemid);

		//--------------ako brisemo predmet koji se nije sacuvan u bazi, ali se nalazi u vektorima obrisati ga iz njih
		_itemsToInsert.erase(std::remove(_itemsToInsert.begin(), _itemsToInsert.end(), itemid), _itemsToInsert.end());
		_itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());

		return true;
	}
	if (pBtn == &_btnUpdate)
	{
		int iRow = _table.getFirstSelectedRow();
		if (iRow < 0)
			return true;
		td::INT4 itemid = getIDfromTable(iRow);

		_table.beginUpdate();
		auto& row = _table.getCurrentRow();
		populateDSRow(row, itemid);
		td::INT4 a = row[0].i4Val();
		_useractivityids.insert({ a, _ActivityID });

		_table.updateRow(iRow);
		_table.endUpdate();
		onChangedSelection(&_table);

		if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), itemid) == _itemsToInsert.end())
			_itemsToUpdate.push_back(itemid);

		return true;
	}
	if (pBtn == &_btnAdd)
	{
		int iRow = _table.getFirstSelectedRow();
		td::INT4 itemid = getIDfromTable(iRow);
		_table.beginUpdate();
		auto& row = _table.getCurrentRow();
		populateDSRow(row, itemid);
		td::INT4 a = row[0].i4Val();

		_useractivityids.insert({ a, _ActivityID });

		_table.updateRow(iRow);
		_table.endUpdate();
		onChangedSelection(&_table);

		_itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());
		_itemsToInsert.push_back(itemid);
		return true;
	}
	if (pBtn == &_btnSave) {
		showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));
		return true;
	}
	if (pBtn == &_btnReport) {
		dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Tip_Aktivnosti FROM Aktivnosti WHERE ID_Aktivnosti = ?");
		dp::Params parDS(pSelect->allocParams());
		//d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
		parDS << _ActivityID;
		dp::Columns pCols = pSelect->allocBindColumns(1);
		td::INT4 id;
		pCols << "Tip_Aktivnosti" << id;
		if (!pSelect->execute())
			return false;
		if (!pSelect->moveNext())
			return false;

		if(id==2)
			homeworkGrades(&_imgHWGrades, _SubjectID); 
		else if(id==5)
			labGrades(&_imgHWGrades, _SubjectID);
	}
	if (pBtn == &_btnHWL) {
		
		showOpenFileDialog();

	}
	return false;
}

void ViewGradeLabHomework::SetCurrentSubject()
{
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
	dp::Params parDS(pSelect->allocParams());
	//d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
	parDS << _SubjectID;
	dp::Columns pCols = pSelect->allocBindColumns(1);
	td::String Predmet;
	pCols << "Naziv_Predmeta" << Predmet;
	if (!pSelect->execute()) {
		Predmet = "Haos";
	}
	while (pSelect->moveNext())
	{
		td::Variant val;
		val = Predmet;
		_cName.setValue(val);

	}
}

td::INT4 ViewGradeLabHomework::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _table.getDataSet();
	auto& row = pDS->getRow(rowID);
	//---------------------pod pretpostavkom da je ucitano kao 0---provjeriti poslije
	return row[7].i4Val();
}

td::INT4 ViewGradeLabHomework::findMaxID()
{
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID), 0) as maxid from OcjeneLabZadace");
	dp::Columns pColumns = pSelect->allocBindColumns(1);
	td::INT4 maxID;
	pColumns << "maxid" << maxID;
	if (!pSelect->execute())
		return false;
	if (!pSelect->moveNext())
		return false;

	auto x = std::max_element(_itemsToInsert.begin(), _itemsToInsert.end());
	auto y = std::max_element(_itemsToUpdate.begin(), _itemsToUpdate.end());

	//provjera da li iteratori pokazuju na neku konkretnu vrijednost
	if (x == _itemsToInsert.end() && y == _itemsToUpdate.end())
		return ++maxID;
	if (x != _itemsToInsert.end() && y == _itemsToUpdate.end())
		if (maxID >= *x) return ++maxID;
		else {
			auto p1 = *x;
			return ++p1;
		}
	if (x == _itemsToInsert.end() && y != _itemsToUpdate.end())
		if (maxID >= *y) return ++maxID;
		else {
			auto p2 = *y;
			return ++p2;
		}
	auto p1 = *x;
	auto p2 = *y;
	if (maxID >= *x && maxID >= *y) return ++maxID;
	if (*x >= maxID && (*x) >= (*y)) return ++p1;
	if (*y >= maxID && (*y) >= (*x)) return ++p2;

	return ++maxID;
}

void ViewGradeLabHomework::insertValues(td::INT4 subjectID)
{
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("INSERT INTO OcjeneLabZadace (ID_Korisnika, ID_Aktivnosti) SELECT a.ID_Studenta, b.ID_Aktivnosti FROM UpisPredmeta a JOIN Aktivnosti b ON a.ID_Predmeta = b.ID_Predmeta WHERE b.Tip_Aktivnosti IN(2, 5)AND NOT EXISTS(SELECT 1 FROM OcjeneLabZadace c WHERE c.ID_Korisnika = a.ID_Studenta AND c.ID_Aktivnosti = b.ID_Aktivnosti); ");

	dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("INSERT INTO PolazniciAktivnosti (ID_Korisnika, ID_Aktivnosti) SELECT a.ID_Studenta, b.ID_Aktivnosti FROM UpisPredmeta a JOIN Aktivnosti b ON a.ID_Predmeta = b.ID_Predmeta WHERE b.Tip_Aktivnosti IN(2, 5)AND NOT EXISTS(SELECT 1 FROM PolazniciAktivnosti c WHERE c.ID_Korisnika = a.ID_Studenta AND c.ID_Aktivnosti = b.ID_Aktivnosti); ");
	dp::Transaction t(dp::getMainDatabase());
	if (!pSelect->execute())
		return;
	if (!pSelect2->execute())
		return;
	t.commit();
	/*if (!pSelect->moveNext())
		return;*/
}

bool ViewGradeLabHomework::loadComboBox(td::String select, gui::DBComboBox& combo)
{
	dp::IStatementPtr pSelect = _db->createStatement(select.c_str());
	dp::Params parDS(pSelect->allocParams());
	parDS << _SubjectID;
	dp::Columns pCols = pSelect->allocBindColumns(4);
	td::String name;
	td::INT4 id;
	td::Date d;
	td::Time t;
	td::Date dnow(true);
	td::Time tnow(true);

	pCols << "ID" << id << "Name" << name << "d" << d << "t" << t;
	if (!pSelect->execute())
		return false;

	while (pSelect->moveNext())
	{
		/*if (dnow > d)
			combo.addItem(name, id);
		else if (dnow == d && tnow > t)*/
			combo.addItem(name, id);

	}
	//combo.selectIndex(0, );
	return true;
}

void ViewGradeLabHomework::openFile(gui::FileDialog* pFD)
{
		auto status = pFD->getStatus();
		if (status == gui::FileDialog::Status::OK)
		{
	 		td::String strFileName = pFD->getFileName();
			td::String strContent;

			if (fo::loadFileContent(strFileName, strContent))
			{
			//	gui::TextEdit* pTE = (*this).getTextEdit();
				//pTE->setText(strContent);
			}
		}
}

void ViewGradeLabHomework::showOpenFileDialog()
{
	//create OpenFile dialog and open it
	//auto pFD = new gui::OpenFileDialog(this, tr("OpenF"), { {tr("TxtDocs"),"*.txt"}, {tr("PDFDocs"),"*.pdf"}, {tr("JPGSlike"),"*.jpg"}, {tr("PNGSlike"),"*.png"} });
#ifdef USE_CALLBACKS
	pFD->openModal(&_callBackOpenFileDlg);
#else

//------------------------------------------------------------------moveNext problem
	
		auto pFD = new gui::SelectFolderDialog(this, tr("SelectFolder"));

		pFD->openModal([this](gui::FileDialog* pFD)
			{
				auto status = pFD->getStatus();
				if (status == gui::FileDialog::Status::OK)
				{
					//folder gdje snimamo BLOB iz tabele u bazi
					td::String strFolderName = pFD->getFileName();

					fo::fs::path filePath(strFolderName.c_str());

					dp::IDatabase* pDB = dp::getMainDatabase();
					dp::IStatementPtr pStatSel = pDB->createStatement("SELECT a.NazivFajla as Name from Predaja a, OpenPredaja b where a.ID_Studenta = ? and b.ID_Aktivnosti = ? and b.ID = a.ID_OpenPredaja");
					dp::IStatementPtr pStatSelBlob = pDB->createStatement("SELECT a.Datoteka as Data from Predaja a, OpenPredaja b where a.ID_Studenta = ? and b.ID_Aktivnosti = ? and b.ID = a.ID_OpenPredaja");
					int iRow = _table.getFirstSelectedRow();
					if (iRow < 0) {
						return;
					}
					td::Variant val;
					dp::IDataSet* pDS = _table.getDataSet();
					auto& row = pDS->getRow(iRow);
					val = row[0];
					td::INT4 ID = val.i4Val();
					dp::Params pStatSelParams(pStatSel->allocParams());
					pStatSelParams << ID<<_ActivityID;
					dp::Params pStatSelBlobParams(pStatSelBlob->allocParams());
					pStatSelBlobParams << ID << _ActivityID;

					//daj mi ime fajla
					td::String fileName;
					dp::Columns colSelect(pStatSel->allocBindColumns(1));
					colSelect << "Name" << fileName;


					if (!pStatSel->execute())
					{
						return;
					}
					if (!pStatSel->moveNext())
					{
						return;
					}

					filePath /= fileName.c_str();

					td::String fullFilePath = filePath.string();

					td::String fileExtension = filePath.filename().extension().string(); //daj mi tip fajla

					//tip BLOBa
					td::BLOB::Type typeFile = td::BLOB::Type::TYPE_BINARY_UNKNOWN;
					if (fileExtension.compareConstStr(".txt"))
						typeFile = td::BLOB::Type::TYPE_TXT;
					else if (fileExtension.compareConstStr(".pdf"))
						typeFile = td::BLOB::Type::TYPE_PDF;
					else if (fileExtension.compareConstStr(".jpg"))
						typeFile = td::BLOB::Type::TYPE_JPG;
					else if (fileExtension.compareConstStr(".png"))
						typeFile = td::BLOB::Type::TYPE_PNG;

					td::BLOB BLOBout(td::BLOB::SRC_FILE, 16384U, typeFile);

					//select sada i snimi u dokument
					dp::Columns colSelect2(pStatSelBlob->allocBindColumns(1));
					colSelect2 << "Data" << BLOBout;



					if (!BLOBout.setOutFileName(fullFilePath))
					{
						//nije odabran folder?
						//ili vec postoji fajl sa ovim fullFilePath?        
						return;
					}
					if (!pStatSelBlob->execute())
					{
						return;
					}
					if (!pStatSelBlob->moveNext()) //------------------------------ovdje ne prolazi
					{
						return;
					}
					#ifdef MU_WINDOWS
						ShellExecute(NULL, "open", fullFilePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
					#endif

				}
			});

}
#endif


bool ViewGradeLabHomework::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)
{
	if ((QuestionID)questionID == QuestionID::Save)
	{
		if (answer == gui::Alert::Answer::Yes) {
			saveData();
			showAlert(tr("succes"), tr("succesEE"));
		}
		return true;
	}
	return false;
}
