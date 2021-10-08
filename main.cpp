#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include <utility>
#include <chrono>
#include "forward_chaining.h"

std::string rulesList(std::stack<std::pair<int, int>> &, std::unordered_map<std::string, int> &);
void searchClauseVar(std::stack<std::pair<int, int>> &,
                   std::vector<std::pair<std::string, int>>,
                   std::vector<std::string>,
                   std::unordered_map<std::string, int> &);
void instVar(std::unordered_map<std::string, int> &, std::string);
std::vector<std::string> initClVarLt();
std::unordered_map<std::string, int> initVarLt();
std::vector<std::pair<std::string, int>> initConclT();
std::string bwInfEng(std::vector<std::pair<std::string, int>>,
                   std::unordered_map<std::string, int> &,
                   std::vector<std::string>);
// Test/debug Functions
// Helper Functions
bool isHeartDisease(std::pair<std::string, int>);
int findSubconclusion(std::vector<std::pair<std::string, int>>, std::string);

int main(){
  // start clock
  auto start = std::chrono::high_resolution_clock::now();
  //Init Variable List
  std::unordered_map<std::string, int> varmap = initVarLt();
  //Init Clause Variable List
  std::vector<std::string> clvarlt = initClVarLt();
  //Init Conclusion List
  std::vector<std::pair<std::string, int>> conclT = initConclT();
  std::string diagnosis = bwInfEng(conclT, varmap, clvarlt);
  std::cout << "Disease is: " << diagnosis << std::endl;

  std::string hasDisease = (diagnosis == "not heart disease") ? "No" : "Yes";
  ForwardChaining fw;
  fw.init();
  fw.provideDisease(hasDisease, diagnosis);
  std::string treatment = fw.returnTreatment();
  std::cout << "Treatment is: " << treatment << std::endl;

  // end clock, calc time, display
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
  std::cout << "\nExecution time: " << duration.count() << " seconds." << std::endl;

  return 0;
}

std::string bwInfEng(std::vector<std::pair<std::string, int>> conclT,
                   std::unordered_map<std::string, int> &varmap,
                   std::vector<std::string> clvarLt) {
  std::string disease = "not heart disease";
  std::string cont;
  //conclusion stack has pair of ints, first int is conclusion number, 2nd int is clause number
  std::stack<std::pair<int, int>> conSt;
  //find heart disease
  auto it = std::find_if(conclT.begin(), conclT.end(), isHeartDisease);
  //push first instance of heart disease onto stack
  conSt.push(std::make_pair(it -> second, 4*((it -> second/10) - 1)));
  it++;

  std::cout << "------ Searching for heart disease ------\n";
  while((it != conclT.end() || !conSt.empty()) && disease == "not heart disease"){
    //instVar(conSt, conclT, clvarLt, varmap);
    searchClauseVar(conSt, conclT, clvarLt, varmap);
    //we have all the subconclusions pushed on to the stack, now we check the rules
    std::cout << "Checking Rules\n";
    disease = rulesList(conSt, varmap);
    if(disease == "not heart disease"){
      std::cout << "This is not the heart disease\n";
    }
    //if disease isn't found find next conclusion list
    if(it != conclT.end()){
      it = std::find_if(it, conclT.end(), isHeartDisease);
      conSt.push(std::make_pair(it -> second, 4*((it -> second/10) - 1)));
      it++;
    }
  }
  std::cout << "------ Found the heart disease ------\n";
  return disease;
}
void searchClauseVar(std::stack<std::pair<int, int>> &conSt,
                   std::vector<std::pair<std::string, int>> conclT,
                   std::vector<std::string> clvarLt,
                   std::unordered_map<std::string, int> &varmap) {
  //go through clause variable list
  int i = conSt.top().second;
  int j = i + 4;
  std::cout << "Instantiating Variables For Rule: " << conSt.top().first << "\n";
  for(; i != j; i++){
    //if the variable is not a sub conclusion
    if(clvarLt[i] == ""){
      break;
    }
    if(varmap.find(clvarLt[i]) != varmap.end()){
      // instVar
      if(varmap[clvarLt[i]] == 0){
        std::cout << "Clause Variable " << clvarLt[i] << " has not been instantiated\n";
        instVar(varmap, clvarLt[i]);
        //after we instantiate a variable check the rules
      }else{
        std::cout << "Clause Variable has been instantiated\n";
      }
    }else{
      std::cout << "Clause Variable is Subconclusion\n";
      int subclnum = findSubconclusion(conclT, clvarLt[i]);
      conSt.push(std::make_pair(subclnum, 4*(subclnum/10 - 1)));
      searchClauseVar(conSt, conclT, clvarLt, varmap);
    }
  }
}

std::string rulesList(std::stack<std::pair<int, int>> &conSt, std::unordered_map<std::string, int> &varmap){
  //we're now search through the rules
  //what are the conditions?
  //All the variables for a particular disease have been instantiated
  //lets check the rules
  //once we check the rules we should pop the stack
  //if the rule relies on sub conclusions we need a way to store that a rule has been check
  //we will keep check rules till the stack is empty
  std::vector<std::string> vec(4);
  while(!conSt.empty()){
    int num = conSt.top().first;
    conSt.pop();
    std::cout << "Checking Rules for " << "Rule Number: " << num << "\n";
    switch(num){
    //very low canche of heart disese
      case 10:
        if(varmap["shortness of breath"] == 2){
          vec[0] = "yes";
        }else if(varmap["shortness of breath"] == 1){
          vec[0] = "no";
        }
        break;
    //heart disease
      case 20:
        if(vec[0] == "no" && varmap["abdomen pain"] == 2){
          return "abdominal aortic aneurysm";
        }
        break;
    //genetic heart disease
      case 30:
        if( vec[0] == "no" && varmap["abdomen pain"] == 1){
          vec[1] = "yes";
        }else{
          vec[1] = "no";
        }
        break;
      //pericarditis
      case 40:
        if(vec[1] == "yes" && varmap["pain when swallowing"] == 2){
          return "pericarditis";
        }
        break;
      //Marfan syndrom
      case 50:
        if(vec[1] == "yes" && varmap["pain when swallowing"] == 1){
          return "marfan syndrome";
        }
        break;
      //less chance of heart disease
      case 60:
        if(varmap["shortness of breath"] == 2 && varmap["chest pain"] == 1){
          vec[0] = "yes";
        }else{
          vec[0] = "no";
        }
        break;
      //congenital heart disease
      case 70:
        if(vec[0] == "yes" && varmap["cough"] == 1){
          return "congenital heart disease";
        }
        break;
      //some type of cardiomyopathy
      case 80:
        if(vec[0] == "yes" && varmap["cough"] == 2){
          vec[1] = "yes";
        }else{
          vec[1] = "no";
        }
        break;
      //dilated cardiomyopathy
      case 90:
        if(vec[1] == "yes" && varmap["nausea"] == 1){
          return "dilated cardiomyopathy";
        }
        break;
      //restrictive cardiomyopathy
      case 100:
        if(vec[1] == "yes" && varmap["nausea"] == 2){
          return "restrictive cardiomyopathy";
        }
        break;
      //chance of severe heart disease
      case 110:
        if(varmap["shortness of breath"] == 2 && varmap["chest pain"] == 2){
          vec[0] = "yes";
        }else{
          vec[0] = "no";
        }
        break;
      //coronary artery disease
      case 120:
        if(vec[0] == "yes" && varmap["weakness or dizziness"] == 1 && varmap["fainting"] == 1){
          return "coronary artery disease";
        }
        break;
      //advise going to the hospital
      case 130:
        if(vec[0] == "yes" && varmap["weakness or dizziness"] == 1 && varmap["fainting"] == 2){
          vec[1] = "yes";
        }else{
          vec[1] = "no";
        }
        break;
      //hypertrophic cardiomyopathy
      case 140:
        if(vec[1] == "yes" && varmap["swelling"] == 1){
          return "hypertrophic cardiomyopathy";
        }
        break;
      //artery disease
      case 150:
        if(vec[1] == "yes" && varmap["swelling"] == 2){
          vec[2] = "yes";
        }else{
          vec[2] = "no";
        }
        break;
      //heart valve disease
      case 160:
        if(vec[2] == "yes" && varmap["enlargement of the veins of the neck"] == 1){
          return "heart valve disease";
        }
        break;
      //pericardial effusion
      case 170:
        if(vec[2] == "yes" && varmap["enlargement of the veins of the neck"] == 2){
          return "pericardial effusion";
        }
        break;
      //chance of severe heart disease
      case 180:
        if(vec[0] == "yes" && varmap["weakness or dizziness"] == 2){
          vec[1] = "yes";
        }else{
          vec[1] = "no";
        }
        break;
      //chance of death
      case 190:
        if(vec[1] == "yes" && varmap["fatigue"] == 1 && varmap["no pulse"] == 2){
          vec[2] = "yes";
        }else{
          vec[2] = "no";
        }
        break;
      //sudden cardiac arrest
      case 200:
        if(vec[2] == "yes"){
          return "sudden cardiac arrest";
        }
        break;
      //no chance of death
      case 210:
        if(vec[1] == "yes" && varmap["no pulse"] == 1 && varmap["fatigue"] == 1){
          vec[2] = "yes";
        }else{
          vec[2] = "no";
        }
        break;
      //heart murmurs
      case 220:
        if(vec[2] == "yes" && varmap["cough"] == 2){
          return "heart murmurs";
        }
        break;
      //cardiomyopathy
      case 230:
        if(vec[2] == "yes" && varmap["cough"] == 1){
          return "cardiomyopathy";
        }
        break;
      //atrial fibrillation
      case 240:
        if(vec[1] == "yes" && varmap["fatigue"] == 2 && varmap["sweating"] == 1 && varmap["palpitations"] == 2){
          return "atrial fibrillation";
        }
        break;
      //cardiomegaly
      case 250:
        if(vec[1] == "yes" && varmap["fatigue"] == 2 && varmap["sweating"] == 1 && varmap["palpitations"] == 1){
          return "cardiomegaly";
        }
        break;
      //emergency
      case 260:
        if(vec[1] == "yes" && varmap["fatigue"] == 2 && varmap["sweating"] == 2){
          vec[2] = "yes";
        }else{
          vec[2] = "no";
        }
        break;
      //require immediate attention
      case 270:
        if(vec[2] == "yes" && varmap["fluttering in chest"] == 1){
          vec[3] = "yes";
        }else{
          vec[3] = "no";
        }
        break;
      //heart attack
      case 280:
        if(vec[3] == "yes"){
          return "heart attack";
        }
        break;
      //doesn't require immediate attention
      case 290:
        if(vec[2] == "yes" && varmap["fluttering in chest"] == 2){
          vec[3] = "yes";
        }else{
          vec[3] = "no";
        }
        break;
      case 300:
        if(vec[3] == "yes"){
          return "arrhythmia";
        }
        break;
      default:
        break;
    }
  }
  return "not heart disease";
}


/* Initiate Conclusion Table
  Uses a hash map to map the name of the the goal/disease to its appropriate rule
  I.E. Heart Murmurs is mapped to the value of 10 so on and so forth
*/
std::vector<std::pair<std::string, int>> initConclT(){
  std::vector<std::pair<std::string, int>> conmap(30);
  conmap[0] = std::make_pair("very low chance of heart disease", 10);
  //abdominal aortic aneurysm
  conmap[1] = std::make_pair("heart disease", 20);
  conmap[2] = std::make_pair("genetic disease", 30);
  //pericarditis
  conmap[3] = std::make_pair("heart disease", 40);
  //Marfan Syndrome
  conmap[4] = std::make_pair("heart disease", 50);
  conmap[5] = std::make_pair("less chance of severe heart disease", 60);
  //congenital heart disease
  conmap[6] = std::make_pair("heart disease", 70);
  conmap[7] = std::make_pair("some type of cardiomyopathy", 80);
  //dilated cardiomyopathy
  conmap[8] = std::make_pair("heart disease", 90);
  //restrictive cardiomyopathy
  conmap[9] = std::make_pair("heart disease", 100);
  conmap[10] = std::make_pair("chance of severe heart disease", 110);
  //coronary artery disease
  conmap[11] = std::make_pair("heart disease", 120);
  conmap[12] = std::make_pair("advise going to hospital", 130);
  //hypertrophic cardiomyopathy
  conmap[13] = std::make_pair("heart disease", 140);
  conmap[14] = std::make_pair("artery disease", 150);
  //heart valve disease
  conmap[15] = std::make_pair("heart disease", 160);
  //pericardial effusion
  conmap[16] = std::make_pair("heart disease", 170);
  conmap[17] = std::make_pair("serious chance of severe heart disease", 180);
  conmap[18] = std::make_pair("chance of death", 190);
  conmap[19] = std::make_pair("heart disease", 200);
  conmap[20] = std::make_pair("no chance of death", 210);
  //heart murmurs
  conmap[21] = std::make_pair("heart disease", 220);
  //cardiomyopathy
  conmap[22] = std::make_pair("heart disease", 230);
  //atrial fibrillation
  conmap[23] = std::make_pair("heart disease", 240);
  //cardiomegaly
  conmap[24] = std::make_pair("heart disease", 250);
  conmap[25] = std::make_pair("emergency", 260);
  conmap[26] = std::make_pair("require immediate attention", 270);
  //heart attack
  conmap[27] = std::make_pair("heart disease", 280);
  conmap[28] = std::make_pair("doesn't require immediate attention", 290);
  //arrhythmia
  conmap[29] = std::make_pair("heart disease", 300);
  return conmap;
}

/*
  Init Varirable Table
  Uses Unordered Map
  If value == 0 then Variable in uninstantiated
  If value == 1 then variable is false
  If value == 2 then variable is true
*/
std::unordered_map<std::string, int> initVarLt(){
  std::unordered_map<std::string, int> varmap;
  varmap["cough"] = 0;
  varmap["swelling"] = 0;
  varmap["weakness or dizziness"] = 0;
  varmap["shortness of breath"] = 0;
  varmap["chest pain"] = 0;
  varmap["fatigue"] = 0;
  varmap["palpitations"] = 0;
  varmap["nausea"] = 0;
  varmap["abdomen pain"] = 0;
  varmap["sweating"] = 0;
  varmap["enlargement of the veins of the neck"] = 0;
  varmap["no pulse"] = 0;
  varmap["fluttering in chest"] = 0;
  varmap["trouble breathing"] = 0;
  varmap["loss consciousness"] = 0;
  varmap["pain when swallowing"] = 0;
  varmap["fainting"] = 0;
  return varmap;
}

std::vector<std::string> initClVarLt(){
  std::vector<std::string> clVarLt(120);
  for(auto &i : clVarLt){
    i = "";
  }
  //Very low chance of heart disease 10
  clVarLt[0] = "shortness of breath";
  //abdominal aortic aneurysm 20
  clVarLt[4] = "very low chance of heart disease";
  clVarLt[5] = "abdomen pain";
  //genetic disease 30
  clVarLt[8] = "very low chance of heart disease";
  clVarLt[9] = "abdomen pain";
  //pericarditis 40
  clVarLt[12] = "genetic disease";
  clVarLt[13] = "pain when swallowing";
  //marfan syndrome 50
  clVarLt[16] = "genetic disease";
  clVarLt[17] = "pain when swallowing";
  //less chance of severe heart disease 60
  clVarLt[20] = "shortness of breath";
  clVarLt[21] = "chest pain";
  //congenital heart disease 70
  clVarLt[24] = "less chance of severe heart disease";
  clVarLt[25] = "cough";
  //some type of cardiomyopathy 80
  clVarLt[28] = "less chance of severe heart disease";
  clVarLt[29] = "cough";
  //dilated cardiomyopathy 90
  clVarLt[32] = "some type of cardiomyopathy";
  clVarLt[33] = "nausea";
  //restrictive cardiomyopathy 100
  clVarLt[36] = "some type of cardiomyopathy";
  clVarLt[37] = "nausea";
  //chance of severe heart disease 110
  clVarLt[40] = "shortness of breath";
  clVarLt[41] = "chest pain";
  //coronary artery disease 120
  clVarLt[44] = "chance of severe heart disease";
  clVarLt[45] = "weakness or dizziness";
  clVarLt[46] = "fainting";
  //advise going to hospital 130
  clVarLt[48] = "chance of severe heart disease";
  clVarLt[49] = "weakness or dizziness";
  clVarLt[50] = "fainting";
  //hypertrophic cardiomyopathy 140
  clVarLt[52] = "advise going to hospital";
  clVarLt[53] = "swelling";
  //artery disease 150
  clVarLt[56] = "advise going to hospital";
  clVarLt[57] = "swelling";
  //heart valve disease 160
  clVarLt[60] = "artery disease";
  clVarLt[61] = "enlargement of the veins of the neck";
  //pericardial effusion 170
  clVarLt[64] = "artery disease";
  clVarLt[65] = "enlargement of the veins of the neck";
  //serious chance of severe heart disease 180
  clVarLt[68] = "chance of severe heart disease";
  clVarLt[69] = "weakness or dizziness";
  //chance of death 190
  clVarLt[72] = "serious chance of severe heart disease";
  clVarLt[73] = "fatigue";
  clVarLt[74] = "no pulse";
  //sudden cardiac arrest 200
  clVarLt[76] = "chance of death";
  //no chance of death 210
  clVarLt[80] = "serious chance of severe heart disease";
  clVarLt[81] = "fatigue";
  clVarLt[82] = "no pulse";
  //heart murmurs 220
  clVarLt[84] = "no chance of death";
  clVarLt[85] = "cough";
  //cardio myopathy 230
  clVarLt[88] = "no chance of death";
  clVarLt[89] = "cough";
  //atrial fibrillation 240
  clVarLt[92] = "serious chance of severe heart disease";
  clVarLt[93] = "fatigue";
  clVarLt[94] = "sweating";
  clVarLt[95] = "palpitations";
  //cardiomegaly 250
  clVarLt[96] = "serious chance of severe heart disease";
  clVarLt[97] = "fatigue";
  clVarLt[98] = "sweating";
  clVarLt[99] = "palpitations";
  //emergency 260
  clVarLt[100] = "serious chance of severe heart disease";
  clVarLt[101] = "fatigue";
  clVarLt[102] = "sweating";
  // require immediate attention 270
  clVarLt[104] = "emergency";
  clVarLt[105] = "fluttering in chest";
  // heart attack 280
  clVarLt[108] = "require immediate attention";
  //doesn't require immediate attention 290
  clVarLt[112] = "emergency";
  clVarLt[113] = "fluttering in chest";
  //arrhythmia 300
  clVarLt[116] = "doesn't require immediate attention";
  return clVarLt;
}
//Debug Functions
//Helper Functions
bool isHeartDisease(std::pair<std::string, int> i){
  return i.first == "heart disease";
}

int findSubconclusion(std::vector<std::pair<std::string, int>> conclt, std::string str){
  for(auto i : conclt){
    if(i.first == str){
      return i.second;
    }
  }
  return -1;
}

void instVar(std::unordered_map<std::string, int> &varmap, std::string str){
  if(varmap[str] == 0){
    //instantiate variable
    std::string ans;
    if(str == "shortness of breath"){
      std::cout << "Do you have shortness of breath? ";
    }else if(str == "cough"){
      std::cout << "Are you coughing alot? ";
    }else if(str == "swelling"){
      std::cout << "Do you have swelling? ";
    }else if(str == "weakness or dizziness"){
      std::cout << "Have you been weak or dizzy? ";
    }else if(str == "chest pain"){
      std::cout << "Are you have chest pain? ";
    }else if(str == "fatigue"){
      std::cout << "Are you feeling fatigued? ";
    }else if(str == "palpitations"){
      std::cout << "Have you had palpitations? ";
    }else if(str == "nausea"){
      std::cout << "Have you been suffering from nausea? ";
    }else if(str == "abdomen pain"){
      std::cout << "Pain in your abdomen?";
    }else if(str == "sweating"){
      std::cout << "Have you had been sweating alot? ";
    }else if(str == "enlargement of the veins of the neck"){
      std::cout << "Have your neck veins become large recently? ";
    }else if(str == "no pulse"){
      std::cout << "Do you lack a pulse? ";
    }else if(str == "fluttering in chest"){
      std::cout << "Do you feel fluttering in your chest? ";
    }else if(str == "trouble breathing"){
      std::cout << "Have you had trouble breathing? ";
    }else if(str == "loss consciousness"){
      std::cout << "Have you lost consciousness? ";
    }else if(str == "pain when swallowing"){
      std::cout << "Pain when swallowing? ";
    }else if(str == "fainting"){
      std::cout << "Have you been fainting recently? ";
    }
    std::cout << std::endl;
    std::cout << "Enter y|n ";
    std::cin >> ans;
    if(ans == "y"){
      varmap[str] = 2;
    }else if(ans == "n"){
      varmap[str] = 1;
    }
  }
}
