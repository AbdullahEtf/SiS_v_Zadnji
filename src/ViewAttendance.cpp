#include "ViewAttendance.h"
#include "Globals.h"
#include "SendMessage.h"
#include <iostream>
#include <math.h>
#include "ReportAttData.h"
#include <rpt/IViewer.h>
#include <dp/IDataSetDelegate.h>
#include <gui/ContextMenu.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <mu/IAppSettings.h>
#include <gui/Image.h>
#include <gui/Frame.h>

ViewAttendance::ViewAttendance(td::INT4 SubjectID, ViewSubject* subject) :
    _LblSubjName(tr("AttSubj")),
    _LblDay(tr("AttDay")),
    _LblTime(tr("AttTime"))
    , _LblType(tr("AttType"))
    , _LblMaxNum(tr("AttMaxNum:"))
    , _btnAdd(tr("add"), tr("AttAddBtnTT"))
    , _btnUpdate(tr("Update"), tr("AttUpdateTT"))
    , _btnDelete(tr("Delete"), tr("AttDelBtnTT"))
    , _btnReport(tr("AttReport"), tr("AttRepTT"))
   // , _Time(td::Time::Format::TimeOwnShortHMM)
//,_btnSave(tr("Save"), tr("SaveTT"))
, _type(td::int4)
,_maxNum(td::int4)

, _hlBtnsDB(6)
,_gl(5, 4)
,_SubjectID(SubjectID)
,_imgClassAtt(":complex")
{
    _subject = subject;
    _maxNum.setValue(20);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnDelete);
    _hlBtnsDB.append(_btnUpdate);
 //   _hlBtnsDB.append(_btnSave);
    _hlBtnsDB.append(_btnAdd);
    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnReport);
   
   // _time.setValue(_Time);
    _btnUpdate.setType(gui::Button::Type::Default);
    //_btnSave.setType(gui::Button::Type::Default);
    _btnDelete.setType(gui::Button::Type::Destructive);
    _btnAdd.setType(gui::Button::Type::Constructive);
    _btnReport.setType(gui::Button::Type::Normal);
    
    SetCurrentSubject();
    _Subject.setAsReadOnly();
    
    gui::GridComposer gc(_gl);
    
    
    gc.appendRow(_LblSubjName);
    gc.appendCol(_Subject);  
    
    gc.appendRow(_LblType);
    gc.appendCol(_type);
    gc.appendCol(_LblDay);
    gc.appendCol(_dayCombo);
    
    gc.appendRow(_LblTime);
    gc.appendCol(_time);
    gc.appendCol(_LblMaxNum);
    gc.appendCol(_maxNum);

    gc.appendRow(_table, 0);
    gc.appendRow(_hlBtnsDB, 0);

    gui::View::setLayout(&_gl); 
   
    populateRoleCombo(_type);
    populateDayCombo(_dayCombo);
    populateData();

  //  _table.init(_pDS, {0,1,2,4});
    if (_pDS->getNumberOfRows())
    {
       _table.selectRow(0, true);
    }
   
    
}
void ViewAttendance::SetCurrentSubject(){
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
    dp::Params parDS(pSelect->allocParams());
    //d::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    parDS << _SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String Predmet;
    pCols << "Naziv_Predmeta" << Predmet;
    if(!pSelect->execute()){
        Predmet = "Haos";
    }
    while (pSelect->moveNext())
    {
        td::Variant val;
        val = Predmet;
        _Subject.setValue(val);

    }
    
}

void ViewAttendance::populateDayCombo(gui::ComboBox& combo)
{
    combo.addItem(tr("Monday"));
    combo.addItem(tr("Tuesday"));
    combo.addItem(tr("Wednesday"));
    combo.addItem(tr("Thursday"));
    combo.addItem(tr("Friday"));
    combo.addItem(tr("Saturday"));
    combo.addItem(tr("Sunday"));

    combo.selectIndex(0);

}
void ViewAttendance::populateRoleCombo(gui::DBComboBox &combo)
{
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("SELECT ID, Naziv FROM TipPredavanja");
    dp::Columns pCols = pSelect->allocBindColumns(2);
    td::String tip;
    td::INT4 id;
    pCols << "ID" << id << "Naziv" << tip;
    pSelect->execute();

    while (pSelect->moveNext())
    {
        combo.addItem(tip, id);
    }
    combo.selectIndex(0);
}

bool ViewAttendance::onChangedSelection(gui::ComboBox* pCmb)
{
    if (pCmb == &_dayCombo) {

        td::String dan = _dayCombo.getSelectedText();

        _day.setValue(dan);

        return true;
    }

    return false;
}
void ViewAttendance::populateData()
{
    auto pDB = dp::getMainDatabase();
    _pDS = pDB->createDataSet("SELECT a.Dan AS AttDay, a.Vrijeme AS Time, b.Naziv AS Type, b.ID as ID, a.Max_br_pol as AttMaxNum, a.Br_prijavljenih as brP, a.ID as IDterm FROM Termini a, TipPredavanja b WHERE a.TipPredavanjaID = b.ID and a.Predmet_ID = ?", dp::IDataSet::Execution::EX_MULT);
    
    dp::Params parDS(_pDS->allocParams());
    //td::INT4 IDPredmeta = Globals::_IDSubjectSelection;
    
    //u parDS ce se ucitavati Globals::CurrentSubject
    parDS << _SubjectID;
    
    dp::DSColumns cols(_pDS->allocBindColumns(7));
    cols << "AttDay" << td::string8 << "Time" << td::time<< "Type" << td::string8 << "ID" << td::int4 << "AttMaxNum" << td::int4<<"brP"<<td::int4<<"IDterm"<<td::int4;

    if (!_pDS->execute())
    {
        _pDS = nullptr;
        return;
    }
    initTable();
}

void ViewAttendance::initTable()
{
    gui::Columns visCols(_table.allocBindColumns(4));
    // visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("Type")) << gui::Header(1, tr("Time")) << gui::Header(2, tr("Date"));
    visCols << gui::Header(0, tr("AttDay"))<<td::Time::Format::TimeOwnShortHMM << gui::Header(1, tr("Time")) << gui::Header(2, tr("Type")) << gui::Header(4, tr("AttMaxNum"));
    _table.init(_pDS);
}
void ViewAttendance::populateDSRow(dp::IDataSet::Row& row)
{
    td::Variant val;
    _day.setValue(_dayCombo.getSelectedText());
    _day.getValue(val);
    row[0].setValue(val);

    _time.getValue(val);
    row[1].setValue(val);

    _type.getValue(val);
    row[3] = val.i4Val();
    row[2].setValue(_type.getSelectedText());
    _maxNum.getValue(val);
    row[4].setValue(val);
    

}
bool ViewAttendance::onChangedSelection(gui::TableEdit* pTE) {
    if (pTE == &_table) {
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0) {
            return true;
        }
        td::Variant val;
        dp::IDataSet* pDS = _table.getDataSet();
        auto& row = pDS->getRow(iRow);
        val = row[0];
        _day.setValue(val);
        for (int i = 0; i < _dayCombo.getNoOfItems(); i++)
        {
            _dayCombo.selectIndex(i);
            if (val == _dayCombo.getSelectedText())
                break;
       }
        val = row[1];
        _time.setValue(val);

        val = row[3];
        _type.setValue(val);
        
        val = row[4];
        _maxNum.setValue(val);

        return true;
    }
    return false;
}

void ViewAttendance::deleteRow(td::INT4 ID_Termina)
{
    dp::IStatementPtr pDelStudent(dp::getMainDatabase()->createStatement("DELETE FROM TerminiStudenti WHERE ID_Termina = ?"));// DA NE OSTANE NIKO PRIJAVLJEN NA TERMINU KOJI NE POSTOJI
    dp::Params par(pDelStudent->allocParams());
    par << ID_Termina;
    dp::Transaction tr1(dp::getMainDatabase());
    if (!pDelStudent->execute())
        return;
    tr1.commit();
    dp::IStatementPtr pDel(dp::getMainDatabase()->createStatement("DELETE FROM Termini WHERE ID = ?"));
    dp::Params par1(pDel->allocParams());
    dp::Transaction tr2(dp::getMainDatabase());
    par1 << ID_Termina;
    if (!pDel->execute())
        return;
    tr2.commit();
}


void ViewAttendance::saveData()
{
    dp::IStatementPtr pInsStat(dp::getMainDatabase()->createStatement("INSERT INTO Termini ( Dan, TipPredavanjaID, Predmet_ID, Vrijeme,Max_br_pol,Br_prijavljenih) VALUES(?,?,?,?,?,?)"));
    dp::Params parDS(pInsStat->allocParams());
    dp::Transaction tr(dp::getMainDatabase());

    td::String day;
    td::Time t;
    td::INT4 tip, predmet,Max_br_pol,Tr_broj;
    Tr_broj = 0;
    td::Variant val;
    
     _day.getValue(val);
     day = val;
     _type.getValue(val);
     tip = val.i4Val();
    predmet = _SubjectID;
    _time.getValue(val);
    t = val;
    _maxNum.getValue(val);
    Max_br_pol = val.i4Val();
    Tr_broj = 0;
    parDS << dp::toNCh(day, 30) << tip << predmet << t<<Max_br_pol<<Tr_broj;
    if (!pInsStat->execute())
        return;
    tr.commit();
    return;
}
void ViewAttendance::saveDataUpdate(td::INT4 maxNum, td::INT4 IDterm)
{
 
    dp::IStatementPtr pInstat(dp::getMainDatabase()->createStatement("UPDATE Termini SET Max_br_pol=?  WHERE ID = ? "));
    dp::Params parDS(pInstat->allocParams());
    dp::Transaction tr(dp::getMainDatabase());
    parDS << maxNum << IDterm;
    if (!pInstat->execute())
        return;
    tr.commit();
}
bool ViewAttendance::onClick(gui::Button* pBtn)
{
    if (pBtn == &_btnDelete)
    {
       
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return false;
        auto row = _pDS->getRow(iRow);
        td::INT4 id_termina = row[6].i4Val();
        _table.beginUpdate();
        _table.removeRow(iRow);
        _table.endUpdate();
        deleteRow(id_termina);
        _subject->getDay().clean();
        _subject->populateDayCombo(_subject->getDay());
        if(CheckTime())
            SendMsg(2);
      //  _table.reload();
       
        return true;
    }
   if (pBtn == &_btnUpdate)
    {
         
        td::Variant val1, val2;
        _day.getValue(val1);
        _time.getValue(val2); 
        if (doesItDexist(val1, val2.timeVal())==false)
       {
            showAlert(tr("alert"), tr("alertAttUpdate"));
            return true;
       }
        _maxNum.getValue(val1);
        if (val1 < 0)
        {

            showAlert(tr("alert"), tr("alertAttNegative"));
            return true;
        }
       
        int iRow = _table.getFirstSelectedRow();
        if (iRow < 0)
            return true; 
        
        auto& row = _table.getCurrentRow();
 if (val1.i4Val() < row[5].i4Val())
        {

            showAlert(tr("alert"), tr("alertAttNegative2"));
            return true;
        } 
        populateDSRow(row);
        td::INT4 IDterm = row[6].i4Val();
        saveDataUpdate(val1.i4Val(), IDterm);
       _table.reload();
       _table.selectRow(iRow);
        if(CheckTime())
            SendMsg(3);
        return true;
    }

    if (pBtn == &_btnAdd)
    {
        _day.setValue(_dayCombo.getSelectedText());
       td::Variant val1, val2;
       _day.getValue(val1); 
       _time.getValue(val2);
       if (doesItDexist(val1, val2.timeVal()))
        {
            showAlert(tr("alert"), tr("alertAttAdd"));
            return true;
        }
       _maxNum.getValue(val1);
       if (val1 < 0)
       {

           showAlert(tr("alert"), tr("alertAttNegative"));
           return true;
       }
        _table.beginUpdate();
        auto& row = _table.getEmptyRow();
        populateDSRow(row);
        _table.push_back();
        _table.endUpdate();
        saveData();
        _table.reload();
        _table.selectRow(_pDS->getNumberOfRows()-1);
        _subject->getDay().clean();
        _subject->populateDayCombo(_subject->getDay());
        if(CheckTime())
            SendMsg(1);
        return true;
    }
    if(pBtn == &_btnReport){
        AttendanceReport(&_imgClassAtt);
        return true;
    }
  /*  if(pBtn == &_btnSave){
        saveData();
    }*/

    return false;
}
bool ViewAttendance::doesItDexist(td::Variant day, td::Time time)
{
    size_t nRows = _pDS->getNumberOfRows();
    for (size_t i = 0; i < nRows; ++i)
    {
        auto row = _pDS->getRow(i);
        if (row[0] == day && row[1].timeVal() == time)
            return true;
    }
    return false;
}

void ViewAttendance::SendMsg(td::INT4 MsgType){
    
    std::vector<td::INT4> userIDs;
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select c.ID  from Korisnici c, Upis a, Predmet b WHERE a.ID_Smjera = b.ID_Smjera AND b.Semestar = a.Semestar AND c.Indeks = a.Indeks AND b.ID_Predmeta = ?");
    dp::Params pParams(pSelect->allocParams());
    pParams << _SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::INT4 id;
    pCols << "ID" << id;
    if (!pSelect->execute())
        return;
    while (pSelect->moveNext())
    {
        userIDs.push_back(id);
    }
    dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("select Naziv_Predmeta FROM Predmet WHERE ID_Predmeta = ?");
    dp::Params pParams1(pSelect1->allocParams());
    pParams1 << _SubjectID;
    dp::Columns pCols1 = pSelect1->allocBindColumns(1);
    td::String predmet;
    pCols1 << "Naziv_Predmeta" << predmet;
    if (!pSelect1->execute())
        return;
    if(!pSelect1->moveNext())
        return;
    //std::cout << predmet;
    td::String naslov;
    td::String poruka;
    if(MsgType == 2){
        naslov = "Uklonjen termin nastave!";
        poruka = "Uklonjen termin nastave za predmet ";
    }
    else if(MsgType == 1){
        naslov = "Novi termin nastave!";
        poruka = "Dodan je novi termin nastave za predmet ";
    }
    else if (MsgType == 3){
        naslov = "Promjena termina nastave!";
        poruka = "Promijenjen termin nastave za predmet ";
    }
    
    poruka += predmet;

    MsgSender za_poruke;
    za_poruke.sendSystemMsgtoUsers(naslov, poruka, userIDs,1);
    td::Time t(true);
    td::Date d(true);
    LastMsgTime = t;
    LastMsgDate = d;
}
bool ViewAttendance::CheckTime(){
    if(LastMsgTime == 0 && LastMsgDate == 0 )
        return true;
    td::Time currTime(true);
    td::Date currDate(true);
    if(currDate == LastMsgDate && std::fabs((LastMsgTime.getMinute() - currTime .getMinute())) <= 1)
        return false;
    return true;
}
void ViewAttendance::AttendanceReport(const gui::Image* pImage){
    
    dp::IDatabase* pDB = dp::getMainDatabase();

    //#ifdef REPTEST
    DocumentData docData(_SubjectID);
    CompanyData companyData;
    ObjectData objectData;

   rpt::ReportData repData(2, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

    rpt::ReportData::iterator it(repData.begin());
    
    td::Variant temp;
    td::Date tempD;
    td::Time tempT;
    {
        dp::IDataSet* pRep(pDB->createDataSet("select a.Ime, a.Prezime, c.Dan as Dan, c.Vrijeme as Vrijeme FROM Korisnici a, TerminiStudenti b, Termini c WHERE b.ID_Studenta = a.ID AND b.ID_termina = c.ID AND c.Predmet_ID = ?"));
        dp::Params pParams1(pRep->allocParams());
        pParams1 << _SubjectID;
        dp::DSColumns colsRep(pRep->allocBindColumns(4));
        colsRep << "Ime" << td::string8 << "Prezime" << td::string8 <<"Dan" << td::string8 <<"Vrijeme" << td::time;
           it << rpt::ContainerType::CNT_Body << pRep;
           
               
               if(!pRep->execute()){
                   return;
               }
               size_t nRows = pRep->getNumberOfRows();
               if (nRows == 0)
               {
                   //niko nije prijavljen, smislit sta treba radit
                   //return;
               }

    }
   ++it;
    it.nextLayout();
    {
        dp::IDataSet* pRep(pDB->createDataSet("SELECT k.Ime, k.Prezime FROM Korisnici k, UpisPredmeta u WHERE  u.ID_Studenta = k.ID AND k.PozicijaID = 5 AND u.ID_Predmeta = ? AND NOT EXISTS(SELECT 1 FROM TerminiStudenti p JOIN Termini t ON p.ID_termina = t.ID WHERE p.ID_studenta = k.ID AND t.Predmet_ID = ? );"));
        dp::Params pParams1(pRep->allocParams());
        pParams1 << _SubjectID<<_SubjectID;
        dp::DSColumns colsRep(pRep->allocBindColumns(2));
        colsRep << "Ime" << td::string8 << "Prezime" << td::string8;
           it << rpt::ContainerType::CNT_Body << pRep;
           
               
               if(!pRep->execute()){
                   return;
               }
               size_t nRows = pRep->getNumberOfRows();
               if (nRows == 0)
               {
                   //niko nije prijavljen, smislit sta treba radit
                   //return;
               }
    }
   
    td::String configName("ClassAttRep");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("ClassAttRep"), pImage);

    }
    

    
}
/*void ViewAttendance::AttendanceReport(const gui::Image* pImage){
    dp::IDatabase* pDB = dp::getMainDatabase();

    DocumentData docData(_SubjectID);
    CompanyData companyData;
    ObjectData objectData;

    //docData[RPT_TXT_DOCUMENT_TYPE] = "Prijave na termine nastave";
    //docData[RPT_TXT_DOCUMENT_ID] = "";
    
    rpt::ReportData repData(2, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

    rpt::ReportData::iterator it(repData.begin());
    //#endif
        //Grouper
    {
        dp::IDataSet* pDP(pDB->createDataSet("select b.Vrijeme FROM Termini b, TerminiStudenti c Where b.ID = c.ID_Termina ORDER BY Vrijeme ASC"));

        //#ifdef REPTEST
        it << rpt::ContainerType::CNT_Grouper << pDP; //define field and its data
        //#endif
        dp::DSColumns cols(pDP->allocBindColumns(1));
        cols  << "Vrijeme" << td::time ;
        if (!pDP->execute())//ovaj select ne prolazi ako pokušam učitavati i sortiran dan u sedmici
        {
            return;
        }
        //it << rpt::ContainerType::CNT_Body << pDP;

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            //nema termina u tabeli

            return;
        }
    }
    ++it;
    {
        dp::IDataSet* pDP(pDB->createDataSet("select a.Ime, a.Prezime From Korisnici a, TerminiStudenti c, Termini b Where a.ID = c.ID_Studenta AND c.ID_Termina = b.ID ORDER BY b.Vrijeme ASC"));

        //#ifdef REPTEST
        it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
        //#endif
        dp::DSColumns cols(pDP->allocBindColumns(2));
        cols  << "Ime" << td::string8 << "Prezime" << td::string8;
        if (!pDP->execute())//ovdje ne prolazi execute, a select radi u dbBrowseru...
        {
            return;
        }
        //it << rpt::ContainerType::CNT_Body << pDP;

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            //nema termina u tabeli

            return;
        }
    }

    //InvoiceSimple
    td::String configName("WaterfallAttempt");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("WaterfallAttempt"), pImage);

    }
}
*/
