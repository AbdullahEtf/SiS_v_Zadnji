#pragma once
#include "Reports.h"
#include "ReportData.h"
#include <rpt/IViewer.h>
#include <dp/IDataSetDelegate.h>
#include <gui/ContextMenu.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <mu/IAppSettings.h>
#include <gui/Image.h>
#include <gui/Frame.h>


void examAttendance(const gui::Image* pImage, td::INT4 SubjectID)
{
	dp::IDatabase* pDB = dp::getMainDatabase();
	td::String name = "Studenti prijavljeni na ispit";
	//#ifdef REPTEST
	DocumentData docData(SubjectID, name);
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	//#endif
		//Body 
	{
		dp::IDataSet* pDP(pDB->createDataSet("select a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime from PolazniciAktivnosti a, Aktivnosti b, Korisnici c where a.ID_Aktivnosti = b.ID_Aktivnosti and a.ID_Korisnika = c.ID and b.ID_Predmeta = ? and b.Tip_Aktivnosti=1 Order by b.Naziv_Aktivnosti DESC"));

		//#ifdef REPTEST
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
		//#endif
		dp::DSColumns cols(pDP->allocBindColumns(6));
		cols << "ID_Korisnika" << td::int4 << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8;
		dp::Params pars = pDP->allocParams();
		pars << SubjectID;


		if (!pDP->execute())//sto ovdje nece ne znam...
		{
			return;
		}


		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			//-----------------alert da nema ispita na predmetu ili nema prijavljenih studenata
			//showAlert(tr("AccessNotAllowed"), "");

			return;
		}
	}

	//InvoiceSimple
	td::String configName("ExamAttendanceRep");

	rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
	if (pRepView)
	{
		pRepView->show(gui::tr("ExamAttendanceRep"), pImage);

	}

}

void examGrades(const gui::Image* pImage, td::INT4 SubjectID)
{
	dp::IDatabase* pDB = dp::getMainDatabase();
	td::String name = "Ocjene studenata na ispitu";
	//#ifdef REPTEST
	DocumentData docData(SubjectID, name);
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	//#endif
		//Body 
	{
		dp::IDataSet* pDP(pDB->createDataSet("SELECT a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime, d.Ocjena FROM PolazniciAktivnosti a JOIN Aktivnosti b ON a.ID_Aktivnosti = b.ID_Aktivnosti JOIN Korisnici c ON a.ID_Korisnika = c.ID JOIN OcjeneIspita d ON b.ID_Aktivnosti = d.ID_Aktivnosti AND a.ID_Korisnika = d.ID_Korisnika WHERE b.ID_Predmeta = ? AND b.Tip_Aktivnosti = 1 AND d.Ocjena IS NOT NULL"));

		//#ifdef REPTEST
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
		//#endif
		dp::DSColumns cols(pDP->allocBindColumns(7));
		cols << "ID_Korisnika" << td::int4 << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8 << "Ocjena" << td::string8;
		dp::Params pars = pDP->allocParams();
		pars << SubjectID;


		if (!pDP->execute())
		{
			return;
		}


		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			//-----------------alert da nema ispita na predmetu ili nema prijavljenih studenata
			//showAlert(tr("AccessNotAllowed"), "");

			return;
		}
	}

	//InvoiceSimple
	td::String configName("ExamGradesRep");

	rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
	if (pRepView)
	{
		pRepView->show(gui::tr("ExamGradesRep"), pImage);

	}

}