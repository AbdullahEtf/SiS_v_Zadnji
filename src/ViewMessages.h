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

class ViewMessages : public gui::View
{
private:
protected:
    gui::Label _lblName;
    gui::LineEdit _name;
    gui::Label _lblLastName;
    gui::LineEdit _lastname;
    gui::Label _lblIndex;
    gui::LineEdit _index;
    gui::LineEdit _lblMsg;
    gui::TextEdit _message;
    gui::GridLayout _gl;
    dp::IDataSetPtr _pDS;
    gui::TableEdit _table;

    dp::IDatabase* _db;

public:
    ViewMessages();
    ~ViewMessages();
protected:
    void SetCurrentData();
    void populateData();
    void initTable(int type);
   
};
