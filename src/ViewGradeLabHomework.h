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

class ViewGradeLabHomework : public gui::View {
protected:
    gui::Label _lblName;
    gui::LineEdit _name;
    gui::Label _lblLName;
    gui::LineEdit _lName;
    gui::Label _lblIndex;
    gui::LineEdit _index;
    gui::Label _lblGrade;
    gui::LineEdit _grade;
    gui::Label _lblActivityName;
    gui::LineEdit _activityName;

    gui::Label _lblCName;
    gui::LineEdit _cName;

    gui::HorizontalLayout _hlBtns;
    gui::HorizontalLayout _report;

    gui::Button _btnAdd;
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    gui::Button _btnSave;
    gui::Button _btnReport;


    dp::IDatabase* _db;
    gui::GridLayout _gl;
    dp::IDataSetPtr _pDS;
    td::INT4 _SubjectID;
    td::INT4 _ActivityID;
    td::INT4 _UserID;
    gui::Image _imgHWGrades;


    gui::TableEdit _table;
    std::set<td::INT4> _userids; 


    std::vector<td::INT4> _itemsToDelete, _itemsToInsert, _itemsToUpdate;
public:
    ViewGradeLabHomework(td::INT4 SubjectID);   // Adnan
protected:
    void populateData();    // Adnan
    bool onChangedSelection(gui::TableEdit* pTE);    // Adnan
    void populateDSRow(dp::IDataSet::Row& row, td::INT4 i);    // Adnan
    bool canAdd();      // uradjeno trenutno
    bool eraseLabHomeworkGrade();    // Iman
    bool insertLabHomeworkGrade();    // Iman
    bool updateLabHomeworkGrade();    // Iman
    bool saveData();    // uradjeno
    bool onClick(gui::Button* pBtn); // uradjeno
    void SetCurrentSubject();     // uradjeno
    // void SetActivityName(td::Variant& val, td::INT4 br);    //---------Adnan-----moze se iskoristiti nesto poput ove funkcije za  ucitavanje ovih naziva
    td::INT4 getIDfromTable(int rowID);    // uradjeno + provjera
    td::INT4 findMaxID();     // uradjeno
    void insertValues(td::INT4 subjectID);
};
