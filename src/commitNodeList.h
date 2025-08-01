#ifndef COMMITNODELIST_H
#define COMMITNODELIST_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <ctime>
#include <string>

using namespace std;

string gen_random(const int len);
string get_time();

class commitNode
{
private:
    string commitID;
    string commitMsg;
    string nextCommitID;
    commitNode *next;

    void createCommitNode();

public:
    commitNode();
    commitNode(string commitID, string commitMsg);
    commitNode(string commitId);

    void revertCommitNode(string fromHash);
    void readNodeInfo();

    string getCommitMsg();
    void setCommitMsg(string commitMsg);

    void setCommitID(string commitID);
    string getCommitID();

    void setNext(commitNode *node);
    commitNode *getNext();

    void setNextCommitID(string nextCommitId);
    void writeNextCommitID(string nextCommitID);
    string checkNextCommitId();
    string getNextCommitId();
};

class commitNodeList
{
private:
    commitNode *HEAD;
    commitNode *TAIL;

    bool checkHead();

public:
    commitNodeList();

    commitNode *getHead();
    void setHead(commitNode *HEAD);

    commitNode *getTail();
    void setTail(commitNode *TAIL);

    void addOnTail(string msg);
    void revertCommit(string commitHash);
    void printCommitList();
};

#endif
