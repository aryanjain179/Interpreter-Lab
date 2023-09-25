#include <iostream>
#include <map>
#include <vector>
#include "Parse.h"
#include "ExpTree.h"
using namespace std;

void run();

int eval();

map <string, int> symbolTable;

int main() {
    set_input("test1.blip");
    run();
    cout << endl;
    set_input("test2.blip");
    run();
    cout << endl;
    set_input("test3.blip");
    run();
    cout << endl;
    set_input("test4.blip");
    run();
    cout << endl;

    //set_input("test_grader.blip");
    run();
}

void run(){
    int num;
    read_next_token();
    while(next_token_type != END){
        string token = next_token();
        if (token.substr(0,2) == "//"){
            skip_line();
            read_next_token();
        }
        else if (token == "text"){
            read_next_token();
            cout << next_token();
            read_next_token();
        }
        else if (token == "var"){
            read_next_token();
            string varName = next_token();
            read_next_token();
            if (next_token_type == NUMBER){
                num = token_number_value;
                read_next_token();
            }
            else {
                num = eval();
            }
            if (symbolTable.count(varName)){
                cout << "variable " << varName << " incorrectly re-initialized" << endl;
                symbolTable.at(varName) = num;
            }
            else {
                symbolTable.emplace(varName, num);
            }
        }
        else if (token == "set"){
            read_next_token();
            string setName = next_token();
            read_next_token();
            if (next_token_type == NUMBER){
                num = token_number_value;
                read_next_token();
            }
            else {
                num = eval();
            }
            if (!symbolTable.count(setName)){
                cout << "variable " << setName << " not declared" << endl;
                symbolTable.emplace(setName, num);
            }
            else {
                symbolTable.at(setName) = num;
            }
        }
        else if (token == "output"){
            read_next_token();
            string temp = next_token();
            if (next_token_type == NUMBER){
                cout << temp;
                read_next_token();
            }
            else {
                num = eval();
                cout << num;
            }
        }
    }
}

int eval(){
    vector<Node*> expr;
    string tempToken = next_token();
    while (next_token_type != END && tempToken.substr(0,2) != "//" && tempToken != "text" && tempToken != "var" && tempToken != "set" && tempToken != "output"){
        bool isOperand;
        int operand;
        string optr;
        if (next_token_type == SYMBOL){
            isOperand = false;
            operand = 0;
            optr = tempToken;
        }
        else {
            isOperand = true;
            if (next_token_type == NAME){
                operand = symbolTable.at(tempToken);
            }
            else {
                operand = token_number_value;
            }
            optr = "\0";
        }
        Node* temp = new Node(isOperand, operand, optr);
        expr.push_back(temp);
        read_next_token();
        tempToken = next_token();
    }
    ExpTree myTree(expr);
    return myTree.parse();
}
