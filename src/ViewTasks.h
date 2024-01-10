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

enum class QuestionIDDDAAAA : td::UINT2 { Saveee };
enum class QuestionIDA : td::UINT4 { OpenFile = 1, SaveFile };
enum class WndID : td::UINT4 { SingleWnd = 1, FileOpenDlg, FileSaveDlg, LoginDlg, SettingsDlg };

class ViewTasks : public gui::View
{
private:
protected:
    gui::Label _LblActName;
    gui::LineEdit _Subject;

    gui::Label _LblDateBegin;
    gui::DateEdit _dateB;
    gui::Label _LblTimeBegin;
    gui::TimeEdit _timeB;

    gui::Label _lblType;
    gui::DBComboBox _type;
    gui::Label _lblCName;
    gui::LineEdit _cName;

    gui::Label _lblTable2;
    gui::TableEdit _table2;

    gui::HorizontalLayout _hlBtnsDB;
    gui::Button _btnAdd;
    gui::Button _btnDelete;
    gui::Button _btnAddFile;
    //  gui::Button _btnUpdate;
    gui::Button _btnSave;

    dp::IDatabase* _db;
    gui::GridLayout _gl;
    dp::IDataSetPtr _pDS;
    //::INT4 _ActivityID;
    td::INT4 _SubjectID;
    ///*td::string8 _nazivakt;*/
    gui::TextEdit _textEdit;

    gui::TableEdit _table;
    std::vector<td::INT4> _itemsToDelete, _itemsToInsert, _itemsToUpdate;
public:
    ViewTasks(td::INT4 SubjectID);
protected:
    bool loadComboBox(td::String select, gui::DBComboBox& combo);
    void populateData();
    bool onChangedSelection(gui::TableEdit* pTE);
    void populateDSRow(dp::IDataSet::Row& row, td::INT4 i);
    bool doesItDexist(td::Date d, td::Time t);
    bool onClick(gui::Button* pBtn);
    bool saveData();
    bool canAdd();
    bool eraseDateTime();
    bool insertDateTime();
    void SetCurrentSubject();
    void SetActivityName(td::Variant& val, td::INT4 br);
    td::INT4 getIDfromTable(int rowID);
    td::INT4 findMaxID();
    void openFile(gui::FileDialog* pFD);
    void showOpenFileDialog();
    gui::TextEdit* getTextEdit();
    bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer);
    bool onAnswer2(td::UINT4 questionIDA, gui::Alert::Answer answer);
    bool onClick(gui::FileDialog* pFD, td::UINT4 dlgID);
    void saveFile(gui::FileDialog* pFD);
};
