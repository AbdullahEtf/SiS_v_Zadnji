#pragma once
#include <rpt/ReportData.h>
#include <rpt/EntityData.h>
#include <rpt/AutoElement.h>

class DocumentData : public rpt::DocumentData
{
public:
    DocumentData(td::INT4 SubjectID, td::String name)
        : rpt::DocumentData()
    {
        //has to be loaded from DB
        td::DateTime now;
        now.now();
        td::DateTime in4Days(now);
        in4Days.addDays(14);

        dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select Naziv_Predmeta from Predmet where ID_Predmeta=?");
        dp::Params pParams2(pSelect->allocParams());
        pParams2 << SubjectID;
        td::String s;
        dp::Columns pColumns = pSelect->allocBindColumns(1);
        pColumns << "Naziv_Predmeta" << s;
        if (!pSelect->execute())
            return;
        if (!pSelect->moveNext())
            return;
        _data[0] = name; //RPT_TXT_DOCUMENT_TYPE
        _data[1] = s; //RPT_TXT_DOCUMENT_ID
        //_data[2] = now;	 //RPT_DT_DOCUMENT_DATETIME
        //_data[3] = (td::INT4)14; //RPT_INT_DOCUMENT_PAYMENT_DAYS
        //_data[4] = (td::INT4)30; //RPT_INT_DOCUMENT_EXTRA_INTEREST_RATE_AFTER_DAYS
        //_data[5] = in4Days; //RPT_DT_DOCUMENT_PAYMENT_DEADLINE_DATETIME
        //_data[6] = (td::INT4)0; //RPT_INT_DOCUMENT_STATE
        //_data[7] = now; //RPT_DT_DOCUMENT_FROM_DATETIME
        //_data[8] = now; //RPT_DT_DOCUMENT_TO_DATETIME
        //_data[9] = td::String("BAM"); //RPT_TXT_DOCUMENT_MONEY_UNIT
        //_data[10] = td::String("KM"); //RPT_TXT_DOCUMENT_MONEY_SYMBOL
        //_data[11] = (td::INT4)7; //RPT_INT_DOCUMENT_RECLAMATION_DAYS
        //_data[12] = td::String("CUST-DOC-No"); //RPT_TXT_DOCUMENT_CUST_DOC_NO;
        //_data[13] = now; //RPT_DT_DOCUMENT_CUST_DOC_DT
        //_data[14] = td::String("INT-DOC-No"); //RPT_DT_DOCUMENT_INT_DOC_NO;
        //_data[15] = now; //RPT_DT_DOCUMENT_INT_DOC_DT
        //_data[16] = td::String("Rev 1.0 (01.03.2017)"); ; //RPT_DT_DOCUMENT_REVISION
    }
};

class ObjectData : public rpt::ObjectData
{
public:
    ObjectData()
        : rpt::ObjectData()
    {
        //has to be loaded from DB
        _data[0] = "Laboratorija 1";
        _data[1] = "Glavna zgrada";
        _data[2] = "Zmaja od Bosne";
        _data[3] = "Sarajevo";
        _data[4] = "tralala la ";
        _data[5] = "033 671 612";
    }
};

class SignatureData : public rpt::SignatureData
{
public:
    SignatureData()
        : rpt::SignatureData()
    {
        //has to be loaded from DB
        _data[0] = "Niko Nikic";
        _data[1] = "Leko Lekic";
        _data[2] = "Neko Nepoznat";
    }
};

class CompanyData : public rpt::CompanyData
{
public:
    CompanyData()
        : rpt::CompanyData()
    {
        //has to be loaded from DB
        _data[0] = "UNIS \"TOK\" d.o.o.";
        _data[1] = "Tvornica za proizvodnju odbojno vlacnih naprava i otkivaka";
        _data[2] = "Zmaja od Bosne bb, 71000 Sarajevo";
        _data[3] = "---ddd--";
        //etc
    }
};

