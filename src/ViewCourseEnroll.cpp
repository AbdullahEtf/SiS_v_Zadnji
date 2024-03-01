#pragma once
#include "ViewCourseEnroll.h"
#include "ViewIDs.h"



ViewCourseEnroll::ViewCourseEnroll() :
    _departlbl(tr("departmentUserL")),
    _department(td::int4),
    _semlbl(tr("semesterUserL")),
    _semester(td::int4),
    _namelbl(tr("nameUserL")),
    _surnamelbl(tr("surnameUserL")),
    _indexlbl(tr("indexUserL")),
    _enrolled(tr("enrolledtocourse")),
    _toBeEnrolled(tr("tobeenrolledtocourse")),
    _hlBtnsDB(8)
    //  , _btnSave(tr("Save"), tr("SaveTT"))
    , _btnReload(tr("Reload"), tr("ReloadTT"))
    , _btnEnroll(tr("Enroll"), tr("EnrollTT"))
    , _btnWithdraw(tr("Withdraw"), tr("WithdrawCourseTT"))
    , _gl(7, 6)
    , _courselbl(tr("courselbl"))
    , _course(td::int4)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
{

    //_hlBtnsDB.append(_btnSave);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReload);
    _hlBtnsDB.appendSpace(20);
    _hlBtnsDB.append(_btnWithdraw);
    _hlBtnsDB.append(_btnEnroll);
    //    _btnSave.setType(gui::Button::Type::Default);
    _btnEnroll.setType(gui::Button::Type::Constructive);
    _btnWithdraw.setType(gui::Button::Type::Destructive);


    gui::GridComposer gc(_gl);

    gc.appendRow(_departlbl);
    gc.appendCol(_department);
    gc.appendCol(_semlbl);
    gc.appendCol(_semesterCombo);
    gc.appendCol(_courselbl);
    gc.appendCol(_course);


    gc.appendRow(_namelbl);
    gc.appendCol(_name);
    gc.appendCol(_surnamelbl);
    gc.appendCol(_surname);
    gc.appendCol(_indexlbl);
    gc.appendCol(_index);

    gc.appendRow(_toBeEnrolled, 0);
    gc.appendRow(_tableStudents, 0);

    gc.appendRow(_enrolled, 0);
    gc.appendRow(_tableEnrolled, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl);

    _db = dp::getMainDatabase();

    populateDataForStudents();
    populateDataForEnrolledStudents();


    //initTableEnrolledStudents();

    populateDepartmentCombo(_department);
    populateSemesterCombo(_semesterCombo);
    populateCourseCombo(_course);

    _department.selectIndex(0);
    _semesterCombo.selectIndex(0);
    _course.selectIndex(0);

    _name.setAsReadOnly();
    _surname.setAsReadOnly();
    _index.setAsReadOnly();

    _enrolled.setBold();
    _toBeEnrolled.setBold();
}

void ViewCourseEnroll::initTableEnrolledStudents()
{
    gui::Columns visCols(_tableEnrolled.allocBindColumns(4));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("IDUser"), "", 465) << gui::Header(3, tr("indexUser"), "", 465) << gui::Header(1, tr("nameUser"), "", 465) << gui::Header(2, tr("surnameUser"), "", 465);
    _tableEnrolled.init(_pDS2);
    if (_pDS2->getNumberOfRows()) {
        _tableEnrolled.selectRow(0, true);
    }
}

void ViewCourseEnroll::initTableStudents()
{
    gui::Columns visCols(_tableStudents.allocBindColumns(4));
    visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("IDUser"), "", 465) << gui::Header(3, tr("indexUser"), "", 465) << gui::Header(1, tr("nameUser"), "", 465) << gui::Header(2, tr("surnameUser"), "", 465);
    _tableStudents.init(_pDS);
    if (_pDS->getNumberOfRows()) {
        _tableStudents.selectRow(0, true);
    }
}

void ViewCourseEnroll::populateDepartmentCombo(gui::DBComboBox& combo)
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID_Smjera,Naziv_Smjera FROM Smjer");
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String name;
    td::INT4 id;
    pCols << "ID_Smjera" << id << "Naziv_Smjera" << name;
    pSelect->execute();

    while (pSelect->moveNext())
    {
        combo.addItem(name, id);
    }
}


void ViewCourseEnroll::populateCourseCombo(gui::DBComboBox& combo, td::String smjer, td::String semestar)
{
    td::String str;
    str.append("SELECT p.ID_Predmeta, p.Naziv_Predmeta FROM Predmet p WHERE p.Semestar =");
    str.append(semestar);
    str.append(" AND p.ID_Smjera=(SELECT ID_Smjera FROM Smjer WHERE Naziv_Smjera='");
    str.append(_department.getSelectedText());
    str.append("')");

    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement(str);
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String name;
    td::INT4 id;
    pCols << "ID_Predmeta" << id << "Naziv_Predmeta" << name;
    pSelect->execute();

    while (pSelect->moveNext())
    {
        combo.addItem(name, id);
    }
    combo.selectIndex(0);
}


void ViewCourseEnroll::populateSemesterCombo(gui::ComboBox& combo)
{
    combo.addItem("I");
    combo.addItem("II");
    combo.addItem("III");
    combo.addItem("IV");
    combo.addItem("V");
    combo.addItem("VI");
}

void ViewCourseEnroll::populateDataForEnrolledStudents() {

    td::String str;

    str.append("SELECT Korisnici.ID AS IDUser,  Korisnici.Ime AS nameUser, Korisnici.Prezime AS surnameUser,  Korisnici.Indeks AS indexUser,  Upis.ID_Smjera AS departID,  Upis.Semestar AS semesterUser  FROM  Korisnici JOIN Upis ON Korisnici.Indeks = Upis.Indeks AND Upis.Semestar=3 JOIN UpisPredmeta ON UpisPredmeta.ID_Predmeta = Predmet.ID_Predmeta JOIN Predmet ON UpisPredmeta.ID_Predmeta = Predmet.ID_Predmeta WHERE UpisPredmeta.ID_Studenta=Korisnici.ID AND Predmet.Naziv_Predmeta = '' AND Korisnici.ID != 0 AND Korisnici.ID != -1");

    _pDS2 = dp::getMainDatabase()->createDataSet(str, dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS2->allocBindColumns(6));
    cols << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "indexUser" << td::string8 << "departID" << td::int4 << "semesterUser";
    if (!_pDS2->execute())
    {
        showAlert("Citanje DataEnrollStudents", "");
        _pDS2 = nullptr;
        return;
    }
    initTableEnrolledStudents();
}

void ViewCourseEnroll::populateDataForStudents()
{
    td::String str;

    str.append("SELECT Korisnici.ID AS IDUser,  Korisnici.Ime AS nameUser,  Korisnici.Prezime AS surnameUser,  Korisnici.Indeks AS indexUser,  Upis.ID_Smjera AS departID, Upis.Semestar AS semesterUser FROM Korisnici JOIN Upis ON Korisnici.Indeks = Upis.Indeks WHERE NOT EXISTS ( SELECT 1 FROM UpisPredmeta JOIN Predmet ON UpisPredmeta.ID_Predmeta = Predmet.ID_Predmeta WHERE UpisPredmeta.ID_Studenta = (SELECT Korisnici.ID FROM Korisnici JOIN Upis ON Upis.Indeks=Korisnici.Indeks) AND Predmet.Naziv_Predmeta = '') AND Korisnici.ID != 0 AND Korisnici.ID != -1");

    _pDS = dp::getMainDatabase()->createDataSet(str, dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(_pDS->allocBindColumns(6));
    cols << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "indexUser" << td::string8 << "departID" << td::int4 << "semesterUser" << td::int4;

    if (!_pDS->execute())
    {
        showAlert("Citanje DataForStudents", "");
        _pDS = nullptr;
        return;
    }
    initTableStudents();
}




bool ViewCourseEnroll::onChangedSelection(gui::TableEdit* pTE) {
    if (pTE == &_tableStudents) {
        int iRow = _tableStudents.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _tableStudents.getDataSet();
        auto& row = pDS->getRow(iRow);
        val = row[0];

        val = row[1];
        _name.setValue(val);

        val = row[2];
        _surname.setValue(val);

        val = row[3];
        _index.setValue(val);

        return true;
    }

    if (pTE == &_tableEnrolled) {
        int iRow = _tableEnrolled.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _tableEnrolled.getDataSet();
        auto& row = pDS->getRow(iRow);
        val = row[0];


        _name.setAsReadOnly(false);
        val = row[1];
        _name.setValue(val);

        val = row[2];
        _surname.setValue(val);

        val = row[3];
        _index.setValue(val);

        return true;
    }

    return false;
}

bool ViewCourseEnroll::onChangedSelection(gui::ComboBox* pCmb)
{
    // showAlert("","Changeovan selection semestra/odsjeka");
    _course.clean();
    td::INT4 depindex = _department.getSelectedIndex() + 1;
    td::INT4 semindex = _semesterCombo.getSelectedIndex() + 1;

    td::String smjer, semestar;

    smjer = std::to_string(depindex);
    semestar = std::to_string(semindex);

    populateCourseCombo(_course, smjer, semestar);


    if (pCmb == &_semesterCombo) {

        int semester = _semesterCombo.getSelectedIndex();

        switch (semester) {
        case 0: _semester.setValue(1); break;
        case 1: _semester.setValue(2); break;
        case 2: _semester.setValue(3); break;
        case 3: _semester.setValue(4); break;
        case 4: _semester.setValue(5); break;
        case 5: _semester.setValue(6); break;
        }

        return true;
    }

    return false;
}

bool ViewCourseEnroll::onChangedSelection(gui::DBComboBox* pCmb)
{
    if (pCmb != &_course) {
        _course.clean();

        td::INT4 depindex = _department.getSelectedIndex() + 1;
        td::INT4 semindex = _semesterCombo.getSelectedIndex() + 1;

        td::String smjer, semestar;

        smjer = std::to_string(depindex);
        semestar = std::to_string(semindex);

        populateCourseCombo(_course, smjer, semestar);
    }
    else {
        UpdateEnrollDataSet();
        UpdateStudentDataSet();
    }
    return true;
}



void ViewCourseEnroll::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val;

    _name.getValue(val);
    row[1].setValue(val);

    _surname.getValue(val);
    row[2].setValue(val);

    _department.getValue(val);
    row[4] = val.i4Val();


    val = _semesterCombo.getSelectedIndex() + 1;
    row[5] = val.i4Val();

}

bool ViewCourseEnroll::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnReload)
    {
        UpdateStudentDataSet();
        _tableStudents.selectRow(0, true);
        UpdateEnrollDataSet();
        _tableEnrolled.selectRow(0, true);

        return true;
    }

    if (pBtn == &_btnWithdraw)   //ako admin napravi gresku da moze azurirati
    {
        int iRow = _tableEnrolled.getFirstSelectedRow();

        td::String str;
        str.append("DELETE FROM UpisPredmeta WHERE ID_Studenta = (SELECT Korisnici.ID FROM Korisnici WHERE Korisnici.Indeks = ? ) AND ID_Predmeta = (SELECT ID_Predmeta FROM Predmet WHERE Naziv_Predmeta = '");
        str.append(_course.getSelectedText());
        str.append("')");

        dp::IStatementPtr pDel(_db->createStatement(str));
        dp::Params parDS(pDel->allocParams());
        parDS << dp::toNCh(_tableEnrolled.getCurrentRow()[3], 30);

        if (!pDel->execute())
        {
            showAlert("NE BRISEEE", "");
            return false;
        }

        if (iRow < 0)
            return true;
        _tableEnrolled.beginUpdate();
        _tableEnrolled.removeRow(iRow);
        _tableEnrolled.endUpdate();

        UpdateStudentDataSet();
        UpdateEnrollDataSet();

        return true;
    }

    if (pBtn == &_btnEnroll)
    {
        int iRow = _tableStudents.getFirstSelectedRow();

        if (iRow < 0)
            return true;

        auto& rowToBeDel = _tableStudents.getCurrentRow();
        //------ZA INSERT U BAZU------------------------------------------
        td::String str = "INSERT INTO UpisPredmeta (ID_Studenta,ID_Predmeta) VALUES (?,(SELECT Predmet.ID_Predmeta FROM Predmet WHERE Predmet.Naziv_Predmeta='";
        str.append(_course.getSelectedText());
        str.append("'))");

        dp::IStatementPtr pInsStat(_db->createStatement(str));
        dp::Params parDS(pInsStat->allocParams());
        td::INT4 idStudenta;
        parDS << idStudenta;

        dp::Transaction tr(_db);

        idStudenta = rowToBeDel[0].i4Val();

        if (!pInsStat->execute()) {

            td::String str1;
            pInsStat->getErrorStr(str1);
            showAlert(str1, "");
            return false;

        }

        tr.commit();
        //-----------------------------------------------------------------

        _tableEnrolled.beginUpdate();
        auto& row = _tableEnrolled.getEmptyRow();
        populateDSRow(row);
        row[3] = rowToBeDel[3];
        row[0] = rowToBeDel[0];
        _tableEnrolled.push_back();

        _tableEnrolled.endUpdate();
        _tableStudents.removeRow(iRow);

        UpdateStudentDataSet();
        UpdateEnrollDataSet();

        return true;

    }

    //if (pBtn == &_btnSave)
    //{
    //    showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSure"), tr("Yes"), tr("No"));
    //    return true;
    //}

    return false;
}

//bool ViewCourseEnroll::saveEnrolls()
//{
//    td::String str;
//
//    str.append("INSERT INTO UpisPredmeta (ID_Studenta, ID_Predmeta)  SELECT (SELECT Korisnici.ID FROM Korisnici WHERE Korisnici.Indeks = ? ), (SELECT ID_Predmeta FROM Predmet WHERE Naziv_Predmeta = '");
//    str.append(_course.getSelectedText());
//    str.append("') WHERE NOT EXISTS ( SELECT 1 FROM UpisPredmeta WHERE ID = (SELECT Korisnici.ID FROM Korisnici WHERE Korisnici.Indeks = ? ) AND ID_Predmeta = (SELECT ID_Predmeta FROM Predmet WHERE Naziv_Predmeta = '");
//    str.append(_course.getSelectedText());
//    str.append("'))");
//
//
//    //ovo je kada su svi updateovani
//    dp::IStatementPtr pInsStat(_db->createStatement(str));
//    dp::Params parDS(pInsStat->allocParams());
//    td::String index;
//    parDS << dp::toNCh(index, 30) << dp::toNCh(index,30);
//
//    dp::Transaction tr(_db);
//
//    size_t nRows = _pDS2->getNumberOfRows();
//    for (size_t i = 0; i < nRows; i++) {
//        auto row = _pDS2->getRow(i);
//        index = row[3];
//        if (!pInsStat->execute()) {
//             
//            td::String str1;
//            pInsStat->getErrorStr(str1);
//            showAlert(str1, "");
//            return false;
//
//        }
//    }
//    tr.commit();
//    return true;
//}

//bool ViewCourseEnroll::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer)
//{
//    if ((QuestionID)questionID == QuestionID::Save)
//    {
//        if (answer == gui::Alert::Answer::Yes) {
//            saveEnrolls();
//            showAlert(tr("succes"), tr("succesEE"));
//        }
//        return true;
//    }
//    return false;
//} 


void ViewCourseEnroll::UpdateEnrollDataSet() {

    td::String str;

    str.append("SELECT Korisnici.ID AS IDUser,  Korisnici.Ime AS nameUser, Korisnici.Prezime AS surnameUser,  Korisnici.Indeks AS indexUser,  Upis.ID_Smjera AS departID, "
        " Upis.Semestar AS semesterUser  FROM  Korisnici JOIN Upis ON Korisnici.Indeks = Upis.Indeks JOIN UpisPredmeta ON UpisPredmeta.ID_Predmeta="
        "(SELECT Predmet.ID_Predmeta FROM Predmet WHERE Predmet.Naziv_Predmeta='");
    str.append(_course.getSelectedText());
    str.append("')JOIN Predmet ON UpisPredmeta.ID_Predmeta = Predmet.ID_Predmeta WHERE UpisPredmeta.ID_Studenta=Korisnici.ID AND Korisnici.ID != 0 AND Korisnici.ID != -1");


    dp::IDataSetPtr pompDS = dp::getMainDatabase()->createDataSet(str, dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(pompDS->allocBindColumns(6));
    cols << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "indexUser" << td::string8 << "departID" << td::int4 << "semesterUser" << td::int4;
    if (!pompDS->execute())
    {
        showAlert("Citanje EnrollDataSet", "");
        pompDS = nullptr;
        return;
    }


    _tableEnrolled.clean();


    size_t nRows = pompDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; i++) {
        _tableEnrolled.beginUpdate();
        auto& rowpom = pompDS->getRow(i);
        auto& row = _tableEnrolled.getEmptyRow();
        row = rowpom;

        _tableEnrolled.push_back();

        _tableEnrolled.endUpdate();
    }

    int iRow = _tableEnrolled.getFirstSelectedRow();
    if (iRow < 0) {
        return;
    }
    td::Variant val;
    dp::IDataSet* pDS = _tableEnrolled.getDataSet();
    auto& row = pDS->getRow(iRow);
    val = row[0];

    val = row[1];
    _name.setValue(val);

    val = row[2];
    _surname.setValue(val);

    val = row[3];
    _index.setValue(val);


    /* if (_pDS2.getPtr() != nullptr) {
         size_t nRows = _pDS2->getNumberOfRows();
         for (size_t i = 0; i < nRows; i++) {
             _pDS2->removeRow(i);
         }

     }

     auto& row = _pDS2->getEmptyRow();

     nRows = pompDS->getNumberOfRows();
     for (size_t i = 0; i < nRows; i++) {
         auto& rowpom = pompDS->getRow(i);
            row =_pDS2->getEmptyRow();
         row = rowpom;

         _pDS2->push_back();
     }

     td::Variant var;

     var = row[2];

     td::String str1 = var.getConstStr();

     showAlert(str1, "");*/

     /*  _pDS2 = pompDS;

       auto& row = _pDS2->getCurrentRow();

       td::Variant var;

       var = row[2];

       td::String str1 = var.getConstStr();

       showAlert(str1, ""); */





}


void ViewCourseEnroll::UpdateStudentDataSet() {

    if (_course.getSelectedIndex() == -1)
    {
        _tableStudents.clean();
        return;
    }

    //td::String str;

    //str.append("SELECT Korisnici.ID AS IDUser,  Korisnici.Ime AS nameUser,  Korisnici.Prezime AS surnameUser,  Korisnici.Indeks AS indexUser,  Upis.ID_Smjera AS departID,"
    //    " Upis.Semestar AS semesterUser FROM Korisnici JOIN Upis ON Korisnici.Indeks = Upis.Indeks WHERE NOT EXISTS"
    //    " ( SELECT 1 FROM UpisPredmeta JOIN Predmet ON UpisPredmeta.ID_Predmeta = Predmet.ID_Predmeta WHERE UpisPredmeta.ID_Studenta =Korisnici.ID AND Predmet.Naziv_Predmeta = '");
    //str.append(_course.getSelectedText());
    //str.append("') AND Korisnici.ID > 0");

    //dp::IDataSetPtr pompDS = dp::getMainDatabase()->createDataSet(str, dp::IDataSet::Execution::EX_MULT);
    //dp::DSColumns cols(pompDS->allocBindColumns(6));
    //cols << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "indexUser" << td::string8 << "departID" << td::int4 << "semesterUser" << td::int4;
    //if (!pompDS->execute())
    //{
    //    showAlert("Citanje StudentDataSet", "");
    //    pompDS = nullptr;
    //    return;
    //}



    td::String str1;

    str1.append("SELECT Korisnici.ID AS IDUser,  Korisnici.Ime AS nameUser,  Korisnici.Prezime AS surnameUser,  Korisnici.Indeks AS indexUser,  Upis.ID_Smjera AS departID,Upis.Semestar AS semesterUser FROM Korisnici JOIN Upis ON Korisnici.Indeks = Upis.Indeks WHERE Korisnici.ID>0 AND Upis.ID_Smjera = (SELECT ID_Smjera FROM Smjer WHERE Naziv_Smjera='");
    str1.append(_department.getSelectedText());
    str1.append("') AND Upis.Semestar =");
    str1.append(std::to_string(_semesterCombo.getSelectedIndex() + 1));

    dp::IDataSetPtr pompDS1 = dp::getMainDatabase()->createDataSet(str1, dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols1(pompDS1->allocBindColumns(6));
    cols1 << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "indexUser" << td::string8 << "departID" << td::int4 << "semesterUser" << td::int4;
    if (!pompDS1->execute())
    {
        showAlert("Citanje StudentDataSet1", "");
        pompDS1 = nullptr;
        return;
    }


    td::String str2;


    str2.append("SELECT Korisnici.ID AS IDUser FROM Korisnici JOIN UpisPredmeta ON Korisnici.ID = UpisPredmeta.ID_Studenta WHERE Korisnici.ID>0 "
        " AND UpisPredmeta.ID_Predmeta = (SELECT ID_Predmeta FROM Predmet WHERE Naziv_Predmeta='");
    str2.append(_course.getSelectedText());
    str2.append("')");

    dp::IDataSetPtr pompDS2 = dp::getMainDatabase()->createDataSet(str2, dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols2(pompDS2->allocBindColumns(1));
    cols2 << "IDUser" << td::int4;
    if (!pompDS2->execute())
    {
        //showAlert("Citanje StudentDataSet2", "");

        td::String err;

        pompDS2->getErrorStr(err);

        showAlert("", err);

        pompDS2 = nullptr;
        return;
    }



    td::String str;

    str.append("SELECT Korisnici.ID AS IDUser,  Korisnici.Ime AS nameUser,  Korisnici.Prezime AS surnameUser,  Korisnici.Indeks AS indexUser,  Upis.ID_Smjera AS departID,Upis.Semestar AS semesterUser FROM Korisnici JOIN Upis ON Korisnici.Indeks = Upis.Indeks WHERE Korisnici.ID>0 AND Upis.ID_Smjera = (SELECT ID_Smjera FROM Smjer WHERE Naziv_Smjera='");
    str.append(_department.getSelectedText());
    str.append("') AND Upis.Semestar =");
    str.append(std::to_string(_semesterCombo.getSelectedIndex() + 1));

    dp::IDataSetPtr pompDS = dp::getMainDatabase()->createDataSet(str, dp::IDataSet::Execution::EX_MULT);
    dp::DSColumns cols(pompDS->allocBindColumns(6));
    cols << "IDUser" << td::int4 << "nameUser" << td::string8 << "surnameUser" << td::string8 << "indexUser" << td::string8 << "departID" << td::int4 << "semesterUser" << td::int4;
    if (!pompDS->execute())
    {
        //showAlert("Citanje StudentDataSet2", "");

        td::String err;

        pompDS->getErrorStr(err);

        showAlert("", err);

        pompDS = nullptr;
        return;
    }


    pompDS->removeAll();


    size_t velupis = pompDS1->getNumberOfRows();
    // showAlert("", std::to_string(velupis));


    size_t velpredmet = pompDS2->getNumberOfRows();


    bool isFound = false;


    for (size_t i = 0;i < velupis;i++)
    {
        auto& row1 = pompDS1->getRow(i);

        for (size_t j = 0;j < velpredmet;j++)
        {
            auto& row2 = pompDS2->getRow(j);
            if (row1[0] == row2[0]) {
                isFound = true;
                break;
            }
        }

        if (isFound)
        {
            isFound = false;
            continue;
        }

        auto& row = pompDS->getEmptyRow();

        row = row1;

        pompDS->push_back();

    }








    _tableStudents.clean();


    size_t nRows = pompDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; i++) {
        _tableStudents.beginUpdate();
        auto& rowpom = pompDS->getRow(i);
        auto& row = _tableStudents.getEmptyRow();
        row = rowpom;

        _tableStudents.push_back();

        _tableStudents.endUpdate();
    }


    int iRow = _tableStudents.getFirstSelectedRow();
    if (iRow < 0) {
        return;
    }
    td::Variant val;
    dp::IDataSet* pDS = _tableStudents.getDataSet();
    auto& row = pDS->getRow(iRow);
    val = row[0];

    val = row[1];
    _name.setValue(val);

    val = row[2];
    _surname.setValue(val);

    val = row[3];
    _index.setValue(val);


}