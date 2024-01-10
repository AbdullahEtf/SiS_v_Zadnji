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
#include <gui/FileDialog.h>
#include "ViewUsers.h"
#include "Globals.h"


class ViewTicket : public gui::View {
private:

protected:
	//first row
	gui::Label _typelbl;
	gui::ComboBox _typeCombo;
	gui::LineEdit _type;
	gui::Label _subjectlbl;
	gui::LineEdit _subject;
	gui::Label _bodylbl;
	gui::TextEdit _body;

	//third row
	gui::HorizontalLayout _hlBtnsDB;
	gui::Button _btnSend;
	gui::Button _btnOpen;
	gui::HorizontalLayout _hlBtns;
	gui::Button _btnAttach;

	gui::TableEdit _tableTickets;
	gui::GridLayout _gl;
	dp::IDatabase* _db;
	dp::IDataSetPtr _pDS;
	gui::Label _attachedFile;
	gui::Label _titleFile;
	td::String _filePath;
	cnt::PushBackVector <td::String> _attachedFiles;

public:
	ViewTicket();

protected:
	//void populateTypeOfTicket(); POSTO NECEMO IMATI TABELU HOCE LI NAM TREBATI OVA FUNKCIJA?
	//void populateTypeCombo(gui::DBComboBox& combo);
	bool onClick(gui::Button* pBtn) override;
	bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer) override;
	bool sendTicketWithAttachment();
	bool sendTicket();
	td::Variant GetStudentIndeks();
	void populateTableData();
	void selectFiles();
	void initTable();
	void populateTypeTicketCombo(gui::ComboBox& combo);
	bool onChangedSelection(gui::ComboBox* pCmb);

	void UpdateTable();

};