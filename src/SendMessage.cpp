#pragma once
#include "SendMessage.h"
#include "ViewIDs.h"
#include "Globals.h"

bool MsgSender::sendMsgtoUser(td::String subject, td::String poruka, td::INT4 userID)
{
    td::INT4 aid = Globals::_currentUserID;
    bool a = ponovljeno(aid, subject, poruka, userID);
    return a;
}

bool MsgSender::sendMsgtoUsers(td::String subject, td::String poruka, std::vector<td::INT4> userIDs)
{
    td::INT4 aid = Globals::_currentUserID;
    bool a = ponovljeno2(aid, subject, poruka, userIDs);
    return a;
}

bool MsgSender::sendSystemMsgtoUser(td::String subject, td::String poruka, td::INT4 userID)
{
    td::INT4 aid = -1; //privatna funkcija prima aid;
    bool a = ponovljeno(aid, subject, poruka, userID);
    return a;
}

bool MsgSender::sendSystemMsgtoUsers(td::String subject, td::String poruka, std::vector<td::INT4> userIDs)
{
    td::INT4 aid = -1;
    bool a = ponovljeno2(aid, subject, poruka, userIDs);
    return a;
}
bool MsgSender::ponovljeno(td::INT4 a, td::String subject, td::String poruka, td::INT4 userID) {

    td::INT4 maxID;
    td::Date date(true);
    td::Time time(true);
    //todo

    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID), 0) as maxid from Messages");
    dp::Columns pColumns = pSelect->allocBindColumns(1);

    pColumns << "maxid" << maxID;
    if (!pSelect->execute())
        return false;
    if (!pSelect->moveNext())
        return false;

    ++maxID;
    dp::IStatementPtr pInsert(dp::getMainDatabase()->createStatement("insert into Messages(ID, AuthorID, Subject, Poruke, Datum, Vrijeme ) values(?,?,?,?,?,?)"));
    dp::Params pParams(pInsert->allocParams());
    pParams << maxID << a << dp::toNCh(subject, MESSAGE_HEADER_LEN) << dp::toNCh(poruka, MESSAGE_BODY_LEN) << date << time;

    dp::IStatementPtr pInsert2(dp::getMainDatabase()->createStatement("insert into MsgReceivers(MsgID, UserID) values(?,?)"));
    dp::Params pParams2(pInsert2->allocParams());
    pParams2 << maxID << userID;


    dp::Transaction tr(dp::getMainDatabase());

    if (!pInsert->execute())
        return false;
    if (!pInsert2->execute())
        return false;

    return tr.commit();
}

bool MsgSender::ponovljeno2(td::INT4 a, td::String subject, td::String poruka, std::vector<td::INT4> userIDs) {
    /*std::vector<td::INT4> test;
    for(int i = 0; i < 3; i++){
        test.push_back(userIDs[i]);
    }*/
    dp::IStatementPtr pSelect = dp::getMainDatabase()->createStatement("select ifnull(max(ID), 0) as maxid from Messages");
    dp::Columns pColumns = pSelect->allocBindColumns(1);
    td::INT4 maxID;
    pColumns << "maxid" << maxID;
    if (!pSelect->execute())
        return false;
    if (!pSelect->moveNext())
        return false;
    td::INT4 aid = a;

    ++maxID;
    td::Date date(true);
    td::Time time(true);
    dp::IStatementPtr pInsert(dp::getMainDatabase()->createStatement("insert into Messages(ID, AuthorID, Subject, Poruke, Datum, Vrijeme) values(?,?,?,?,?,?)"));
    dp::Params pParams(pInsert->allocParams());
    pParams << maxID << aid << dp::toNCh(subject, MESSAGE_HEADER_LEN) << dp::toNCh(poruka, MESSAGE_BODY_LEN) << date << time;


    dp::IStatementPtr pInsert2(dp::getMainDatabase()->createStatement("insert into MsgReceivers(MsgID, UserID) values(?,?)"));
    dp::Params pParams2(pInsert2->allocParams());
    td::INT4 UserID;
    pParams2 << maxID << UserID;

    dp::Transaction tr(dp::getMainDatabase());
    if (!pInsert->execute())
        return false;

    //insert u recievers


    for (auto id : userIDs) {
        UserID = id;

        if (!pInsert2->execute())
            return false;

    }

    return tr.commit();
}
