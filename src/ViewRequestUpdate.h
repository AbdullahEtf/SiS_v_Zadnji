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
#include "ViewIDs.h"
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <fo/FileOperations.h>
#include <td/BLOB.h>
#include "Globals.h"
#include <gui/Window.h>



class ViewRequestUpdate : public gui::View {
private:

protected:
	gui::Label _typelbl;
	gui::LineEdit _type;
	gui::Label _subjectlbl;
	gui::LineEdit _subject;
	gui::LineEdit _attachPath;
	gui::Label _bodylbl;
	gui::TextEdit _body;
	gui::Label _attachlbl;

	//third row
	gui::HorizontalLayout _hlBtnsDB;
	gui::HorizontalLayout _hlAttach;
	gui::HorizontalLayout _hlLabels;
	gui::Button _btnSave;
	gui::Button _btnAttachment;

	gui::GridLayout _gl;
	dp::IDatabase* _db;
	dp::IDataSetPtr _pDS;
	td::INT4 _IDBaza;

	td::String strFileFullPath;
	td::String strContent;

	td::String _indeks;


public:

	ViewRequestUpdate(td::String tipKarte, td::String naslov,gui::TextEdit body,td::INT4 id,td::String indeks);

protected:


	void showOpenFileDialog();

	bool onClick(gui::Button* pBtn) override;

	void SaveToDatabase();

	bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer) override;
	
	//void populateDataForStudent();
	/*void initTableStudent();
	void populateDataForRequest();
	void initTableRequest();*/


};