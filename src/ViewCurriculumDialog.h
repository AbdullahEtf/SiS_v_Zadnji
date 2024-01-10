#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/PasswordEdit.h>
#include <gui/GridLayout.h>
#include <gui/NumericEdit.h>
#include <gui/ComboBox.h>
#include <gui/DBComboBox.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>

class ViewCurriculumDialog : public gui::View
{
private:
protected:
    gui::Label _lblDepartment;
    gui::DBComboBox _department;
    gui::Label _lblSemester;
    gui::NumericEdit _semester;
    gui::GridLayout _gl;
    dp::IDatabase* _db;
    dp::IDataSet* _pDS = nullptr;
public:
    ViewCurriculumDialog()
        : _lblDepartment(tr("Department"))
        , _department(td::int4)
        , _lblSemester(tr("Semester"))
        , _semester(td::int4)
        , _gl(2, 2)
        ,_db(dp::getMainDatabase())
    {

        // _department.setToolTip(tr("Enter department:"));
        _gl.insert(0, 0, _lblDepartment);
        _gl.insert(0, 1, _department);
        _gl.insert(1, 0, _lblSemester);
        _gl.insert(1, 1, _semester);

        gui::View::setLayout(&_gl);
    }

    bool loadComboBox(td::String select, gui::DBComboBox& combo)
    {
        dp::IStatementPtr pSelect = _db->createStatement(select.c_str());
        dp::Columns pCols = pSelect->allocBindColumns(2);
        td::String name;
        td::INT4 id;
        pCols << "ID" << id << "Name" << name;
        if (!pSelect->execute())
            return false;

        while (pSelect->moveNext())
        {
            combo.addItem(name, id);
        }
        combo.selectIndex(0);
        return true;
    }

    //gui::DBComboBox& getCombobox() {
    //    return _department;
    //}

    void getDepartment()
    {
        loadComboBox("select ID_Smjera as ID, Naziv_Smjera as Name from Smjer", _department);
    }

    td::INT4 getSemester() const
    {
        td::Variant semestar;
        _semester.getValue(semestar);
        if (semestar > 0 && semestar != 0 && semestar < 30)
            return semestar.i4Val();
        else
            return 1;
    }

    td::INT4& getDepartmentID()
    {
        td::Variant val;
        _department.getValue(val);
        return val.i4Val();

    }
};
