#pragma once
#include <algorithm>
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <gui/Button.h>
#include <gui/NumericEdit.h>
#include <gui/ComboBox.h>
#include <gui/DBComboBox.h>
#include <gui/CheckBox.h>
#include <gui/Slider.h>
#include <gui/ProgressIndicator.h>
#include <gui/GridComposer.h>
#include <gui/DateEdit.h>
#include <gui/TimeEdit.h>
#include <gui/ColorPicker.h>
#include <gui/TableEdit.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>
#include <fo/FileOperations.h>
#include <gui/ImageView.h>
#include <rnd/MinMax.h>
#include "ViewUsers.h"
#include "Globals.h"


class ViewTicketForSAO : public gui::View
{
private:

protected:
	
	gui::Label _ticketslbl;
	gui::TableEdit _tickets;


	gui::HorizontalLayout _hlBtnsDB;
	gui::Button _btnOpen;

	gui::GridLayout _gl;
	dp::IDatabase* _db;
	dp::IDataSetPtr _pDS;


public:
	ViewTicketForSAO();

protected:

	void populateTableData();
	void initTable();
	bool onClick(gui::Button* pBtn);
	td::String getIndex();
	td::String getName();
	td::String getSurname();
	td::String getTypeOfTicket();
};