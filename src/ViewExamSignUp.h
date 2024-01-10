#pragma once
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
#include <gui/Color.h>
#include "Globals.h"




class ViewExamSignUp : public gui::View
{
protected:
    gui::Label _lblIndeks;
    gui::LineEdit _indeks;
 

    gui::Label _lblName;
    gui::LineEdit _name;
    gui::Label _lblSurname;
    gui::LineEdit _surname;
    gui::Label _lblTable1;
    gui::Label _lblTable2;

    gui::TableEdit _table1;
    gui::TableEdit _table2;
    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnEnroll;
    gui::Button _btnDEnroll;
    gui::Button _btnReload;

  
  
    gui::GridLayout _gl;

    dp::IDatabase* _db;
    dp::IDataSetPtr _pDS = nullptr;
    dp::IDataSetPtr _pDS2 = nullptr;
    td::INT4 _paramFrom;  //mozda kasnije potrebni
    td::INT4 _paramTo;    //mozda kasnije potrebni

    std::vector<td::INT4> _depsToDelete, _depsToInsert, _depsToUpdate;

public:
    ViewExamSignUp();
    ~ViewExamSignUp();
    td::Variant ID_Pre1;
    td::Variant ID_Pre2;

protected:
    void initTable1();
    void initTable2();
    bool saveData1();
    bool saveData2();
    virtual bool onClick(gui::Button* pBtn);
    void populateDataForTable1();
    void populateDataForTable2();
};

