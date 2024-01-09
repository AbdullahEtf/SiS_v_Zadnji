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


enum class QuestionIDDDAAA : td::UINT2 { Saveee };

class ViewActivity : public gui::View {
protected:
    //gui::Label _lblID;
    gui::NumericEdit _id;       //id aktivnosti
    gui::Label _lblName; 
    gui::LineEdit _name;        //naziv aktivnosti
    //gui::Label _lblIDP; 
    td::INT4 _idP;      //id predmeta 
    gui::Label _lblType;
    gui::DBComboBox _type;     //Vrsta aktivnosti
    gui::Label _lblCName;
    gui::LineEdit _cName;       //ime predmeta
    gui::Label _lblDes;
    gui::LineEdit _desAct;      //opis aktivnosti
    gui::Label _lblPoints;
    gui::NumericEdit _points;   // procenat ili poeni

    gui::TableEdit _table;
    gui::HorizontalLayout _hlBtns;
    gui::Button _btnAdd;
    gui::Button _btnSave;
    gui::Button _btnReload;
    //gui::Button _btnRemoveAll;
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    gui::Button _btnInsert;   //jedna    ove       mozda    treba
    //gui::Button _btnPushBack; //      od     dvije       ne 
    gui::GridLayout _gl;

    dp::IDatabase* _db;
    dp::IDataSet* _pDS = nullptr;
    td::INT4 _paramFrom;
    td::INT4 _paramTo;

    std::vector<td::INT4> _actsToDelete, _actsToInsert, _actsToUpdate;
    td::INT4 findMaxID();
public:
    ViewActivity(td::INT4 SubjectID);
  //  ViewActivity();
    ~ViewActivity();
protected:
    td::INT4 getIDfromTable(int rowID);
    bool canDelete(int iRow);     //adnan
    bool eraseActivity();          ///adnan
    bool saveData();            //adnan
    bool canAdd();//ovo i funkcija ispod se mogu mijenjati              //adnan
    bool doesIDexist(td::INT4 id);
    bool existsInActivity(td::INT4 id);
    //bool existsInDepartment(td::INT4 id);  //ne treba da se provjerava jer se ne koristi Dep
    //bool existsInCourse(td::INT4 id);// ne bi rekao da treba
    bool canUpdate(int iRow);       //adnan
    void populateData();
    bool updateActivity();
    bool loadComboBox(td::String select, gui::DBComboBox& combo);
    virtual bool onChangedSelection(gui::TableEdit* pTE);
    void populateDSRow(dp::IDataSet::Row& row, td::INT4 i);
    virtual bool onClick(gui::Button* pBtn);
    bool insertActivity();   
    void SetCurrentSubject();
    bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);
    void SetActivityTypeName(td::Variant& val, td::INT4 br);
};


