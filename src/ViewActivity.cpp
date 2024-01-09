#pragma once
#include "ViewActivity.h"
#include "ViewIDs.h"
#include "Globals.h"
#include "WindowMessages.h"
#include "SendMessage.h"



//random edit za pokazivanje rada sa GitHubom


td::INT4 ViewActivity::findMaxID() // Eminina funkcija :-D
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID_Aktivnosti), 0) as maxid from Aktivnosti");
    dp::Columns pColumns = pSelect->allocBindColumns(1);
    td::INT4 maxID;
    pColumns << "maxid" << maxID;
    if (!pSelect->execute())
        return false;
    if (!pSelect->moveNext())
        return false;

    auto x = std::max_element(_actsToInsert.begin(), _actsToInsert.end());
    auto y = std::max_element(_actsToUpdate.begin(), _actsToUpdate.end());

    //provjera da li iteratori pokazuju na neku konkretnu vrijednost
    if (x == _actsToInsert.end() && y == _actsToUpdate.end())
        return ++maxID;
    if (x != _actsToInsert.end() && y == _actsToUpdate.end())
        if (maxID >= *x) return ++maxID;
        else {
            auto p1 = *x;
            return ++p1;
        }
    if (x == _actsToInsert.end() && y != _actsToUpdate.end())
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
//
//ViewActivity::ViewActivity() : _db(dp::getMainDatabase()) //ovaj konstruktor se ne treba koristiti.
////, _lblID(tr("id"))
//, _id(td::int4)
//, _lblName(tr("Course:"))
//, _idP(td::int4)
////, _lblIDP(tr("name"))
//, _lblType(tr("code"))
//, _type(td::int4)
//, _lblCName(tr("course"))
//, _lblDes(tr("description"))
//, _lblPoints(tr("points"))
//, _points(td::decimal2)// da se unosi kao bodivi umjesto procenti, ali da imaju decimalni dio. Samo pomnoziti sa 100.
//, _btnAdd(tr("add"))
//, _btnSave(tr("save"))
//, _btnReload(tr("Reload"))
////, _btnRemoveAll(tr("g3"))
//, _btnDelete(tr("Delete"))
//, _btnUpdate(tr("Update"))
//, _btnInsert(tr("Insert"))
//, _btnShowWindow(tr("Messages"))
////, _btnPushBack(tr("g3"))
//, _hlBtns(8) //nisam siguran koliko ce biti dugmadi
//, _gl(5, 4)
//, _paramFrom(0)
//, _paramTo(100)
////ne bi trebalo da jos ista fali
//{
//    setVisualID(View_ACTIVITY);
//    _hlBtns.appendSpace(4);
//    _hlBtns.append(_btnSave, td::HAlignment::Right);
//    _hlBtns.appendSpacer();
//    _hlBtns.append(_btnAdd, td::HAlignment::Right);
//    _hlBtns.append(_btnReload, td::HAlignment::Right);
//    ////_hlBtns.append(_btnRemoveAll, td::HAlignment::Right);
//    _hlBtns.append(_btnDelete, td::HAlignment::Right);
//    _hlBtns.append(_btnUpdate, td::HAlignment::Right);
//    _hlBtns.append(_btnInsert, td::HAlignment::Right);
//    ////_hlBtns.append(_btnPushBack, td::HAlignment::Right);
//    _btnSave.setAsDefault();
//
//    gui::GridComposer gc(_gl);
//    //gc.appendRow(_lblID);
//    gc.appendRow(_id);
//    gc.appendCol(_lblName);//ovdje umre i nece vise da ide dalje jer me ne voli program...
//    gc.appendCol(_name, 0);
//    gc.appendRow(_lblType);
//    gc.appendCol(_type);
//    gc.appendCol(_lblCName);
//    gc.appendCol(_cName);
//    gc.appendRow(_lblDes);
//    gc.appendCol(_desAct);
//    gc.appendCol(_lblPoints);
//    gc.appendCol(_points);
//
//    gc.appendRow(_table, 0);
//    gc.appendRow(_hlBtns, 0);
//    gui::View::setLayout(&_gl);
//
//    populateData();
//    loadComboBox("select ID as ID, Naziv as Naziv from VrstaAktivnosti", _type);
//}

ViewActivity::ViewActivity(td::INT4 SubjectID) : _db(dp::getMainDatabase()) //ovaj konstruktor se koristi jer je u switcheru
, _id(td::int4)
, _lblName(tr("Activity:"))
, _idP(SubjectID)
, _lblType(tr("ActivityType"))
, _type(td::int4)
, _lblCName(tr("Course:"))
, _lblDes(tr("ActivityDes:"))
, _lblPoints(tr("ActivityPoints"))
, _points(td::decimal2)// da se unosi kao bodivi umjesto procenti, ali da imaju decimalni dio. Samo pomnoziti sa 100.
, _btnAdd(tr("add"))
, _btnSave(tr("save"))
, _btnReload(tr("Reload"))
, _btnDelete(tr("Delete"))
, _btnUpdate(tr("Update"))
, _btnInsert(tr("Insert"))
, _hlBtns(8) //nisam siguran koliko ce biti dugmadi
, _gl(5, 4)
, _paramFrom(0)
, _paramTo(100)
//ne bi trebalo da jos ista fali
{
    setVisualID(View_ACTIVITY);
    _hlBtns.appendSpace(4);
    _hlBtns.append(_btnSave, td::HAlignment::Right);
    _hlBtns.appendSpacer();
    _hlBtns.append(_btnAdd, td::HAlignment::Right);
    _hlBtns.append(_btnReload, td::HAlignment::Right);
    _hlBtns.append(_btnDelete, td::HAlignment::Right);
    _hlBtns.append(_btnUpdate, td::HAlignment::Right);
    _hlBtns.append(_btnInsert, td::HAlignment::Right);
    _btnSave.setAsDefault();

    SetCurrentSubject();
    _cName.setAsReadOnly();

    gui::GridComposer gc(_gl);
    gc.appendRow(_lblCName);
    gc.appendCol(_cName,0);
    gc.appendRow(_lblName);
    gc.appendCol(_name);
    gc.appendCol(_lblType);
    gc.appendCol(_type);
    
    gc.appendRow(_lblDes);
    gc.appendCol(_desAct);
    gc.appendCol(_lblPoints);
    gc.appendCol(_points);

    
    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtns, 0);
    gui::View::setLayout(&_gl);

    populateData();
    loadComboBox("select ID as ID, Naziv as Naziv from VrstaAktivnosti", _type);
}

ViewActivity::~ViewActivity() {
   
}

void ViewActivity::SetCurrentSubject() {
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << _idP;
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



    void ViewActivity::populateData()// popunjava tabelu
    {
        auto pDB = dp::getMainDatabase();

        _pDS = _db->createDataSet("select a.ID_Aktivnosti, a.Naziv_Aktivnosti, p.Naziv_Predmeta, a.Procenat, a.Opis_Aktivnosti, v.Naziv ,a.Tip_Aktivnosti, a.ID_Predmeta from Aktivnosti a, Predmet p, VrstaAktivnosti v where p.ID_Predmeta = ? and p.ID_Predmeta = a.ID_Predmeta and a.Tip_Aktivnosti = v.ID", dp::IDataSet::Execution::EX_MULT);
        dp::Params params(_pDS->allocParams());
        params << _idP;

           //specify columns to obtain from the data provider
        dp::DSColumns cols(_pDS->allocBindColumns(8));
        cols << "ID_Aktivnosti" << td::int4
            << "Naziv_Aktivnosti" << td::string8
            << "Naziv_Predmeta" << td::string8
            << "Procenat" << td::decimal2
            << "Opis_Aktivnosti" << td::string8
            << "Naziv" << td::string8
            << "Tip_Aktivnosti" << td::int4
            << "ID_Predmeta" << td::int4;

        if (!_pDS->execute())
        {
            //show log
            _pDS = nullptr;
            return;
        }

        _table.init(_pDS, { 0,1,2,3,4,5 });
    }
    //---------------------------------------




    bool ViewActivity::eraseActivity() // testirano
    {
        td::INT4 id;
        dp::IStatementPtr pDeleteCust(_db->createStatement("delete from Aktivnosti where ID_Aktivnosti = ?"));
        dp::Params pParams2(pDeleteCust->allocParams());
        //pParams2 << type << id;
        pParams2 << id;

        for (auto item : _actsToDelete)
        {
            id = item;
            if (!pDeleteCust->execute())
            {
                return false;
            }
        }
        return true;
    }

    bool ViewActivity::updateActivity() // call me confused... ovo radi koliko sam ja uspio testirati, ali trebalo bi jos neko da provjeri
    {
        dp::IStatementPtr pChangeCourse(_db->createStatement("UPDATE Aktivnosti SET ID_Predmeta=?, Procenat=?, Naziv_Aktivnosti=?, Opis_Aktivnosti=?, Tip_Aktivnosti=? WHERE ID_Aktivnosti=?"));
        dp::Params pParams2(pChangeCourse->allocParams());
        td::INT4 idA, idP, tipP;  
        td::Decimal2 procenat;  
        td::String nameA, opisA, nameP; 
        pParams2 << idP << procenat << dp::toNCh(nameA, 100) << dp::toNCh(opisA, 1000);

        dp::IDataSet* pDS = _table.getDataSet();
        auto rowCnt = pDS->getNumberOfRows();
        for (size_t i = 0; i < rowCnt; ++i) 
        {
            auto& row = pDS->getRow(i); 
            idA = row[0].i4Val(); 
            if (std::find(_actsToInsert.begin(), _actsToInsert.end(), idA) == _actsToInsert.end()) 
                continue; 

            idP = _idP; 
            procenat = row[3].dec2Val(); 
            nameA = row[1]; 
            opisA = row[4]; 
            tipP = row[6].i4Val();

            if (!pChangeCourse->execute())
            {
                return false;
            }
        }
        return true;
    }

    bool ViewActivity::insertActivity()
    {
        dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Aktivnosti(ID_Aktivnosti, ID_Predmeta, Procenat, Naziv_Aktivnosti, Opis_Aktivnosti, Tip_Aktivnosti) values(?,?,?,?,?,?)"));
        dp::Params parDS(pInsStat->allocParams());
        td::INT4 idA, idP, tipP;
        td::Decimal2 procenat;
        td::String nameA, opisA;

        parDS << idA << idP << procenat << dp::toNCh(nameA, 30) << dp::toNCh(opisA, 30) << tipP;

        dp::IDataSet* pDS = _table.getDataSet();
        auto nRows = pDS->getNumberOfRows();
        for (size_t i = 0; i < nRows; ++i)
        {
            auto& row = pDS->getRow(i); 
            idA = row[0].i4Val(); 
            if (std::find(_actsToInsert.begin(), _actsToInsert.end(), idA) == _actsToInsert.end())
                continue;

            idP = _idP; 
            procenat = row[3].dec2Val(); 
            nameA = row[1]; 
            opisA = row[4]; 
            tipP = row[6].i4Val(); 

            if (!pInsStat->execute()) 
                return false;
        }

        return true;
    }
    //-------------------------------------------

    //-------------Pokusaj realizacije (koliko sam ja testirala rade, al bi bilo dobro da jos neko testira)// --------------------------
    bool ViewActivity::saveData()
    {
        dp::Transaction tran(_db);
        if (!eraseActivity())
            return false;

        if (!insertActivity())
            return false;

        if (!updateActivity())
            return false;

        if (tran.commit())
        {
            _actsToDelete.clear();
            _actsToInsert.clear();
            _actsToUpdate.clear();
        }
        return true;
    }
    //----------------Vjerovatno ce trebati modifikovati---------------------
    void ViewActivity::populateDSRow(dp::IDataSet::Row& row, td::INT4 i)
    {
        td::Variant val;
        td::Variant x = i;
        td::Variant var;
        row[0].setValue(x);

        _name.getValue(val);
        row[1].setValue(val);

        _cName.getValue(val);
        row[2].setValue(val);

        _points.getValue(val);
        row[3].setValue(val);
        _desAct.getValue(val);
        row[4].setValue(val);
        _type.getValue(val);
        row[6].setValue(val);
        td::INT4 a = val.i4Val();
        SetActivityTypeName(var, a);
        row[5].setValue(var);

    }
    //-----------------------------------------------
    void ViewActivity::SetActivityTypeName(td::Variant& val, td::INT4 br) 
    {
        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv FROM VrstaAktivnosti WHERE ID=?");     
        dp::Params parDS(pSelect->allocParams());          
        parDS << br;     
        dp::Columns pCols = pSelect->allocBindColumns(1);     
        td::String Aktivnost;     
        pCols << "Naziv" << Aktivnost;     
        if (!pSelect->execute()) 
        {         
            Aktivnost = "Haos";     
        }     
        if (!pSelect->moveNext())
            return;     
        val = Aktivnost; 
    }

    //-------Ova funkcija moze umjesto funkcije canAdd()------
    bool ViewActivity::doesIDexist(td::INT4 id) // redundant funkcija za provjeru... imaju dvije tri...
    {
        size_t nRows = _pDS->getNumberOfRows();
        for (size_t i = 0; i < nRows; ++i)
        {
            auto row = _pDS->getRow(i);
            if (row[0].i4Val() == id)
                return true;
        }
        return false;
    }
    //-------------------

    bool ViewActivity::existsInActivity(td::INT4 id) // redundant funkcija za provjeru... imaju dvije tri...
    {
        dp::IStatementPtr pSel(_db->createStatement("select count(1) as Cnt from Aktivnosti a where a.ID_Aktivnosti = ?"));
        dp::Params pParams(pSel->allocParams());
        pParams << id;

        dp::Columns pCols = pSel->allocBindColumns(1);
        td::INT4 cnt;
        pCols << "Cnt" << cnt;
        if (!pSel->execute())
            return false;
        pSel->moveNext();
        return (cnt > 0);
    }


    bool ViewActivity::onClick(gui::Button* pBtn) // dok sam pravio ova dugmad, maca mi se popela u krilo i pocela da grize dugmad na majci...
    {
        if (pBtn == &_btnReload)
        {
            _table.reload(); 
            _table.selectRow(0, true); 
            _actsToDelete.clear(); 
            _actsToInsert.clear(); 
            _actsToUpdate.clear(); 

            return true;
        }

        if (pBtn == &_btnDelete)
        {
            showYesNoQuestionAsync(tr("alertD"), tr("saveSure"), tr("Yes"), tr("No"), [this](gui::Alert::Answer answer)
                {
                    int iRow = _table.getFirstSelectedRow();
                    if (iRow < 0)
                        return true;

                    td::INT4 itemid = getIDfromTable(iRow);

                    _table.beginUpdate();
                    _table.removeRow(iRow);
                    _table.endUpdate();
                    _actsToDelete.push_back(itemid);

                    _actsToInsert.erase(std::remove(_actsToInsert.begin(), _actsToInsert.end(), itemid), _actsToInsert.end());
                    _actsToUpdate.erase(std::remove(_actsToUpdate.begin(), _actsToUpdate.end(), itemid), _actsToUpdate.end());
                });
            return true;
        }


        if (pBtn == &_btnUpdate)
        {
            int iRow = _table.getFirstSelectedRow(); 
            if (iRow < 0) 
                return true; 
            if (!canUpdate(iRow)) 
                return true; 
            td::INT4 itemid = getIDfromTable(iRow); 

            _table.beginUpdate(); 
            auto& row = _table.getCurrentRow(); 
            populateDSRow(row, itemid); 
            _table.updateRow(iRow); 
            _table.endUpdate(); 

            if (std::find(_actsToInsert.begin(), _actsToInsert.end(), itemid) == _actsToInsert.end())
                _actsToUpdate.push_back(itemid); 

            return true;
        }


        if (pBtn == &_btnInsert)
        {
            int iRow = _table.getFirstSelectedRow(); 
            if (iRow < 0) 
                iRow = 0; 
            if (!canAdd()) 
                return true; 

            td::INT4 itemid = getIDfromTable(iRow); 
            td::INT4 id = findMaxID(); 

            _table.beginUpdate(); 
            auto& row = _table.getEmptyRow(); 
            populateDSRow(row, itemid); 
            _table.insertRow(iRow); 
            _table.endUpdate(); 

            _actsToUpdate.erase(std::remove(_actsToUpdate.begin(), _actsToUpdate.end(), id), _actsToUpdate.end());
            _actsToInsert.push_back(id); 
            return true; 
        }


        if (pBtn == &_btnAdd)
        {
            td::INT4 itemid = findMaxID();
            td::Variant val;
            _id.getValue(val);
            if (doesIDexist(val.i4Val()))
            {
                showAlert(tr("alert"), tr("alertPB"));
                return true;
            }
            if (!canAdd())
                return true;

            _table.beginUpdate();
            auto& row = _table.getEmptyRow();
            populateDSRow(row, itemid);
            _table.push_back();
            _table.endUpdate();

            _actsToUpdate.erase(std::remove(_actsToUpdate.begin(), _actsToUpdate.end(), itemid), _actsToUpdate.end());
            _actsToInsert.push_back(itemid);
            return true;
        }


        if (pBtn == &_btnSave)//??
        {
            showYesNoQuestionAsync(QuestionIDDDAAA::Saveee, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));

            return true;
        }
        return false;
    }

    bool ViewActivity::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)//??
    {
        if ((QuestionIDDDAAA)questionID == QuestionIDDDAAA::Saveee)
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
                    td::String naslov = "Aktivnost!";
                    td::String poruka = "Registrovana je promjena za odredjenu aktivnost! ";
                    MsgSender msg;
                    msg.sendSystemMsgtoUsers(naslov, poruka, userIDs);
                }
            }
            return true;
        }
        return false;
    }

    bool ViewActivity::canAdd()// redundant funkcija za provjeru... imaju dvije tri...
    {
        td::Variant id = _id.getValue();
        td::Variant point = _points.getValue();
        td::Decimal2 x = point.dec2Val();
        if (x > 10000)
        {
            return false;
        }

        dp::IDataSet* pDS = _table.getDataSet();
        for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
        {
            auto& row = pDS->getRow(i);
            if (row[0] == id)
            {
                return false;
            }
        }

        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select sum(Procenat) as suma from Aktivnosti where ID_Predmeta = ?");
        dp::Params params(pSelect->allocParams());
        params << _idP;
        dp::Columns pCols = pSelect->allocBindColumns(1);
        td::Decimal2 sum;
        pCols << "suma" << sum;
        if (!pSelect->execute())
            return false;
        if (!pSelect->moveNext())
            return false;

        if (sum + x > 10000)
            return false;

        return true;
    }


    bool ViewActivity::onChangedSelection(gui::TableEdit* pTE)
    {

        if (pTE == &_table)
        {
            int iRow = _table.getFirstSelectedRow();
            if (iRow < 0)
            {
                _name.toZero();
                _cName.toZero();
                _points.toZero();
                _desAct.toZero();
                return true;
            }
            td::Variant val;
            dp::IDataSet* pDS = _table.getDataSet();
            auto& row = pDS->getRow(iRow);
            //val = row[0];
            //_id.setValue(val);
            val = row[1];
            _name.setValue(val);
            val = row[2];
            _cName.setValue(val);
            val = row[3];
            _points.setValue(val);
            val = row[4];
            _desAct.setValue(val);
            val = row[6];
            _type.setValue(val);

            return true;
        }
        return false;
    }

    bool ViewActivity::loadComboBox(td::String select, gui::DBComboBox& combo) //IT'S ALIVEEEEEEEEEEEEEEEEEEEEEEEEEEE
    {                                                                          
        dp::IStatementPtr pSelect = _db->createStatement(select.c_str());
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

    

    td::INT4 ViewActivity::getIDfromTable(int rowID)
    {
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(rowID);
        return row[0].i4Val();
    }

    bool ViewActivity::canDelete(int iRow) // ne iskoristeno ali trebalo bi da radi ako treba negdje implementirati...
    {
        //Za slucaj pokusaja brisanja nepostojece 
        return !doesIDexist(getIDfromTable(iRow));
    }

    bool ViewActivity::canUpdate(int iRow)
    {
        //Ne moze se dodati ista aktivnost
        td::Variant id = _id.getValue();
        td::INT4 idTable = getIDfromTable(iRow);

        if (idTable == id.i4Val())
        {
            return true;
        }
        //show msg - cant update
        return false;
    }

    