#pragma once
#include <gui/ViewSwitcher.h>
#include "ViewActivity.h"
#include "ViewSubject.h"
#include "ViewDateTimeActivity.h"
#include "ViewTasks.h"

class SwitcherViewActivity : public gui::ViewSwitcher
{
private:
protected:
    ViewActivity _viewActivity;
    ViewDateTimeActivity _viewDateTime;
    ViewTasks _viewTasks;

public:
    SwitcherViewActivity(td::UINT4 subjectid)
        : gui::ViewSwitcher(3)
        , _viewActivity(subjectid)
        , _viewDateTime(subjectid) 
        , _viewTasks(subjectid)
    {
        addView(&_viewActivity, true);
        addView(&_viewDateTime);
        addView(&_viewTasks);
    }
};

