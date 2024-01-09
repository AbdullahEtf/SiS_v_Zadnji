//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/MenuBar.h>

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu application;
    gui::SubMenu sifarnici;
protected:

public:


    void populateAppMenu()
    {
        auto& items = application.getItems();
        items[0].initAsQuitAppActionItem(tr("Quit")); //prevedeno u natGUI
        items[1].initAsActionItem(tr("logout"), 10);
    }

    void populateSifMenu() {
        auto& items = sifarnici.getItems();
        items[0].initAsActionItem(tr("viewUsers"), 10);
        items[1].initAsActionItem(tr("viewDeps"), 20);
        items[2].initAsActionItem(tr("viewCourse"), 30);
        items[3].initAsActionItem(tr("viewClassroom"), 40);
        items[4].initAsActionItem(tr("viewTStaff"), 50);
        items[5].initAsActionItem(tr("viewActivity"), 60);
        items[6].initAsActionItem(tr("viewEnroll"), 70);
        items[7].initAsActionItem(tr("viewAttendance"), 80);
        items[8].initAsActionItem(tr("viewCurriculum"), 90);
        items[9].initAsActionItem(tr("viewExamAtt"), 100);
        items[10].initAsActionItem(tr("viewExamSignUp"), 110);
        items[11].initAsActionItem(tr("viewCourseEnroll"), 120);
        items[12].initAsActionItem(tr("viewTicket"), 130);
        items[13].initAsActionItem(tr("viewMessages"), 140);
        items[14].initAsActionItem(tr("viewExamGrade"), 150);
        items[15].initAsActionItem(tr("viewUpload"), 160);

    }
   
public:
    MenuBar()
    : gui::MenuBar(2) 
    , application(10, tr("App"), 2)
    , sifarnici(20, tr("Sif"), 16)

    {
        populateAppMenu();
        populateSifMenu();

        setMenu(0, &application);
        setMenu(1, &sifarnici);
    
    }
    
    ~MenuBar()
    {
    }
};
