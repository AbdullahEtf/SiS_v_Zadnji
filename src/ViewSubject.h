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

class ViewSubject : public gui::View
{
    gui::Label _lblName;
    gui::LineEdit _name;
    gui::Label _lblSurname;
    gui::LineEdit _surname;
    gui::TableEdit _table;
    gui::Label _lblTablePresent;
    gui::TableEdit _tablePresent;
    gui::Label _lblTime;
    gui::DBComboBox _time;
    gui::Label _lblDate;
    gui::DBComboBox _date;
    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnPresent;
    gui::Button _btnNotPresent;
    gui::GridLayout _gl;
    dp::IDataSetPtr _pDS;
    dp::IDataSetPtr _pDS2;
    dp::IDataSetPtr _pDSPos;
    td::INT4 _SubjectID;

    td::INT4 _TerminID;


public:
    ViewSubject(td::INT4 SubjectID);

protected:
    td::INT4 getCurrentTerminID();
     bool onChangedSelection(gui::TableEdit* pTE);
     bool onChangedSelection(gui::DBComboBox* pCB);
    void populateData();
    void populateTablePresent();
    void populateDateCombo(gui::DBComboBox& combo);
    void populateTimeCombo(gui::DBComboBox& combo, td::Date date);
    bool saveData();
    bool onClick(gui::Button* pBtn);
    bool doesIDexist(td::INT4 ID);
};

