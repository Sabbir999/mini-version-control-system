#include "commitNodeList.h"

// ================  Global Functions  ==================
string gen_random(const int len)
{
    srand((unsigned)time(NULL) * getpid());
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

string get_time()
{
    time_t t = std::time(0);
    tm *now = std::localtime(&t);
    string dateTime = to_string(now->tm_year + 1900) + "/" +
                      to_string(now->tm_mon + 1) + "/" +
                      to_string(now->tm_mday) + " " +
                      to_string(now->tm_hour) + ":" +
                      to_string(now->tm_min) + "\n";

    return dateTime;
}
// =========================================================

// commitNode class methods

void commitNode::createCommitNode()
{
    filesystem::create_directory(filesystem::current_path() / ".git" / "commits" / commitID);
    auto path = filesystem::current_path() / ".git" / "commits" / commitID / "commitInfo.txt";
    ofstream write(path.string());
    write << "1." + commitID + "\n" +
                 "2." + commitMsg + "\n" +
                 "3." + get_time() + "\n";

    auto STAGING_AREA_PATH = filesystem::path(filesystem::current_path() / ".git" / "staging_area");
    const auto copyOptions = filesystem::copy_options::update_existing | filesystem::copy_options::recursive;
    filesystem::copy(STAGING_AREA_PATH, filesystem::current_path() / ".git" / "commits" / commitID / "Data", copyOptions);
}

commitNode::commitNode()
{
    this->next = NULL;
}

commitNode::commitNode(string commitID, string commitMsg)
{
    this->commitID = commitID;
    this->commitMsg = commitMsg;
    this->next = NULL;
    createCommitNode();
}

commitNode::commitNode(string commitId)
{
    this->commitID = commitId;
    next = NULL;
    readNodeInfo();
}

void commitNode::revertCommitNode(string fromHash)
{
    filesystem::create_directories(filesystem::current_path() / ".git" / "commits" / getCommitID() / "Data");
    auto nextCommitPath = filesystem::current_path() / ".git" / "commits" / getCommitID() / "commitInfo.txt";
    auto copyFrom = filesystem::current_path() / ".git" / "commits" / fromHash / "Data";
    ofstream write(nextCommitPath.string());
    write << "1." + commitID + "\n" +
                 "2." + commitMsg + "\n" +
                 "3." + get_time() + "\n";
    const auto copyOptions = filesystem::copy_options::recursive;
    filesystem::copy(copyFrom, filesystem::current_path() / ".git" / "commits" / getCommitID() / "Data", copyOptions);
}

void commitNode::readNodeInfo()
{
    string info;
    auto path = filesystem::current_path() / ".git" / "commits" / getCommitID() / "commitInfo.txt";
    if (filesystem::exists(path)) {
        ifstream file(path.string());
        while (getline(file, info))
        {
            if (info[0] == '1')
            {
                this->setCommitID(info.substr(2));
            }
            if (info[0] == '2')
            {
                this->setCommitMsg(info.substr(2));
            }
        }
        file.close();
    }
}

string commitNode::getCommitMsg()
{
    return this->commitMsg;
}

void commitNode::setCommitMsg(string commitMsg)
{
    this->commitMsg = commitMsg;
}

void commitNode::setCommitID(string commitID)
{
    this->commitID = commitID;
}

string commitNode::getCommitID()
{
    return this->commitID;
}

void commitNode::setNext(commitNode *node)
{
    this->next = node;
}

commitNode *commitNode::getNext()
{
    return next;
}

void commitNode::setNextCommitID(string nextCommitId)
{
    this->nextCommitID = nextCommitId;
}

void commitNode::writeNextCommitID(string nextCommitID)
{
    setNextCommitID(nextCommitID);
    auto path = filesystem::current_path() / ".git" / "commits" / getCommitID() / "nextCommitInfo.txt";
    ofstream write(path.string());
    write << nextCommitID;
}

string commitNode::checkNextCommitId()
{
    filesystem::path tempPath(filesystem::current_path() / ".git" / "commits" / getCommitID() / "nextCommitInfo.txt");
    if (filesystem::exists(tempPath)) {
        string info;
        ifstream file(tempPath);
        while (getline(file, info))
        {
            this->nextCommitID = info;
            break;
        }
        file.close();
        return nextCommitID;
    }
    return "";
}

string commitNode::getNextCommitId()
{
    return this->nextCommitID;
}

// commitNodeList class methods

bool commitNodeList::checkHead()
{
    auto tempDir = filesystem::current_path() / ".git" / "commits" / "0x1111";
    return filesystem::exists(tempDir);
}

commitNodeList::commitNodeList()
{
    HEAD = NULL;
    TAIL = NULL;
    
    if (checkHead())
    {
        HEAD = new commitNode("0x1111");
        commitNode* curr = HEAD;
        
        while (true) {
            string nextId = curr->checkNextCommitId();
            if (nextId.empty() || nextId.length() < 8) {
                TAIL = curr;
                break;
            }
            
            commitNode* nextNode = new commitNode(nextId);
            curr->setNext(nextNode);
            curr = nextNode;
        }
    }
}

commitNode *commitNodeList::getHead()
{
    return HEAD;
}

void commitNodeList::setHead(commitNode *HEAD)
{
    this->HEAD = HEAD;
}

commitNode *commitNodeList::getTail()
{
    return TAIL;
}

void commitNodeList::setTail(commitNode *TAIL)
{
    this->TAIL = TAIL;
}

void commitNodeList::addOnTail(string msg)
{
    if (HEAD == NULL)
    {
        string commitID = "0x1111";
        commitNode *newNode = new commitNode(commitID, msg);
        HEAD = newNode;
        TAIL = newNode;
    }
    else
    {
        string commitID = gen_random(8);
        commitNode *newNode = new commitNode(commitID, msg);
        TAIL->setNext(newNode);
        TAIL->writeNextCommitID(commitID);
        TAIL = newNode;
    }
}

void commitNodeList::revertCommit(string commitHash)
{
    if (HEAD == NULL)
    {
        cout << "Nothing to Revert to " << endl;
        return;
    }

    commitNode* target = NULL;
    commitNode* curr = HEAD;
    while (curr != NULL)
    {
        if (curr->getCommitID() == commitHash)
        {
            target = curr;
            break;
        }
        curr = curr->getNext();
    }

    if (target == NULL)
    {
        cout << commitHash << " does not match any Commit Hash" << endl;
        return;
    }

    string newCommitId = gen_random(8);
    commitNode *newNode = new commitNode();
    newNode->setCommitID(newCommitId);
    newNode->setCommitMsg("Revert to " + commitHash + ": " + target->getCommitMsg());
    newNode->revertCommitNode(target->getCommitID());

    TAIL->setNext(newNode);
    TAIL->writeNextCommitID(newCommitId);
    TAIL = newNode;
}

void commitNodeList::printCommitList()
{
    commitNode *currNode = HEAD;
    while (currNode != NULL)
    {
        filesystem::path commitPath(filesystem::current_path() / ".git" / "commits" / currNode->getCommitID() / "commitInfo.txt");
        if (filesystem::exists(commitPath)) {
            string info;
            ifstream file(commitPath.string());
            while (getline(file, info))
            {
                if (info[0] == '1')
                    cout << "Commit ID:    " << info.substr(2) << endl;
                if (info[0] == '2')
                    cout << "Commit Msg:   " << info.substr(2) << endl;
                if (info[0] == '3')
                    cout << "Date & Time:  " << info.substr(2) << endl;
            }
            file.close();
            cout << "============================\n\n";
        }
        currNode = currNode->getNext();
    }
}