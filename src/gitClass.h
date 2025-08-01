#ifndef GITCLASS_H
#define GITCLASS_H

#include <string>
#include "commitNodeList.h"

class gitClass
{
public:
    commitNodeList list;
    void gitInit();
    void gitAdd();
    void gitAdd(std::string files[], int arrSize);
    void gitCommit(std::string msg);
    void gitRevert(std::string commitHash);
    void gitLog();
    // void gitStatus();
};

#endif
