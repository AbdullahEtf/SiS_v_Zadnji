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
#include <gui/View.h>

void examAttendance(const gui::Image* pImage, td::INT4 SubjectID)
{	
	td::Date d(true);
	td::Time t(true);
	dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("INSERT INTO PolazniciAktivnosti (ID_Korisnika, ID_Aktivnosti) SELECT a.ID_Studenta, b.ID_Aktivnosti FROM Prijavljeni_ispiti a JOIN Rokovi b ON a.ID_Roka = b.ID_Roka WHERE NOT EXISTS ( SELECT 1 FROM PolazniciAktivnosti pa WHERE pa.ID_Korisnika = a.ID_Studenta AND pa.ID_Aktivnosti = b.ID_Aktivnosti ) AND b.Datum_Prijave < ? AND b.Vrijeme_Prijave < ?");
	dp::Params pParams2(pSelect2->allocParams());
	pParams2 << d << t;
	//dp::IStatementPtr pSelect2 = dp::getMainDatabase()->createStatement("INSERT INTO PolazniciAktivnosti (ID_Korisnika, ID_Aktivnosti) SELECT a.ID_Studenta, b.ID_Aktivnosti FROM Prijavljeni_ispiti a JOIN Rokovi b ON a.ID_Roka = b.ID_Roka WHERE NOT EXISTS ( SELECT 1 FROM PolazniciAktivnosti pa WHERE pa.ID_Korisnika = a.ID_Studenta AND pa.ID_Aktivnosti = b.ID_Aktivnosti )");
	dp::Transaction a(dp::getMainDatabase());
	if (!pSelect2->execute())
		return;
	a.commit();
	//------------------TAKODER dodati select za upis u tabelu PolazniciAktivnosti - samo se ucitava nakon sto je proslo vrijeme prijave ispita

	// trenutno ucitava nakon isteka roka za prijavu - moguce popraiti ucitavanjem iz tabele Prijavljeni ispiti
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
		dp::IDataSet* pDP(pDB->createDataSet("select a.ID_Studenta, d.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime  from Prijavljeni_ispiti a, Aktivnosti b, Korisnici c, Rokovi d where d.ID_Aktivnosti = b.ID_Aktivnosti and a.ID_Studenta = c.ID and b.ID_Predmeta = ? and b.Tip_Aktivnosti = 1 and a.ID_Roka = d.ID_Roka Order by b.Naziv_Aktivnosti DESC"));

		//#ifdef REPTEST
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
		//#endif
		dp::DSColumns cols(pDP->allocBindColumns(6));
		cols << "ID_Studenta" << td::int4 << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8;
		dp::Params pars = pDP->allocParams();
		pars << SubjectID;


		if (!pDP->execute())//sto ovdje nece ne znam...
		{
			//gui::Alert::show(tr("Error"), tr("Ne moze..."));
			return;
		}


		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			//-----------------alert da nema ispita na predmetu ili nema prijavljenih studenata
			gui::View view;
			view.showAlert(gui::tr("AccessFound"), "Nema studenata prijavljenih na ispit.");

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

void examGrade(const gui::Image* pImage, td::INT4 SubjectID, td::INT4 id_akt)
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
		dp::IDataSet* pDP(pDB->createDataSet("SELECT a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime, d.Ocjena FROM PolazniciAktivnosti a JOIN Aktivnosti b ON a.ID_Aktivnosti = b.ID_Aktivnosti JOIN Korisnici c ON a.ID_Korisnika = c.ID JOIN OcjeneIspita d ON b.ID_Aktivnosti = d.ID_Aktivnosti AND a.ID_Korisnika = d.ID_Korisnika WHERE b.ID_Predmeta = ? AND a.ID_Aktivnosti=? AND b.Tip_Aktivnosti = 1 AND d.Ocjena IS NOT NULL"));

		//#ifdef REPTEST
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
		//#endif
		dp::DSColumns cols(pDP->allocBindColumns(7));
		cols << "ID_Korisnika" << td::int4 << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8 << "Ocjena" << td::string8;
		dp::Params pars = pDP->allocParams();
		pars << SubjectID<< id_akt;


		if (!pDP->execute())
		{
			return;
		}


		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			gui::View view;
			view.showAlert(gui::tr("AccessFound"), "Nema studenata sa ocjenama na ispitu.");

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

void examGrades(const gui::Image* pImage, td::INT4 SubjectID)
{
	dp::IDatabase* pDB = dp::getMainDatabase();
	td::String name = "Ocjena studenta na ispitu";
	//#ifdef REPTEST
	DocumentData docData(SubjectID, name);
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	//#endif
		//Body 
	{
		dp::IDataSet* pDP(pDB->createDataSet("SELECT a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime, a.Ocjena FROM OcjeneIspita a JOIN Aktivnosti b ON a.ID_Aktivnosti = b.ID_Aktivnosti JOIN Korisnici c ON a.ID_Korisnika = c.ID WHERE b.ID_Predmeta = ? AND b.Tip_Aktivnosti = 1 AND a.Ocjena IS NOT NULL"));

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
			gui::View view;
			view.showAlert(gui::tr("AccessFound"), "Nema studenata sa ocjenama na ispitu.");

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
void homeworkGrades(const gui::Image* pImage, td::INT4 SubjectID)
{
	dp::IDatabase* pDB = dp::getMainDatabase();
	td::String name = "Ocjene studenata na zadaci";
	//#ifdef REPTEST
	DocumentData docData(SubjectID, name);
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	//#endif
		//Body 
	{
		dp::IDataSet* pDP(pDB->createDataSet("SELECT a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime, d.Ocjena as Procenat FROM PolazniciAktivnosti a JOIN Aktivnosti b ON a.ID_Aktivnosti = b.ID_Aktivnosti JOIN Korisnici c ON a.ID_Korisnika = c.ID JOIN OcjeneLabZadace d ON b.ID_Aktivnosti = d.ID_Aktivnosti AND a.ID_Korisnika = d.ID_Korisnika WHERE b.ID_Predmeta = ? AND b.Tip_Aktivnosti = 2 AND d.Ocjena IS NOT NULL"));

		//#ifdef REPTEST
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
		//#endif
		dp::DSColumns cols(pDP->allocBindColumns(7));
		cols << "ID_Korisnika" << td::int4 << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8 << "Procenat" << td::string8;
		dp::Params pars = pDP->allocParams();
		pars << SubjectID;


		if (!pDP->execute())
		{
			return;
		}


		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			gui::View view;
			view.showAlert(gui::tr("AccessFound"), "Nema studenata sa ocjenama na zadaci.");

			return;
		}
	}

	//InvoiceSimple
	td::String configName("ExamGradesLabHW");

	rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
	if (pRepView)
	{
		pRepView->show(gui::tr("ExamGradesLabHW"), pImage);

	}

}

void labGrades(const gui::Image* pImage, td::INT4 SubjectID)
{
	dp::IDatabase* pDB = dp::getMainDatabase();
	td::String name = "Ocjene studenata na laboratorijskoj vjezbi";
	//#ifdef REPTEST
	DocumentData docData(SubjectID, name);
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	//#endif
		//Body 
	{
		dp::IDataSet* pDP(pDB->createDataSet("SELECT a.ID_Korisnika, a.ID_Aktivnosti, b.Naziv_Aktivnosti, c.Indeks, c.Ime, c.Prezime, d.Ocjena as Procenat FROM PolazniciAktivnosti a JOIN Aktivnosti b ON a.ID_Aktivnosti = b.ID_Aktivnosti JOIN Korisnici c ON a.ID_Korisnika = c.ID JOIN OcjeneLabZadace d ON b.ID_Aktivnosti = d.ID_Aktivnosti AND a.ID_Korisnika = d.ID_Korisnika WHERE b.ID_Predmeta = ? AND b.Tip_Aktivnosti = 5 AND d.Ocjena IS NOT NULL"));

		//#ifdef REPTEST
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
		//#endif
		dp::DSColumns cols(pDP->allocBindColumns(7));
		cols << "ID_Korisnika" << td::int4 << "ID_Aktivnosti" << td::int4 << "Naziv_Aktivnosti" << td::string8 << "Indeks" << td::string8 << "Ime" << td::string8 << "Prezime" << td::string8 << "Procenat" << td::string8;
		dp::Params pars = pDP->allocParams();
		pars << SubjectID;


		if (!pDP->execute())
		{
			return;
		}


		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			gui::View view;
			view.showAlert(gui::tr("AccessFound"), "Nema studenata sa ocjenama na laboratorijskoj vjezbi.");

			return;
		}
	}

	//InvoiceSimple
	td::String configName("ExamGradesLabHW");

	rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
	if (pRepView)
	{
		pRepView->show(gui::tr("ExamGradesLabHW"), pImage);

	}

}