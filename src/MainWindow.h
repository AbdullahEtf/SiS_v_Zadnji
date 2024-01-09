//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "Globals.h"
#include "NavigatorView.h"
#include "MenuBar.h"
#include "DynTabView.h"
#include "StatusBar.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    DynTabView _mainView;
    gui::Image _imgUsers;
    gui::Image _imgDepartment;
    gui::Image _imgCourse;
    gui::Image _imgClassroom;
    gui::Image _imgTStaff;
    gui::Image _imgActivity;
    gui::Image _imgEnroll;
    gui::Image _imgAttendance;
    gui::Image _imgCurr;
    gui::Image _imgExamAtt;
    gui::Image _imgTicket;
    gui::Image _imgCourseenr;
    gui::Image _imgMessages;
    gui::Image _imgExamGrades;
    gui::Image _imgUpload;

    StatusBar _statBar;


protected:
    void onInitialAppearance() override
    {
        showLogin();
    }


public:
    MainWindow();
public:
    void showLogin();
    bool showSubjectChoose();
    bool showMySubjectChoose();
    bool showAllSubjectChoose();
    bool showSubjectChooseActivty();
    bool showSomeSubjectChoose();
    bool showCurriculum();


protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
    bool focusOnViewPositionWithID(td::UINT4 viewID);
    bool showUsersView();
    bool showDepartmentsView();
    bool showCoursesView();
    bool showClassroomView();
    bool showTStaffView(td::INT4 SubjectID);
    bool showActivityView(td::INT4 SubjectID);
    bool showEnrollView();
    bool showAttendanceView(td::INT4 SubjectID);
    bool showCurriculumView(td::INT4 _departmentID, td::INT4 _semesterID);
    bool showExamSignUpView();
    bool showTicketView();
    bool showCourseEnrollView();
    bool showMessagesView();
    bool showGradeExamView(td::INT4 SubjectID);
    bool showUpload();
    
    //bool onClick(gui::Dialog* pDlg, td::UINT4 dlgID) override;



};
