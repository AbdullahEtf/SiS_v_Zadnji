#pragma once
#include "ViewRequestUpdate.h"
#include "ViewIDs.h"

//td::INT4 current_index=5542;

ViewRequestUpdate::ViewRequestUpdate(td::String tipKarte, td::String naslov, gui::TextEdit body,td::INT4 id,td::String indeks)
    :
    _typelbl(tr("typeOfMessage"))
    , _subjectlbl(tr("subject"))
    , _bodylbl(tr("contentlbl"))
    , _attachlbl(tr("attachmentlbl"))
    , _hlBtnsDB(4)
    , _hlAttach(4)
    , _hlLabels(4)
    , _btnSave(tr("send"),tr("sendRequestTT"))
    , _btnAttachment(tr("attachmentAddLbl"),tr("attachmentTT"))
    , _gl(4, 4)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
    , _body(body)
    , _IDBaza(id)
    ,_indeks(indeks)
    ,strFileFullPath("")
    {

    _hlBtnsDB.appendSpacer();
    _hlBtnsDB.append(_btnSave);

    _hlAttach.append(_attachlbl);
    _hlAttach.append(_attachPath);
    _hlAttach.append(_btnAttachment);

    _hlLabels.append(_typelbl);
    _hlLabels.append(_type);
    _hlLabels.append(_subjectlbl);
    _hlLabels.append(_subject);


    _btnAttachment.setType(gui::Button::Type::Default);
    _btnSave.setType(gui::Button::Type::Default);


    gui::GridComposer gc(_gl);

    gc.appendRow(_hlLabels);
    gc.appendRow(_body);
    gc.appendRow(_hlAttach);
    gc.appendRow(_hlBtnsDB);

    gui::View::setLayout(&_gl);

    _db = dp::getMainDatabase();

    _type.setValue(tipKarte);
    _subject.setValue(naslov);

    _type.setAsReadOnly();
    _subject.setAsReadOnly();
    _attachPath.setAsReadOnly();


    //populateDataForStudent();
}


bool ViewRequestUpdate::onClick(gui::Button* pBtn) {

    if (pBtn == &_btnAttachment) {
        showOpenFileDialog();
        return true;
    }
    if (pBtn == &_btnSave)
    {
        showYesNoQuestionAsync(QuestionID::Save, this, tr("alert"), tr("saveSureTicket"), tr("Yes"), tr("No"));
        return true;
    }

    return false;
}


bool ViewRequestUpdate::onAnswer(td::UINT4 questionID, gui::Alert::Answer answer) {
    if ((QuestionID)questionID == QuestionID::Save)
    {
        if (answer == gui::Alert::Answer::Yes) {
            SaveToDatabase();
            showAlert(tr("succes"), tr("succesEE"));
        }
        this->getContainingWindow()->close();
        return true;
    }
    return false;
}






void ViewRequestUpdate::showOpenFileDialog()
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
                strFileFullPath = pFD->getFileName();

                if (fo::loadFileContent(strFileFullPath, strContent))
                {
                    //gui::TextEdit* pTE = _mainView.getTextEdit();
                    //pTE->setText(strContent);

                    _attachPath.setText(strFileFullPath.c_str());

                }
            }
        });
#endif

}





void ViewRequestUpdate::SaveToDatabase() {

    fo::fs::path homePath;
    mu::getHomePath(homePath);
    fo::fs::path testDBPath = (homePath / "Work\\CPProjects\\SiS_v_Zadnji\\Temp_Baza.db");

    dp::IDatabasePtr pDB = dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3);
    //dp::getMainDatabase() if I were connected to DB before

    if (!pDB->connect(testDBPath.string().c_str()))
    {
        assert(false && "Nema baze...");
        return;
    }
    //just for test, i always delete everything in the table, so it doesent get huge
    
    td::String strDelStat = "delete from SAOStudentTicket WHERE SAOStudentTicket.ID=";
    strDelStat.append(std::to_string(_IDBaza));

    dp::Transaction transactionpom(pDB);
    
    dp::IStatementPtr pStatDel = pDB->createStatement(strDelStat);

    bool delOK = pStatDel->execute();

    if (!delOK)
    {
        td::String errorstr;

        pStatDel->getErrorStr(errorstr);

        showAlert(errorstr, "");


    }

    transactionpom.commit();


    dp::IStatementPtr pStatIns;

    if (strFileFullPath!= td::String(""))
        pStatIns = pDB->createStatement("insert into SAOStudentTicket(ID,Indeks,Ticket_Tip,Req_Title,Request,Attachment,Status) values(?,?,?,?,?,?,?)");
    else
        pStatIns = pDB->createStatement("insert into SAOStudentTicket(ID,Indeks,Ticket_Tip,Req_Title,Request,Status) values(?,?,?,?,?,?)");
  
    dp::Params paramsInsert(pStatIns->allocParams());

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
    // td::INT4 ID = 1;

    td::String tip = _type.getText();
    td::String title = _subject.getText();
    td::String request = _body.getText();

    td::String status(tr("OnHold"));

    if(strFileFullPath!=td::String(""))
       paramsInsert <<_IDBaza<<dp::toNCh(_indeks,30)<< dp::toNCh(tip, 30) << dp::toNCh(title, 30)<< dp::toNCh(request, 10000)<< dataIn<<dp::toNCh(status,30);
    else
        paramsInsert <<_IDBaza<< dp::toNCh(_indeks, 30) << dp::toNCh(tip, 30) << dp::toNCh(title, 30) << dp::toNCh(request, 10000) << dp::toNCh(status, 30);

    //Neophodno, sa ove lokacije (strFileFullPath) se uzima BLOB
    if (!dataIn.setInFileName(strFileFullPath) && strFileFullPath != td::String(""))
    {
        gui::Alert::show(tr("Error"), tr("Did you delete selected file?"));
        return;
    }

    dp::Transaction transaction(pDB);

    bool insOK = pStatIns->execute();

    if (!insOK) {
        td::String strerr;

        pStatIns->getErrorStr(strerr);

        showAlert(strerr, "");
    }

    bool commitOK = transaction.commit();


}



