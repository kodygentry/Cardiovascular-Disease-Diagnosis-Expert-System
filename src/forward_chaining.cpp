#include <string>
#include <iostream>
#include "forward_chaining.h"

ForwardChaining::ForwardChaining(){

}

void ForwardChaining::init() {
    for(int i = 0; i < 100; i++) {
        clauseVarList[i] = "";
    }

    clauseVarList[1] = "HD"; //10

    clauseVarList[5] = "HD"; //20
    clauseVarList[6] = "DN";

    clauseVarList[9] = "HD"; //30
    clauseVarList[10] = "DN";

    clauseVarList[13] = "HD"; //40
    clauseVarList[14] = "DN";

    clauseVarList[17] = "HD"; //50
    clauseVarList[18] = "DN";

    clauseVarList[21] = "HD"; //60
    clauseVarList[22] = "DN";

    clauseVarList[25] = "HD"; //70
    clauseVarList[26] = "DN";

    clauseVarList[29] = "HD"; //80
    clauseVarList[30] = "DN";

    clauseVarList[33] = "HD"; //90
    clauseVarList[34] = "DN";

    clauseVarList[37] = "HD"; //100
    clauseVarList[38] = "DN";

    clauseVarList[41] = "HD"; //110
    clauseVarList[42] = "DN";

    clauseVarList[45] = "HD"; //120
    clauseVarList[46] = "DN";

    clauseVarList[49] = "HD"; //130
    clauseVarList[50] = "DN";

    clauseVarList[53] = "HD"; //140
    clauseVarList[54] = "DN";

    clauseVarList[57] = "HD"; //150
    clauseVarList[58] = "DN";

    clauseVarList[61] = "HD"; //160
    clauseVarList[62] = "DN";

    clauseVarList[65] = "HD"; //170
    clauseVarList[66] = "DN";

    clauseVarList[69] = "HD"; //180
    clauseVarList[70] = "DN";

    clauseVarList[73] = "HD"; //190
    clauseVarList[74] = "DN";

    variableList["HD"] = "";
    variableList["DN"] = "";
    variableList["TR"] = "";

    instantiatedList["HD"] = 0;
    instantiatedList["DN"] = 0;

    /* askQuestion("HD");
    conclusionVarQueue.push("HD");

    int searchedIndex = -1;
    checkInstantiatedList(searchedIndex);*/
}

void ForwardChaining::provideDisease(std::string hasDisease, std::string diseaseName) {
    instantiatedList["HD"] = 1;
    variableList["HD"] = hasDisease;
    conclusionVarQueue.push("HD");

    instantiatedList["DN"] = 1;
    variableList["DN"] = diseaseName;
    conclusionVarQueue.push("DN");

    int searchedIndex = -1;
    checkInstantiatedList(searchedIndex);
}

void ForwardChaining::checkInstantiatedList(int& searchedIndex) {
    std::string firstConcVar = "";
    if(!conclusionVarQueue.empty()) {
        firstConcVar = conclusionVarQueue.front();
    }

    if(firstConcVar != "") {
        int ruleNumber = 0;
        for(int i = 0; i < 100; i++) { //searches for the rule number, and sets the clause variable pointer
            if(clauseVarList[i] == firstConcVar) {
                if(i > searchedIndex) {
                    searchedIndex = i;
                    ruleNumber = ((i / 4) + 1) * 10;

                    clauseVarPointer.first = ruleNumber;
                    clauseVarPointer.second = 1;

                    int firstClause = (((clauseVarPointer.first / 10) - 1) * 4);
                    while(clauseVarList[firstClause + clauseVarPointer.second] != "") {
                        std::string aVar = clauseVarList[firstClause + clauseVarPointer.second];
                        if(instantiatedList[aVar] == 0) { //this wouldn't be ran since its being instantiated before
                            askQuestion(aVar);
                        }
                        clauseVarPointer.second += 1;
                    }
                    findTreatment(ruleNumber);
                    checkInstantiatedList(searchedIndex);
                    break;
                }
            }
        }
    }
}

void ForwardChaining::askQuestion(std::string missing) {
    if(missing == "HD") {
        std::cout << "Do you have a heart disease? (Yes/No): ";
        getline(std::cin, variableList[missing]);
        instantiatedList[missing] = 1;
    } else if(missing == "DN") {
        std::cout << "What disease do you have? ";
        getline(std::cin, variableList[missing]);
        instantiatedList[missing] = 1;
    }
}

std::string ForwardChaining::returnTreatment() {
    return variableList["TR"];
}

void ForwardChaining::findTreatment(int ruleNumber) {
    switch(ruleNumber) {
        case 10:
            if(variableList["HD"] == "No") {
                variableList["TR"] = "No treatment needed.";
            }
            break;
        case 20:
            if(variableList["HD"] == "Yes" && variableList["DN"] == "coronary artery disease") {
                variableList["TR"] = "Bypass Surgery";
            }
            break;
        case 30:
            if(variableList["HD"] == "Yes" && variableList["DN"] == "heart attack") {
                variableList["TR"] = "Bypass Surgery";
            }
            break;
        case 40:
            if(variableList["HD"] == "Yes" && variableList["DN"] == "sudden cardiac arrest") {
                variableList["TR"] = "CPR";

            }
            break;
        case 50:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "cardiomegaly") {
                variableList["TR"] = "Medications";
            }
            break;
        case 60:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "arrhythmia") {
                variableList["TR"] = "Beta Blocker Medicine";
            }
            break;
        case 70:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "atrial fibrillation") {
                variableList["TR"] = "Cardioversion";
            }
            break;
        case 80:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "heart valve disease") {
                variableList["TR"] = "Heart Valve Replacement/Repair";
            }
            break;
        case 90:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "congenital heart disease") {
                variableList["TR"] = "Intracardiac repair surgery";
            }
            break;
        case 100:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "dilated cardiomyopathy") {
                variableList["TR"] = "Heart Transplant";
            }
            break;
        case 110:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "ischemic cardiomyopathy") {
                variableList["TR"] = "Cardiac Resynchronization Therapy";
            }
            break;
        case 120:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "restrictive cardiomyopathy") {
                variableList["TR"] = "Medications";
            }
            break;
        case 130:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "pericarditis") {
                variableList["TR"] = "Corticosteroids";
            }
            break;
        case 140:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "pericardial effusion") {
                variableList["TR"] = "Pericardiectomy";
            }
            break;
        case 150:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "marfan syndrome") {
                variableList["TR"] = "Medications";
            }
            break;
        case 160:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "heart murmurs") {
                variableList["TR"] = "Valve repair/replacment";
            }
            break;
        case 170:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "hypertrophic cardiomyopathy") {
                variableList["TR"] = "Septal Myectomy";
            }
            break;
        case 180:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "abdominal aortic aneurysm") {
                variableList["TR"] = "Endovascular repair";
            }
            break;
        case 190:
           if(variableList["HD"] == "Yes" && variableList["DN"] == "cardiomyopathy") {
                variableList["TR"] = "Surgery";
            }
            break;
         default:
            variableList["TR"] = "No treatment found.";
            break;
    }

    if(variableList["TR"] != "") {
        conclusionVarQueue.pop();
        conclusionVarQueue.push("TR");
        //cout << "Treatment = " << variableList["TR"] << endl;
    }
}
