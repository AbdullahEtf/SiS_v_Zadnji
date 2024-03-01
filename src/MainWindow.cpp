#pragma once
#include "MainWindow.h"
#include "ViewIDs.h"
#include "DialogLogin.h"
#include "ViewDepartment.h"
#include "ViewUsers.h"
#include "ViewCourse.h"
#include "ViewClassroom.h"
#include "ViewTeachingStaff.h"
#include "ViewActivity.h"
#include "ViewEnroll.h"
#include "SetPasswordWindow.h"
#include "DialogChooseSubject.h"
#include "ViewCurriculum.h"
#include "DialogChooseAllSubjects.h"
#include "Reports.h"
#include "ViewExamSignUp.h"
#include "ViewCourseEnroll.h"
#include "ViewTicket.h"
#include "ViewCurriculumDialog.h"
#include "DialogCurriculum.h"
#include "ViewMessages.h"
#include "ViewGradeExams.h"
#include "upload.h"
#include "DialogChooseSubjectForTimeSlot.h"
#include <rpt/IResources.h>
#include "NavigatorViewActivity.h"
#include "ViewTimeSlot.h"
#include "ViewGradeLabHomework.h"
#include "CanvasETF.h"
#include "ViewTicketFORSAO.h"
#include "ViewFinalGrade.h"


MainWindow::MainWindow()
    : gui::Window(gui::Geometry(100, 100, 1000, 600))
    , _mainView()
    , _imgUsers(":complex")
    , _imgDepartment(":complex")
    , _imgCourse(":complex")
    , _imgClassroom(":complex")
    , _imgTStaff(":complex")
    , _imgActivity(":complex")
    , _imgEnroll(":complex")
    , _imgAttendance(":complex")
    , _imgCurr(":complex")
    , _imgExamAtt(":complex")
    , _imgTicket(":pencil")
    , _imgCourseenr(":plus")
    , _imgMessages(":complex")
    , _imgExamGrades(":complex")
    , _imgExamLabHomework(":complex")
    , _imgSAOTicket(":pencil")
    , _imgUpload(":complex")
{
    setTitle(tr("SIS"));
    _mainMenuBar.setAsMain(this);
    setCentralView(&_mainView);
    //setCentralView(&_etf);

    setStatusBar(&_statBar);

    //load report resources
    rpt::IResources* pRes = rpt::IResources::getManager();

    //RepConfig.xml must be located in res subfolder
    if (!pRes->loadFromXML("MutableResources.xml"))
    {
        assert(false);
        return;
    }

}

void MainWindow::showLogin()
{

    // pDlg->openModal();
    // pDlg->setTitle(tr("Login"));
    // -----------------------------OVO SAM JA ZAKOMENTARISALA
    DialogLogin* pDlg = new DialogLogin(this);
    pDlg->setTitle(tr("Login"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                _statBar.UpdateStatusBar();
                _mainView.reset();
                return true;
            }
            else
            {
              
                close();
            }
            return true;
        });

    //pDlg->openModal();
    //pDlg->setTitle(tr("Login"));
}

//bool MainWindow::onClick(gui::Dialog* pDlg, td::UINT4 dlgID)
//{
//    if ((DlgID)dlgID == DlgID::Login)
//    {
//        auto btnID = pDlg->getClickedButtonID();
//        if (btnID == gui::Dialog::Button::ID::OK) {
//            UpdateMenu();
//        }
//        else
//            close();
//
//        return true;
//    }
//    return false;
//}




bool MainWindow::showSubjectChoose()
{     
    if (!Globals::isAdmin && !Globals::isProfessor && !Globals::isAssistant)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;

    }

    DialogChooseSubject* pDlg = new DialogChooseSubject(this);
    pDlg->setTitle(tr("SubjectChoose"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                auto dlgCS = static_cast<DialogChooseSubject*> (pDlg);
                if (dlgCS->getComboBox().getNoOfItems() == 0)
                {
                    showAlert(tr("alert"), tr("alertNoProfesorSubject"));
                    return false;
                }
                showAttendanceView(dlgCS->getSubjectID());
            }
             return true;
        });

    //pDlg->openModal(DlgID::Login, this);
    return false;
}



bool MainWindow::showSubjectChooseForTimeSlot()                                         ///
{   
    if (!Globals::isAdmin && !Globals::isStudent)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;

    }

    DialogChooseSubjectForTimeSlot* pDlg = new DialogChooseSubjectForTimeSlot(this);
    pDlg->setTitle(tr("SubjectChoose"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                auto dlgCS = static_cast<DialogChooseSubjectForTimeSlot*> (pDlg);
                if (dlgCS->getComboBox().getNoOfItems() == 0)
                {
                    showAlert(tr("alert"), tr("alertNoSubject"));
                    return false;
                }
                showTimeSlotView(dlgCS->getSubjectID());                              ///
            }
             return true;
        });
    return false;
}
bool MainWindow::showTimeSlotView(td::INT4 SubjectID)
{
    td::INT4 ViewID = (View_TIMESLOT << 24) | SubjectID;
    if (focusOnViewPositionWithID(ViewID))
        return true;

  
    //showSubjectChooseForTimeSlot();

   // NavigatorView* pView = new NavigatorView(ViewID, SubjectID);
    ViewTimeSlot* pView = new ViewTimeSlot(SubjectID, ViewID);              ///
    td::String a = (tr("viewTimeSlot"));
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv FROM Predmet a WHERE a.ID_Predmeta = ?");
    dp::Params pParams(pSelect->allocParams());
    pParams << SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String name;
    pCols << "Naziv" << name;
    if (!pSelect->execute())
        return false;

    if (!pSelect->moveNext())
        return false;
    a += " - ";
    a += name;

    _mainView.addView(pView, a, &_imgAttendance);
    return true;
}

bool MainWindow::showCurriculum()
{
    auto x = Globals::_currentUserRole;
    if (x != 6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    DialogCurriculum* pDlg = new DialogCurriculum(this);
    pDlg->setTitle(tr("Choose semester and department"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                auto dlgCS = static_cast<DialogCurriculum*> (pDlg);
                showCurriculumView(dlgCS->getDepartmentID(), dlgCS->getSemester());
            }
             return true;
        });

    //pDlg->openModalWithID(DlgID::Login, this);
    return false;
}


bool MainWindow::showSubjectChooseActivty()
{
    auto x = Globals::_currentUserRole;
    if (x != 1 && x != 3 && x != 6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }

    DialogChooseSubject* pDlg = new DialogChooseSubject(this);
    pDlg->setTitle(tr("SubjectChoose"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                auto dlgCS = static_cast<DialogChooseSubject*> (pDlg);
                showActivityView(dlgCS->getSubjectID());
            }
            return true;
        });

    //pDlg->openModal(DlgID::Login, this);
    return false;
}


bool MainWindow::showMySubjectChoose()
{
    auto x = Globals::_currentUserRole;
    if (x != 1 && x != 6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    DialogChooseSubject* pDlg = new DialogChooseSubject(this);
    pDlg->setTitle(tr("SubjectChoose"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                auto dlgCS = static_cast<DialogChooseSubject*> (pDlg);
                examAttendance(&_imgExamAtt, dlgCS->getSubjectID());
            }
            return true;
        });

    return false;
}


bool MainWindow::showAllSubjectChoose()
{
    auto x = Globals::_currentUserRole;
    if (x != 6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }

    DialogChooseSubject* pDlg = new DialogChooseSubject(this);
    pDlg->setTitle(tr("SubjectChoose"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                auto dlgCS = static_cast<DialogChooseSubject*> (pDlg);
                showTStaffView(dlgCS->getSubjectID());
            }
            return true;
        });

    return false;
}

bool MainWindow::showSomeSubjectChoose()
{
    auto x = Globals::_currentUserRole;
    if (x!=1 && x!=6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    DialogChooseSubject* pDlg = new DialogChooseSubject(this);
    pDlg->setTitle(tr("SubjectChoose"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                auto dlgCS = static_cast<DialogChooseSubject*> (pDlg);
                //examGrades(&_imgExamAtt, dlgCS->getSubjectID());

                showGradeExamView(dlgCS->getSubjectID());
            }
            return true;
        });

    return false;
}


bool MainWindow::onActionItem(gui::ActionItemDescriptor& aiDesc)
{
    auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();

    if (menuID == 10 && firstSubMenuID == 0 && lastSubMenuID == 0)
    {
        switch (actionID)
        {
        case 10:
        {
            if (_mainView.getNumberOfViews() > 0) return false;
            Globals::_currentUserRole = -1;
            Globals::_currentUserID = -1;
            //Globals::_IDSubjectSelection = -1;
            // po potrebi ovjde dodati 
            //  Globals::isLogged = false;
            Globals::isAdmin = false;
            Globals::isProfessor = false;
            Globals::isAssistant = false;
            Globals::isStudent = false;
            Globals::isSAO = false;
            _mainView.reset();
            showLogin();
            return true;
        }
        case 20:
        {
            gui::Window* pParentWnd = getParentWindow();
            auto pWnd = new SetPasswordWindow(pParentWnd, 2);
            pWnd->keepOnTopOfParent();
            pWnd->open();
            return true;
        }
        }
    }

    //ovo je meni za poglede
    if (menuID == 20 && firstSubMenuID == 0 && lastSubMenuID == 0)
    {

        switch (actionID)
        {
        break; case 10: return showUsersView();
        break; case 20: return showDepartmentsView();
        break; case 30: return showCoursesView();
        break; case 40: return showClassroomView();
        break; case 50: return showAllSubjectChoose();
        break; case 60: return showSubjectChooseActivty();
        break; case 70: return showEnrollView();
      //  break; case 80: return showSubjectChoose();
        break; case 90: return showCurriculum();
        break; case 100: showMySubjectChoose(); return true;
        break; case 110: return showExamSignUpView();
        break; case 120: return showCourseEnrollView();
        break; case 130: return showTicketView();
        break; case 140: return showMessagesView();
        break; case 150: return showSomeSubjectChoose();
        break; case 160: return showUpload();
        break; case 170: return showSubjectChooseForTimeSlot();
        break; case 180: return showSomeSubjectChoose2();
        break; case 190: return showTicketForSaoView();
        break; case 200: return showGradeSubjectChoose();






        break; default: break;
        }

    }

   

    
    if (menuID == 20 && firstSubMenuID == 30 && lastSubMenuID == 30) {        ///
        switch (actionID) {
        break; case 80: return showSubjectChoose();
        break; case 170: return showSubjectChooseForTimeSlot();
        }
    }




    //ovo neka ostane, ako smo zaboravili pozvati menu opciju iskocice nam ovo
    //ako ga stavimo u ovaj #ifdef, ovaj dio koda nece postojati u release modu
#ifdef MU_DEBUG
    td::String msgText("Handling onActionItem MainWindow");
    td::String informativeText;
    informativeText.format("Nedostaje implementacija za onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);

    showAlert(msgText, informativeText);
#endif

    return true;
}

bool MainWindow::focusOnViewPositionWithID(td::UINT4 viewID)
{
    for (int i = 0; i < _mainView.getNumberOfViews(); ++i)
    {
        if (auto pView = _mainView.getView(i); pView->getVisualID() == viewID)
        {
            _mainView.showView(i);
            return true;
        }

    }
    return false;
}

bool MainWindow::showUsersView()
{
    //if (!Globals::isAdmin)
    //{
    //   // gui::Alert("nemas prava");
    //   return true;
    //}

    if (!Globals::isAdmin)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }


    if (focusOnViewPositionWithID(View_USERS))
        return true;

    ViewUsers* pView = new ViewUsers;
    _mainView.addView(pView, tr("viewUsers"), &_imgUsers);
    return true;
}

bool MainWindow::showDepartmentsView()
{
    auto x = Globals::_currentUserRole;
    if (x != 6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    if (focusOnViewPositionWithID(View_DEPARTMENT))
        return true;

    ViewDepartment* pView = new ViewDepartment;
    _mainView.addView(pView, tr("viewDeps"), &_imgDepartment);
    return true;
}

bool MainWindow::showCoursesView()
{
    auto x = Globals::_currentUserRole;
    if (x != 6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    if (focusOnViewPositionWithID(View_COURSE))
        return true;

    ViewCourse* pView = new ViewCourse;
    _mainView.addView(pView, tr("viewCourse"), &_imgCourse);
    return true;
}

bool MainWindow::showClassroomView()
{
    if (!Globals::isAdmin)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
}
    if (focusOnViewPositionWithID(View_CLASSROOM))
        return true;

    ViewClassroom* pView = new ViewClassroom;
    _mainView.addView(pView, tr("viewClassroom"), &_imgClassroom);
    return true;
}

bool MainWindow::showTStaffView(td::INT4 SubjectID)
{
    td::INT4 ViewID = (View_TSTAFF << 24) | SubjectID;
    if (focusOnViewPositionWithID(ViewID))
        return true;

    ViewTeachingStaff* pView = new ViewTeachingStaff(SubjectID);
    _mainView.addView(pView, tr("viewTStaff"), &_imgTStaff);

    return true;
}

//bool MainWindow::showActivityView()
//{
//    if (focusOnViewPositionWithID(View_ACTIVITY))
//        return true;
//
//    ViewActivity* pView = new ViewActivity();
//    _mainView.addView(pView, tr("viewActivity"), &_imgActivity);
//    return true;
//}

bool MainWindow::showActivityView(td::INT4 SubjectID)
{
    td::INT4 ViewID = (View_ACTIVITY << 24) | SubjectID;
    if (focusOnViewPositionWithID(View_ACTIVITY))
        return true;

    NavigatorViewActivity* pView = new NavigatorViewActivity(ViewID, SubjectID);
    //ViewActivity* pView = new ViewActivity;
    _mainView.addView(pView, tr("viewActivity"), &_imgActivity);
    return true;
}


bool MainWindow::showEnrollView()
{
    if (!Globals::isSAO && !Globals::isAdmin)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }

    if (focusOnViewPositionWithID(View_ENROLL))
        return true;

    ViewEnroll* pView = new ViewEnroll;
    _mainView.addView(pView, tr("viewEnroll"), &_imgEnroll);
    return true;
}

bool MainWindow::showAttendanceView(td::INT4 SubjectID)
{
    //showSubjectChoose();
    td::INT4 ViewID = (View_ATTENDANCE << 24) | SubjectID;
    if (focusOnViewPositionWithID(ViewID))
        return true;

    NavigatorView* pView = new NavigatorView(ViewID, SubjectID);
    td::String a = (tr("viewAttendance"));
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select a.Naziv_Predmeta AS Naziv FROM Predmet a WHERE a.ID_Predmeta = ?");
    dp::Params pParams(pSelect->allocParams());
    pParams << SubjectID;
    dp::Columns pCols = pSelect->allocBindColumns(1);
    td::String name;
    pCols << "Naziv" << name;
    if (!pSelect->execute())
        return false;

    if (!pSelect->moveNext())
        return false;
    a += " - ";
    a += name;

    _mainView.addView(pView, a, &_imgAttendance);
    return true;
}

bool MainWindow::showCurriculumView(td::INT4 _departmentID, td::INT4 _semesterID)
{
    auto x = Globals::_currentUserRole;
    if (x != 6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    if (focusOnViewPositionWithID(View_CURRICULUM))
        return true;

    ViewCurriculum* pView = new ViewCurriculum(_departmentID, _semesterID);
    _mainView.addView(pView, tr("viewCurriculum"), &_imgCurr);
    return true;
}

bool MainWindow::showExamSignUpView()
{
    auto x = Globals::_currentUserRole;
    if (x != 5 && x != 6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }

    if (focusOnViewPositionWithID(View_EXAM))
        return true;

    ViewExamSignUp* pView = new ViewExamSignUp;
    _mainView.addView(pView, tr("viewExamSignUp"), &_imgCurr);
    return true;
}

bool MainWindow::showTicketView()
{
    if (!Globals::isStudent)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }

    if (focusOnViewPositionWithID(View_TICKET))
        return true;

    ViewTicket* pView = new ViewTicket;
    _mainView.addView(pView, tr("viewTicket"), &_imgTicket);
    return true;
}

bool MainWindow::showCourseEnrollView() {

    if (!Globals::isSAO && !Globals::isAdmin)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }

    if (focusOnViewPositionWithID(View_COURSEENROLL))
        return true;

    ViewCourseEnroll* pView = new ViewCourseEnroll;
    _mainView.addView(pView, tr("viewCourseEnroll"), &_imgCourseenr);
    return true;
}

bool MainWindow::showMessagesView() {

    //obavjestenjima smije svako pristupiti

    if (focusOnViewPositionWithID(View_MESSAGES))
        return true;

    ViewMessages* pView = new ViewMessages;
    _mainView.addView(pView, tr("viewMessages"), &_imgMessages);
    return true;
}

bool MainWindow::showGradeExamView(td::INT4 SubjectID)
{
    td::INT4 ViewID = (View_GRADE << 24) | SubjectID;
    if (focusOnViewPositionWithID(ViewID))
        return true;

    ViewGradeExams* pView = new ViewGradeExams(SubjectID);
    _mainView.addView(pView, tr("viewGradeExam"), &_imgExamGrades);

    return true;
}

bool MainWindow::showUpload()
{
    auto x = Globals::_currentUserRole;
    if (x != 6 && x != 5 )
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    if (focusOnViewPositionWithID(View_UPLOAD))
        return true;

    ViewUpload* pView = new ViewUpload;
    _mainView.addView(pView, tr("viewUpload"), &_imgUpload);
    return true;
}

bool MainWindow::showSomeSubjectChoose2()
{
    auto x = Globals::_currentUserRole;
    if (x != 3 && x !=6)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    DialogChooseSubject* pDlg = new DialogChooseSubject(this);
    pDlg->setTitle(tr("SubjectChoose"));
    pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK) {
                auto dlgCS = static_cast<DialogChooseSubject*> (pDlg);
                //examGrades(&_imgExamAtt, dlgCS->getSubjectID());

                showGradeLabHomeworkView(dlgCS->getSubjectID());
            }
            return true;
        });

    return false;
}

bool MainWindow::showGradeLabHomeworkView(td::INT4 SubjectID)
{
    td::INT4 ViewID = (View_GRADE_LABHOM << 24) | SubjectID;
    if (focusOnViewPositionWithID(ViewID))
        return true;

    ViewGradeLabHomework* pView = new ViewGradeLabHomework(SubjectID);
    _mainView.addView(pView, tr("viewGradeLabHw"), &_imgExamGrades);

    return true;
}


bool MainWindow::showTicketForSaoView() {

    if (!Globals::isSAO && !Globals::isAdmin)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }

    if (focusOnViewPositionWithID(View_SAOTICKET))
        return true;

    ViewTicketForSAO* pView = new ViewTicketForSAO();
    _mainView.addView(pView, tr("viewTicketForSAO"), &_imgSAOTicket);
    return true;
  


}

bool MainWindow::showGradeSubjectChoose(){

   
 if (!Globals::isProfessor && !Globals::isAdmin)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }


    DialogChooseSubject* pDlg = new DialogChooseSubject(this);
     pDlg->setTitle(tr("SubjectChoose"));
     pDlg->openModal([this](gui::Dialog::Button::ID btn, gui::Dialog* pDlg)
        {
        auto btnID = pDlg->getClickedButtonID();
        if (btnID == gui::Dialog::Button::ID::OK) {
            auto dlgCS = static_cast<DialogChooseSubject*> (pDlg);
            //examGrades(&_imgExamAtt, dlgCS->getSubjectID());

            showFinalGradeView(dlgCS->getSubjectID());
        }
            return true;
       });

    return false;
}


bool MainWindow::showFinalGradeView(td::INT4 SubjectID)
{

    if (!Globals::isProfessor && !Globals::isAdmin)
    {
        showAlert(tr("AccessNotAllowed"), "");
        return true;
    }
    
    td::INT4 ViewID = (View_FINALGRADE << 24) | SubjectID;
    if (focusOnViewPositionWithID(ViewID))
        return true;

    ViewFinalGrade* pView = new ViewFinalGrade(SubjectID);
    _mainView.addView(pView, tr("viewGradeExam"), &_imgExamGrades);

    return true;
}
