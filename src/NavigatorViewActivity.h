#pragma once
#include <gui/Controls.h>
#include <gui/Navigator.h>
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
#include "SwitcherViewActivity.h"
#include "ViewIDs.h"


class NavigatorViewActivity : public gui::View
{
protected:
    gui::HorizontalLayout _hl;
    gui::Navigator _navigator;
    gui::Image _imgActivity;
    gui::Image _imgSubject;
    gui::Image _imgTask;
    SwitcherViewActivity _switcherView;
public:
    NavigatorViewActivity(td::INT4 ViewID, td::INT4 subjectID)
        : _hl(2)
        , _navigator(3, true)
        , _imgActivity(":imgActivity")
        , _imgSubject(":imgSubject")
        , _imgTask(":imgSubject")
        , _switcherView(subjectID)

    {
        setMargins(0, 0, 0, 0);
        _navigator.setItem(0, &_imgActivity, tr("Activity"));
        _navigator.setItem(1, &_imgSubject, tr("Subject"));
        _navigator.setItem(2, &_imgTask, tr("Task"));
        _hl.append(_navigator);
        _hl.append(_switcherView);
        setLayout(&_hl);
        setVisualID(ViewID);
    }
    void setNavigatorSelection(td::UINT2 sel)
    {
        _navigator.setCurrentSelection(sel);
        _switcherView.showView((int)sel);
    }
protected:
    virtual bool onChangedSelection(gui::Navigator* pNav)
    {
        td::UINT2 currSelection = pNav->getCurrentSelection();
        _switcherView.showView((int)currSelection);
        return true;
    }
};


