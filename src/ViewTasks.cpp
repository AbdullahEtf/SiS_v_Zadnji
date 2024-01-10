#include "ViewTasks.h"
#include "SendMessage.h"
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <fo/FileOperations.h>
#include <td/BLOB.h>
#include <gui/Window.h>
#include "ViewIDs.h"

//#include "Globals.h"


ViewTasks::ViewTasks(td::INT4 SubjectID) :
    _db(dp::getMainDatabase()),
    _LblActName(tr("Activity")),
    _LblDateBegin(tr("ActivityDateB")),
    _LblTimeBegin(tr("ActivityTimeB")),
    _lblType(tr("ActivityNameForDateTime")),
    _type(td::int4),
    _lblCName(tr("Course:")),
    _lblTable2(tr("Docs:")),
    _btnAdd(tr("add"), tr("AddTT"))
    //, _btnUpdate(tr("Update"), tr("UpdateTT"))
    , _btnDelete(tr("Delete"), tr("DeleteTT"))
    , _btnSave(tr("Save"), tr("SaveTT"))
    , _btnAddFile(tr("AddFile"), tr("AddFileTT"))
    , _hlBtnsDB(6)
    , _gl(7, 4)
    , _SubjectID(SubjectID)
{


    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnDelete);
    // _hlBtnsDB.append(_btnUpdate); 
    _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.append(_btnAdd);
    _hlBtnsDB.appendSpacer();


    //  _btnUpdate.setType(gui::Button::Type::Default);
    _btnSave.setType(gui::Button::Type::Default);
    _btnDelete.setType(gui::Button::Type::Destructive);
    _btnAdd.setType(gui::Button::Type::Constructive);


    SetCurrentSubject();
    _cName.setAsReadOnly();

    gui::GridComposer gc(_gl);


    /*gc.appendRow(_LblActName);
      gc.appendCol(_Activity);*/
    gc.appendRow(_lblCName);
    gc.appendCol(_cName);
    gc.appendCol(_lblType);
    gc.appendCol(_type);

    gc.appendRow(_LblDateBegin);
    gc.appendCol(_dateB);

    gc.appendCol(_LblTimeBegin);
    gc.appendCol(_timeB);

    gc.appendRow(_btnAddFile);

    gc.appendRow(_table, 0);
    gc.appendRow(_lblTable2);
    gc.appendRow(_textEdit, 0);

    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);
    populateData();
    loadComboBox("select ID_Aktivnosti as ID, Naziv_Aktivnosti as Naziv From Aktivnosti where Tip_Aktivnosti=1 and ID_Predmeta=?", _type);

    /* if (_pDS->getNumberOfRows())
     {
         _table.selectRow(0, true);
     }*/
}
bool ViewTasks::loadComboBox(td::String select, gui::DBComboBox& combo)
{
    dp::IStatementPtr pSelect = _db->createStatement(select.c_str());
    dp::Params parDS(pSelect->allocParams());
    parDS << _SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String kod;
    td::INT4 id;
    pCols << "ID" << id << "Naziv" << kod;
    if (!pSelect->execute())
        return false;

    while (pSelect->moveNext())
    {
        combo.addItem(kod, id);
    }
    combo.selectIndex(0);
    return true;
}
void ViewTasks::populateData()
{
    auto pDB = dp::getMainDatabase();
    _pDS = pDB->createDataSet("SELECT a.Datum_Pocetka AS datumb,a.Vrijeme_Pocetka AS vrijemeb, a.Datum_Kraja AS datume, a.Vrijeme_Kraja AS vrijemee, a.Datum_Prijave AS datumf, a.Vrijeme_Prijave AS vrijemef,a.ID_Roka,a.ID_Aktivnosti,a.ID_Predmeta, b.Naziv_Aktivnosti FROM Rokovi a, Aktivnosti b where a.ID_Predmeta=? and b.Tip_Aktivnosti=1 and a.ID_Predmeta = b.ID_Predmeta and a.ID_Aktivnosti=b.ID_Aktivnosti", dp::IDataSet::Execution::EX_MULT);

    dp::Params parDS(_pDS->allocParams());
    //td::INT4 IDPredmeta = Globals::_IDSubjectSelection;

    //u parDS ce se ucitavati Globals::CurrentActivity
    parDS << _SubjectID;
    td::String Predmet;
    dp::DSColumns cols(_pDS->allocBindColumns(10));
    cols << "datumb" << td::date << "vrijemeb" << td::time << "datume" << td::date << "vrijemee" << td::time << "datumf" << td::date << "vrijemef" << td::time << "ID_Roka" << td::int4 << "ID_Aktivnosti" << td::int4 << "ID_Predmeta" << td::int4 << "Naziv_Aktivnosti" << td::string8;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    _table.init(_pDS, { 9, 0, 1, 2, 3, 4 ,5, 6 });
}
void ViewTasks::SetCurrentSubject() {
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
void ViewTasks::SetActivityName(td::Variant& val, td::INT4 br) {
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Aktivnosti FROM Aktivnosti WHERE ID_Aktivnosti=?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << br;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Aktivnost;
    pCols << "Naziv_Aktivnosti" << Aktivnost;
    if (!pSelect->execute()) {
        Aktivnost = "Haos";
    }
    if (!pSelect->moveNext())
        return;
    val = Aktivnost;
}

bool ViewTasks::onChangedSelection(gui::TableEdit* pTE) {
    if (pTE == &_table) {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(iRow);

        /*_Aktivnost.setValue(val);*/

        val = row[0];
        _dateB.setValue(val);

        val = row[1];
        _timeB.setValue(val);

        val = row[7];
        td::Variant var;
        _type.setValue(val);

        return true;
    }
    return false;
}

void ViewTasks::populateDSRow(dp::IDataSet::Row& row, td::INT4 i)
{
    td::Variant val;
    _dateB.getValue(val);
    row[0].setValue(val);

    _timeB.getValue(val);
    row[1].setValue(val);

    td::Variant x = i;
    row[6].setValue(x);

    /*val = _ActivityID;*/
    /*row[7].setValue(val);*/

    val = _SubjectID;
    row[8].setValue(val);
    td::Variant var;
    _type.getValue(val);
    row[7].setValue(val);
    td::INT4 a = val.i4Val();
    SetActivityName(var, a);
    row[9].setValue(var);

}
td::INT4 ViewTasks::findMaxID()
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID_Roka), 0) as maxid from Rokovi");
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


bool  ViewTasks::saveData()
{
    dp::Transaction tran(_db);
    if (!eraseDateTime())
        return false;

    if (!insertDateTime())
        return false;

    if (tran.commit())
    {
        _itemsToDelete.clear();
        _itemsToInsert.clear();
        _itemsToUpdate.clear();
    }
    return true;
}

//void ViewDateTimeActivity::saveData()
//{
//    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO  Rokovi ( ID_Rokovi, ID_Aktivnosti, Datum_Pocetka, Vrijeme_Pocetka, Datum_Kraja, Vrijeme_Kraja, Datum_Prijave, Vrijeme_Prijave, ID_Predmeta) VALUES(?,?,?,?,?,?,?)"));
//    dp::Params parDS(pInsStat->allocParams());
//    dp::Transaction tr(dp::getMainDatabase());
//
//    td::Date dateb;
//    td::Time tb;
//    td::Date datee;
//    td::Time te;
//    td::Date datef;
//    td::Time tf;
//   
//    td::Variant val;
//    parDS <<dateb << tb << datee << te << datef << tf;
//    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Rokovi"));
//    if (!pDel->execute())
//        return;
//    size_t nRows = _pDS->getNumberOfRows();
//    for (size_t i = 0; i < nRows; ++i)
//    {
//        auto row = _pDS->getRow(i);
//        dateb = row[0];
//        tb = row[1];
//        datee = row[2];
//        te = row[3];
//        datef = row[4];
//        tf = row[5];
//       
//
//        if (!pInsStat->execute())
//            return;
//    }
//    tr.commit();
//    return;
//}

bool ViewTasks::canAdd()
{
    /*Vrsiti log provjeru vremena da li je vrijeme pocetka ispita u budnucnosti i da li manje od vremena kraj ispita vrijeme prijave vece od tren i prije pocetka
    vrijeme ot prijave*/
    return true;
}

bool ViewTasks::eraseDateTime()

{
    td::INT4 id;
    dp::IStatementPtr pDeleteItem(_db->createStatement("DELETE from Rokovi where ID_Roka=?"));
    dp::Params pParams2(pDeleteItem->allocParams());
    pParams2 << id;

    for (auto itd : _itemsToDelete)
    {
        ///* dp::IStatementPtr pDeleteItem(_db->createStatement("delete from Rokovi where ID_Roka=?"));
        // dp::Params pParams2(pDeleteItem->allocParams());
        // pParams2 << id;*/
        id = itd;
        if (!pDeleteItem->execute())
        {
            return false;
        }
    }
    return true;
}

bool ViewTasks::insertDateTime()
{
    dp::IStatementPtr pInsertCourseP(_db->createStatement("insert into Rokovi (ID_Roka, ID_Aktivnosti, Datum_Pocetka, Vrijeme_Pocetka, Datum_Kraja, Vrijeme_Kraja,Datum_Prijave, Vrijeme_Prijave,ID_Predmeta) values (?,?,?,?,?,?,?,?,?)"));
    dp::Params pParams2(pInsertCourseP->allocParams());
    td::INT4 id_roka, id_akt, id_pred;
    td::Date datump, datumk, datumf;
    td::Time vrijemep, vrijemek, vrijemef;

    pParams2 << id_roka << id_akt << datump << vrijemep << datumk << vrijemek << datumf << vrijemef << id_pred;

    dp::IDataSet* pDS = _table.getDataSet();
    auto rowCnt = pDS->getNumberOfRows();
    for (size_t iRow = 0; iRow < rowCnt; ++iRow)
    {

        auto& row = pDS->getRow(iRow);
        id_roka = row[6].i4Val();
        if (std::find(_itemsToInsert.begin(), _itemsToInsert.end(), id_roka) == _itemsToInsert.end())
            continue;

        td::Variant val;

        datump = row[0];


        vrijemep = row[1];

        datumk = row[2];


        vrijemek = row[3];


        datumf = row[4];

        vrijemef = row[5];



        id_pred = _SubjectID;

        val = row[7];

        id_akt = val.i4Val();

        if (!pInsertCourseP->execute())
        {
            return false;
        }
    }
    return true;
}



bool ViewTasks::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnDelete)
    {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true;
        /* if (!canDelete(iRow))
             return true;*/
             //-------------uzimamo id reda kojeg je potrebno obrisati
        td::INT4 itemid = getIDfromTable(iRow);

        _table.beginUpdate();
        _table.removeRow(iRow);
        _table.endUpdate();
        _itemsToDelete.push_back(itemid);

        //--------------ako brisemo predmet koji se nije sacuvan u bazi, ali se nalazi u vektorima obrisati ga iz njih
        _itemsToInsert.erase(std::remove(_itemsToInsert.begin(), _itemsToInsert.end(), itemid), _itemsToInsert.end());

        return true;
    }

    if (pBtn == &_btnAdd)
    {

        td::INT4 itemid = findMaxID();
        if (!canAdd())
            return true;
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row, itemid);
        _table.push_back();
        _table.endUpdate();

        _itemsToInsert.push_back(itemid);
        return true;
    }
    if (pBtn == &_btnAddFile) {
        gui::TextEdit* pTE = (*this).getTextEdit();
        bool isEmpty = pTE->isEmpty();
        if (!isEmpty)
        {
            showYesNoQuestionAsync(QuestionIDA::OpenFile, this, tr("Replace data"), tr("Text edit is not empty. Are you sure you want to replace it with new content?"), tr("Yes"), tr("No"));
        }
        else
            showOpenFileDialog();
        return true;
    }
    if (pBtn == &_btnSave) {
        showYesNoQuestionAsync(QuestionIDDDAAAA::Saveee, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));

        return true;
    }

    return false;
}
bool ViewTasks::doesItDexist(td::Date d, td::Time t)
{
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        if (row[0].dateVal() == d && row[1].timeVal() == t)
            return true;
    }
    return false;
}
td::INT4 ViewTasks::getIDfromTable(int rowID)
{
    dp::IDataSet* pDS = _table.getDataSet();
    auto& row = pDS->getRow(rowID);
    return row[6].i4Val();
}

void ViewTasks::openFile(gui::FileDialog* pFD)
{
    auto status = pFD->getStatus();
    if (status == gui::FileDialog::Status::OK)
    {
        td::String strFileName = pFD->getFileName();
        td::String strContent;

        if (fo::loadFileContent(strFileName, strContent))
        {
            gui::TextEdit* pTE = (*this).getTextEdit();
            pTE->setText(strContent);
        }
    }
}

void ViewTasks::saveFile(gui::FileDialog* pFD)
{
    auto status = pFD->getStatus();
    if (status == gui::FileDialog::Status::OK)
    {
        td::String strFileName = pFD->getFileName();
        gui::TextEdit* pTE = (*this).getTextEdit();
        td::String strContent = pTE->getText();
        fo::OutFile f;
        if (fo::createTextFile(f, strFileName))
            f << strContent;
        f.close();
    }
}



void ViewTasks::showOpenFileDialog()
{
    //create OpenFile dialog and open it
    auto pFD = new gui::OpenFileDialog(this, tr("OpenF"), { {tr("TxtDocs"),"*.txt"}, {tr("PDFDocs"),"*.pdf"}, {tr("JPGSlike"),"*.jpg"}, {tr("PNGSlike"),"*.png"} });
#ifdef USE_CALLBACKS
    pFD->openModal(&_callBackOpenFileDlg);
#else
    //pFD->openModalWithID(WndID::FileOpenDlg, this);
    pFD->openModal([this](gui::FileDialog* pFD)
        {
            auto status = pFD->getStatus();
            if (status == gui::FileDialog::Status::OK)
            {
                td::String strFileFullPath = pFD->getFileName();
                td::String strContent;

                if (fo::loadFileContent(strFileFullPath, strContent))
                {
                    gui::TextEdit* pTE = (*this).getTextEdit();
                    pTE->setText(strContent);

                    fo::fs::path homePath;
                    mu::getHomePath(homePath);
                    fo::fs::path testDBPath = (homePath / "other_bin/TestData/natGUITest/Test.db");

                    dp::IDatabasePtr pDB = dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3);
                    //dp::getMainDatabase() if I were connected to DB before

                    if (!pDB->connect(testDBPath.string().c_str()))
                    {
                        assert(false && "Nema baze...");
                        return;
                    }
                    //just for test, i always delete everything in the table, so it doesent get huge
                   /* dp::IStatementPtr pStatDel = pDB->createStatement("delete from TestBLOBAttachment");*/


                    dp::IStatementPtr pStatIns = pDB->createStatement("insert into DokumentiOpenPredaja(ID, Dokumenti, ID_Predaje) values(?, ?, ?)");
                    dp::Params paramsInsert(pStatIns->allocParams());

                    fo::fs::path filePath(strFileFullPath.c_str());
                    //td::String strFileName = filePath.filename().string();//daj mi naziv fajla
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


                    td::BLOB dataIn(td::BLOB::SRC_FILE, 16384U, typeFile);
                    static td::INT4 ID=1, ID_predaje=1;
                    paramsInsert <<ID <<dataIn<<ID_predaje;
                    ID++; ID_predaje++;
                    //Neophodno, sa ove lokacije (strFileFullPath) se uzima BLOB
                    if (!dataIn.setInFileName(strFileFullPath))
                    {
                        gui::Alert::show(tr("Error"), tr("Did you delete the selected file?"));
                        return;
                    }

                    dp::Transaction transaction(pDB);

                    //bool delOK = pStatDel->execute();
                    bool insOK = pStatIns->execute();

                    bool commitOK = transaction.commit();
                }
            }
        });
#endif
}

gui::TextEdit* ViewTasks::getTextEdit()
{
    return &_textEdit;
}


bool ViewTasks::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)//??
{
    if ((QuestionIDDDAAAA)questionID == QuestionIDDDAAAA::Saveee)
    {
        if (answer == gui::Alert::Answer::Yes) {
            saveData();

            showAlert(tr("succes"), tr("succesEE"));
            if (answer == gui::Alert::Answer::Yes) {
                saveData();
                //unutar button save se detektuje promjena i salje se poruka od sistema za sve studente
//sad bi trebao da bude poseban button za poruke na koji ce se otvoriti novi prozorcic koji ce prikazati tabelu Messages gdje je id korisnika jednak user id poruke
                std::vector<td::INT4> userIDs;
                //svim studentima
                dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID FROM KORISNICI WHERE PozicijaID=5");
                dp::Columns pCols = pSelect->allocBindColumns(1);
                td::INT4 id;
                pCols << "ID" << id;
                if (!pSelect->execute())
                    return false;
                while (pSelect->moveNext())
                {
                    userIDs.push_back(id);
                }
                td::String naslov = "Imate novi zadatak!";
                td::String poruka = "Registrovana je promjena iz predmeta ";
                poruka += _cName.getText();
                poruka += ".";
                MsgSender msg;
                msg.sendSystemMsgtoUsers(naslov, poruka, userIDs);
            }
        }
        return true;
    }
    return false;
}

bool ViewTasks::onAnswer2(td::UINT4 questionIDA, gui::Alert::Answer answer)
{
    if ((QuestionIDA)questionIDA == QuestionIDA::OpenFile)
    {
        if (answer == gui::Alert::Answer::Yes)
            showOpenFileDialog();
        return true;
    }
    return false;
}

bool ViewTasks::onClick(gui::FileDialog* pFD, td::UINT4 dlgID)
{
    if ((WndID)dlgID == WndID::FileOpenDlg)
    {
        openFile(pFD);
        return true;
    }
    else if ((WndID)dlgID == WndID::FileSaveDlg)
    {
        saveFile(pFD);
        return true;
    }
    return false;
}
