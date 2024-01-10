#pragma once
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <mu/IAppSettings.h>
#include <gui/Image.h>
#include <gui/Frame.h>




void examAttendance(const gui::Image*, td::INT4 SubjectID);
void examGrades(const gui::Image*, td::INT4 SubjectID); 
void homeworkGrades(const gui::Image*, td::INT4 SubjectID);
void labGrades(const gui::Image*, td::INT4 SubjectID);