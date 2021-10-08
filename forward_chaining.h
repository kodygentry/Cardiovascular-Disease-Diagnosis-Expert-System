#ifndef FORWARD_CHAINING_H
#define FORWARD_CHAINING_H
#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>

class ForwardChaining
{
private:
    std::string clauseVarList[200];
    std::queue<std::string> conclusionVarQueue;
    std::pair<int, int> clauseVarPointer; //rule number, clause number
    std::unordered_map<std::string, int> instantiatedList;
    std::unordered_map<std::string, std::string> variableList;

public:
    ForwardChaining();
    void init();
    void provideDisease(std::string, std::string);
    void checkInstantiatedList(int&);
    void askQuestion(std::string);
    std::string returnTreatment();
    void findTreatment(int);

};

#endif
