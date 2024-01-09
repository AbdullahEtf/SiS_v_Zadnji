#pragma once
#include "upload.h"
#include "ViewIDs.h"
#include <td/Types.h>
#include <gui/Window.h>
#include "MenuBar.h"
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <fo/FileOperations.h>
#include <td/BLOB.h>


td::INT4 ViewUpload::findMaxID() // Eminina funkcija :-D
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID_Predaje), 0) as maxid from Predaja");
    dp::Columns pColumns = pSelect->allocBindColumns(1);
    td::INT4 maxID;
    pColumns << "maxid" << maxID;
    if (!pSelect->execute())
        return false;
    if (!pSelect->moveNext())
        return false;

    return ++maxID;
}
ViewUpload::ViewUpload()
    : _lblName(tr("namee:")),

    _lblSurname(tr("surname:")),
    _lblIndeks(tr("indeksUser:")),

    _lblTable1(tr("Active:")),
    _lblTable2(tr("Committed:")),
    // _toBeEnrolled(tr("tobeenrolled")),
    _hlBtnsDB(4)
    , _btnDEnroll(tr("DEnroll"))
    , _btnReload(tr("Reload"))
    , _btnEnroll(tr("Enroll"))
    , _btnSend(tr("Send"))
    , _btnUpload(tr("Upload"))
    //, _btnWithdraw(tr("Withdraw"), tr("WithdrawTT"))
    , _gl(7, 6)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{

    //_hlBtnsDB.append(_btnDEnroll);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReload);
    // _hlBtnsDB.appendSpace(20);
    // _hlBtnsDB.append(_btnWithdraw);
    _hlBtnsDB.append(_btnSend);
    _hlBtnsDB.append(_btnUpload);
   _btnDEnroll.setType(gui::Button::Type::Default);
    _btnEnroll.setType(gui::Button::Type::Constructive);
    // _btnWithdraw.setType(gui::Button::Type::Destructive);


    gui::GridComposer gc(_gl);

    gc.appendRow(_lblIndeks);
   gc.appendCol(_indeks);
    gc.appendCol(_lblName);
    gc.appendCol(_name);
    gc.appendCol(_lblSurname);
    gc.appendCol(_surname);

    gc.appendRow(_lblTable1, 0);
    gc.appendRow(_table1, 0);

    gc.appendRow(_lblTable2, 0);
    gc.appendRow(_table2, 0);
    gc.appendRow(_textEdit, 0);
    gc.appendRow(_hlBtnsDB, 0);


    gui::View::setLayout(&_gl);

   _db = dp::getMainDatabase();
    populateDataForTable1();
    populateDataForTable2();


    _indeks.setValue("111111");
    _indeks.setAsReadOnly();

    _name.setValue("Mini");
    _name.setAsReadOnly();
    _surname.setValue("Mouse");
    _surname.setAsReadOnly();


    _lblTable1.setBold();
   _lblTable2.setBold();
}

ViewUpload::~ViewUpload()
{

}


void ViewUpload::populateDataForTable1()
{
    _pDS = dp::getMainDatabase()->createDataSet("SELECT P.Naziv_Predmeta, P.Sifra_Predmeta, A.Naziv_Aktivnosti, r.Datum_Kraja, r.Vrijeme_Kraja, P.ID_Predmeta FROM Predmet P, Aktivnosti A, Predaja pr, Rokovi r where P.ID_Predmeta = A.ID_Predmeta and r.ID_Predmeta = A.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and r.Datum_Kraja > 0 and pr.Predano = 0 and r.ID_Roka = pr.ID_Roka", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS->allocBindColumns(6));
    cols << "Naziv_Predmeta" << td::string8 
        << "Sifra_Predmeta" << td::string8 
        << "Naziv_Aktivnosti" << td::string8 
        << "Datum_Kraja" << td::date 
        << "Vrijeme_Kraja" << td::time
        /* << "Reg_time" << td::time */ 
        << "ID_Predmeta" << td::int4;;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    //initTable1();
    _table1.init(_pDS,0);
}
void ViewUpload::populateDataForTable2() // treba modifikovati select da radi kad se preda na zadnji dan da se uporedi vrijeme 
{
    _pDS2 = dp::getMainDatabase()->createDataSet("SELECT P.Naziv_Predmeta, P.Sifra_Predmeta, A.Naziv_Aktivnosti, pr.Datum_Predaje, pr.Vrijeme_Predaje, P.ID_Predmeta, pr.Datoteka FROM Predmet P, Aktivnosti A, Rokovi r, Predaja pr where  predano = 1 and P.ID_Predmeta = A.ID_Predmeta and r.ID_Predmeta = A.ID_Predmeta and (A.Tip_Aktivnosti = 2 or A.Tip_Aktivnosti = 5) and r.ID_Roka = pr.ID_Roka and r.Datum_Kraja > pr.Datum_Predaje", dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS2->allocBindColumns(7));
    cols << "Naziv_Predmeta" << td::string8 << "Sifra_Predmeta" << td::string8 << "Naziv_Aktivnosti" << td::string8 << "Datum_Predaje" << td::date << "Vrijeme_Predaje" << td::time << "ID_Predmeta" << td::int4  << "Datoteka" << td::string8; // treba vidjet koji tip podatka treba za datoteku

    if (!_pDS2->execute())
    {
       _pDS2 = nullptr;
        return;
   }
    //initTable2();

    _table2.init(_pDS2,0);
}


//bool ViewExamSignUp::saveData1()
//{
//    //ovo je kada su svi updateovani
//
//    dp::IStatementPtr pInsStat(_db->createStatement("UPDATE Polozeni_Predmeti SET Prijavljen = 1 WHERE ID_Studenta = 5 AND ID_Predmeta = ?"));
//
//    dp::Params parDS(pInsStat->allocParams());
//
//    parDS << ID_Pre1;
//
//
//    dp::Transaction tr(_db);
//
//
//
//    size_t nRows = _pDS2->getNumberOfRows();
//    for (size_t i = 0; i < nRows; i++) {
//        auto row = _pDS2->getRow(i);
//
//        if (!pInsStat->execute())
//            return false;
//
//    }
//    tr.commit();
//    return true;
//}
//
//bool ViewExamSignUp::saveData2()
//{
//    //ovo je kada su svi updateovani
//
//    dp::IStatementPtr pInsStat(_db->createStatement("UPDATE Polozeni_Predmeti SET Prijavljen = 0 WHERE ID_Studenta = 5 AND ID_Predmeta = ?"));
//
//    dp::Params parDS(pInsStat->allocParams());
//
//    parDS << ID_Pre2;
//
//
//    dp::Transaction tr(_db);
//
//
//
//    size_t nRows = _pDS2->getNumberOfRows();
//    for (size_t i = 0; i < nRows; i++) {
//        auto row = _pDS2->getRow(i);
//
//        if (!pInsStat->execute())
//            return false;
//
//    }
//    tr.commit();
//    return true;
//}
//
//
//
//bool ViewExamSignUp::onChangedSelection(gui::TableEdit* pTE)
//{
//    if (pTE == &_table2)
//    {
//        int iRow = _table2.getFirstSelectedRow();
//        if (iRow < 0)
//        {
//            _name.toZero();
//            return true;
//        }
//        // td::Variant val;
//        dp::IDataSet* pDS = _table2.getDataSet();
//        auto& row = pDS->getRow(iRow);
//
//        ID_Pre2 = row[6];
//
//        return true;
//    }
//
//    if (pTE == &_table1)
//    {
//        int iRow = _table1.getFirstSelectedRow();
//        if (iRow < 0)
//        {
//            _name.toZero();
//            return true;
//        }
//        // td::Variant val;
//        dp::IDataSet* pDS = _table1.getDataSet();
//        auto& row = pDS->getRow(iRow);
//
//        ID_Pre1 = row[6];
//
//        return true;
//    }
//    return false;
//}
//
//
//
//
bool ViewUpload::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnReload)
    {
        _table1.reload();
        _table1.selectRow(0, true);
        _table2.reload();
        _table2.selectRow(0, true);
        return true;
    }
    if (pBtn == &_btnSend) {

    }

    if (pBtn == &_btnUpload) {
        showOpenFileDialog();
    }

//    if (pBtn == &_btnEnroll)
//    {
//        saveData1();
//        _table1.reload();
//        _table1.selectRow(0, true);
//        _table2.reload();
//        _table2.selectRow(0, true);
//        return true;
//
//    }
//
//    if (pBtn == &_btnDEnroll)
//    {
//        saveData2();
//        _table1.reload();
//        _table1.selectRow(0, true);
//        _table2.reload();
//        _table2.selectRow(0, true);
//        return true;
//
//    }

    return false;

}

void ViewUpload::showOpenFileDialog()
{
    //create OpenFile dialog and open it
    auto pFD = new gui::OpenFileDialog(this, tr("OpenF"), { {tr("TxtDocs"),"*.txt"}, {tr("PDFDocs"),"*.pdf"}, {tr("JPGSlike"),"*.jpg"}, {tr("PNGSlike"),"*.png"} });

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
                    gui::TextEdit* pTE = &_textEdit;
                    pTE->setText(strContent);

                    fo::fs::path homePath;
                    mu::getHomePath(homePath);
                    fo::fs::path testDBPath = (homePath / "Work/CPProjects/SiS_v_Zadnji/Temp_Baza.db");

                    dp::IDatabasePtr pDB = dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3);
                    //dp::getMainDatabase() if I were connected to DB before

                    if (!pDB->connect(testDBPath.string().c_str()))
                    {
                        assert(false && "Nema baze...");
                        return;
                    }
                    ////just for test, i always delete everything in the table, so it doesent get huge
                    //dp::IStatementPtr pStatDel = pDB->createStatement("delete from TestBLOBAttachment");


                    dp::IStatementPtr pStatIns = pDB->createStatement("insert into Predaja(ID_Predaje, ID_Roka, ID_Studenta, Datoteka, Datum_Predaje, Vrijeme_Predaje, Predano) values(?, ?, ?, ?, ?, ?, ?)");
                    dp::Params paramsInsert(pStatIns->allocParams());

                    td::INT4 id, idr, ids, date, time, predano;
                    fo::fs::path filePath(strFileFullPath.c_str());
                    td::String strFileName = filePath.filename().string();//daj mi naziv fajla
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
                    id = findMaxID();
                    //dodati u tabeli u bazi naziv datoteke
                    paramsInsert << id << dp::toNCh(strFileName, 100) << dataIn;//prilagoditi da se slaze sa insertom

                    //Neophodno, sa ove lokacije (strFileFullPath) se uzima BLOB
                    if (!dataIn.setInFileName(strFileFullPath))
                    {
                        gui::Alert::show(tr("Error"), tr("Did you delete selected file?"));
                        return;
                    }

                    dp::Transaction transaction(pDB);

                    //bool delOK = pStatDel->execute();
                    bool insOK = pStatIns->execute();

                    bool commitOK = transaction.commit();
                }
            }
        });

}