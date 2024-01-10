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
#include "ViewCurriculumDialog.h"

//enum class QuestionID : td::UINT2 { Save };
class ViewCurriculum : public gui::View
{
private:
protected:
    // first row
    gui::Label _lblDepartment;
    gui::LineEdit _department;
    gui::Label _lblSemester;
    gui::NumericEdit _semester;
    gui::Label _lblCourse;
    gui::DBComboBox _course;
    gui::Label _lblShortname;
    gui::DBComboBox _shortname;
    gui::Label _lblECTS;
    gui::NumericEdit _ECTS;
    gui::TableEdit _table;

    //labels for the bottom row
    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnSave;
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    gui::Button _btnPushBack;
    gui::GridLayout _gl;
    //dp::IDatabase* _db;
    dp::IDatabasePtr _db;
    dp::IDataSetPtr _pDS;
    //  td::INT4 _paramFrom;
     // td::INT4 _paramTo;
    td::INT4 _departmentID;
    td::INT4 _semesterID;
    std::vector<td::INT4> _itemsToDelete, _itemsToInsert, _itemsToUpdate;

public:
    ViewCurriculum(td::INT4 _departmentID, td::INT4 _semesterID);
    ~ViewCurriculum();
protected:
    td::INT4 getIDfromTable(int rowID);
    bool canDelete(int iRow);
    bool eraseCurriculum();
    bool saveData();
    bool canAdd();
    bool existsInDepartment(td::INT4 id);
    bool canUpdate(int iRow);
    void populateData();
    bool updateCourse();
    bool loadComboBox(td::String select, gui::DBComboBox& combo);
    bool onChangedSelection(gui::TableEdit* pTE);
    void populateDSRow(dp::IDataSet::Row& row);
    //bool canAdd();    //trenutno ne treba 
    virtual bool onClick(gui::Button* pBtn);
    bool insertCurriculum();
    bool doesIDexist(td::INT4 id);
    //bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);
    void SetCurrentDepartment();
    void SetCurrentSemester();
    void SetCurrentECTS();
};