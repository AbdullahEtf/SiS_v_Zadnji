#pragma once
#include "ViewGradeLabHomework.h"
#include <td/Types.h>
#include "Reports.h"
#include "SendMessage.h"


ViewGradeLabHomework::ViewGradeLabHomework(td::INT4 SubjectID) : _db(dp::getMainDatabase())
, _lblName(tr("userName:"))
, _lblLName(tr("userLName:"))
, _lblIndex(tr("indeksUser:"))
, _lblGrade(tr("grade:")) // napomena za Eminu - poslije dodati send message
, _lblActivityName(tr("activityName:"))
, _lblCName(tr("courseName:"))
, _btnAdd(tr("add"))
, _btnSave(tr("save"))
, _btnDelete(tr("Delete"))
, _btnUpdate(tr("Update"))
, _btnReport(tr("Report"))
, _hlBtns(5)
, _gl(6, 4) // pazi na brojeve----neka budu tri reda ovih labela (naziv aktivnosti i naziv predmeta, ime i prezime, indeks i ocjena)
, _SubjectID(SubjectID)
, _report(1)
, _imgHWGrades(":complex")

{

	_hlBtns.appendSpacer();
	_hlBtns.append(_btnDelete);
	// _hlBtnsDB.append(_btnUpdate); 
	_hlBtns.append(_btnSave);
	_hlBtns.append(_btnAdd);
	_hlBtns.append(_btnUpdate);
	_hlBtns.appendSpacer();

	_report.append(_btnReport, td::HAlignment::Right);

	//  _btnUpdate.setType(gui::Button::Type::Default);
	_btnSave.setType(gui::Button::Type::Default);
	_btnDelete.setType(gui::Button::Type::Destructive);
	_btnAdd.setType(gui::Button::Type::Constructive);
	gui::GridComposer gc(_gl);

	SetCurrentSubject();
	_cName.setAsReadOnly();  // postavlja se u funkciji setcurrentsubject

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
	populateData();
	onChangedSelection(&_table);
}

void ViewGradeLabHomework::populateData()
{

	_pDS = _db->createDataSet("SELECT a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime, d.Ocjena as Procenat, d.ID FROM PolazniciAktivnosti a, Aktivnosti b, Korisnici c, OcjeneLabZadace d WHERE a.ID_Aktivnosti = b.ID_Aktivnosti AND d.ID_Korisnika = a.ID_Korisnika AND d.ID_Aktivnosti = b.ID_Aktivnosti AND a.ID_Korisnika = c.ID AND b.ID_Predmeta = ? AND b.Tip_Aktivnosti IN(5, 2) ORDER BY b.Naziv_Aktivnosti DESC", dp::IDataSet::Execution::EX_MULT);

	dp::Params parDS(_pDS->allocParams());

	parDS << _SubjectID;
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

		val = row[2];
		_activityName.setValue(val);

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
	row[2].setValue(val);

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

	for (auto i : _userids) {

		td::String naslov = "Ocjena!";
		td::String poruka = "Unesena je ocjena za odredenu aktivnost! ";
		MsgSender msg;
		msg.sendSystemMsgtoUser(naslov, poruka, i);
	}
	_userids.clear();

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
		_userids.insert(a);
		//	_table.updateRow(iRow);
		_table.endUpdate();

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
		_userids.insert(a);
		_table.updateRow(iRow);
		_table.endUpdate();

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
		_userids.insert(a);
		_table.updateRow(iRow);
		_table.endUpdate();

		_itemsToUpdate.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());
		_itemsToInsert.push_back(itemid);
		return true;
	}
	if (pBtn == &_btnSave) {
		saveData();
	}
	if (pBtn == &_btnReport) {
		//gui::Image _imgExamGrades(":complex");
		labGrades(&_imgHWGrades, _SubjectID);
		//homeworkGrades(&_imgHWGrades, _SubjectID); //---------------------popraviti da se mogu dva sacuvati

		// pada zbog pristupa nedozvoljenim lokacijama - PROBLEM
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
	dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("INSERT INTO OcjeneLabZadace (ID_Korisnika, ID_Aktivnosti) SELECT a.ID_Korisnika, a.ID_Aktivnosti FROM PolazniciAktivnosti a WHERE NOT EXISTS( SELECT 1 FROM OcjeneLabZadace WHERE OcjeneLabZadace.ID_Korisnika = a.ID_Korisnika AND OcjeneLabZadace.ID_Aktivnosti = a.ID_Aktivnosti);");
	if (!pSelect->execute())
		return;
	if (!pSelect->moveNext())
		return;
}
