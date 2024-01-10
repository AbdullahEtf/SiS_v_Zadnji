//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Application.h>
#include <dp/IDatabase.h>
#include "MainWindow.h"

class Application : public gui::Application
{
protected:
    
    gui::Window* createInitialWindow() override
    {
        return new MainWindow();
    }
    
public:
    Application(int argc, const char** argv)
    : gui::Application(argc, argv)
    {
    }

    bool connectToDB()
    {

        dp::IDatabase* pDB = dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3);

        fo::fs::path home;
        mu::getHomePath(home);

        fo::fs::path DBPath = (home / "other_bin/TestData/natGUITest/Temp_Baza.db");

        if (!pDB->connect(DBPath.string().c_str()))
            return false;

        dp::setMainDatabase(pDB);
        return true;
    }
};
