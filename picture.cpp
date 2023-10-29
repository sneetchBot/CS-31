#include "grid.h"
#include <iostream>

#include <string>

#include <cctype>
#include <map>
using namespace std;

//magic numbers
const int HORIZ = 0;
const int VERT = 1;
const int FG = 0;
const int BG = 1;

//status variables
int fgbgStatus = 0;
int currentCol = 1;
int currentRow = 1;
char currentChar = '*';

int position;
//Project 3: Phase 1
//Implementations of the plotVerticalLine function (treated as BG by default)
void plotVerticalLine(int r, int c, int distance, char ch) {
    currentChar = ch;
    currentCol = c;
    currentRow += distance;
    for (int i = 0; i < distance; i++) {
            setChar(r + i, c, ch);
    }
}

void plotHorizontalLine(int r, int c, int distance, char ch) {
    currentChar = ch;
    currentCol += distance;
    currentRow = r;
    for (int i = 0; i < distance; i++) {
            setChar(r, c + i, ch);
    }
}
//Override to account for fgbg when extending lines
void plotVerticalLine(int r, int c, int distance, char ch, int fgbg) {
    currentChar = ch;
    fgbgStatus = fgbg;
    if((currentRow + distance) <= MAXROWS){
        currentCol = c;
        currentRow += distance;
        for (int i = 0; i < distance; i++) {
            if (fgbg == FG || (fgbg == BG && getChar(r + i, c) == ' ')) {
                setChar(r + i, c, ch);
            }
        }
    }
}

void plotHorizontalLine(int r, int c, int distance, char ch, int fgbg) {
    currentChar = ch;
    fgbgStatus = fgbg;
    if((currentCol + distance) <= MAXCOLS){
        currentCol += distance;
        currentRow = r;
        for (int i = 0; i < distance; i++) {
            if (fgbg == FG || (fgbg == BG && getChar(r, c + i) == ' ')) {
                setChar(r, c + i, ch);
            }
        }
    }
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg) {
    // Check if dir and fgbg values are valid
    if ((dir != HORIZ && dir != VERT) || (fgbg != FG && fgbg != BG)) {
        return false;
    }
    // Check if plotChar is a printable character
    if (!isprint(plotChar)) {
        return false;
    }
    // Determine the start and end coordinates based on dir and distance
    if (dir == HORIZ) {
        if (distance > 0) {
            plotHorizontalLine(r, c, distance, plotChar, fgbg);
        }
    } else {
        if (distance > 0) {
            plotVerticalLine(r, c, distance, plotChar, fgbg);
        }
    }
    return true;
}


void plotRectangle(int r, int c, int height, int width, char ch){
    plotVerticalLine(r,c,height,ch);
    plotHorizontalLine(r,c,width,ch);
    plotVerticalLine(r,(c+width),height,ch);
    plotHorizontalLine((r+height-1), c, width, ch);
};
void performCommand(char command, string parameter){
    //Vertical line
    if(command == 'v'){
        int distance = stoi(parameter);
        bool plottable = plotLine(currentRow, currentCol, distance, VERT, currentChar, fgbgStatus);
        if(plottable){
            plotLine(currentRow, currentCol, distance, VERT, currentChar, fgbgStatus);
        }
    }
    //Horizontal line
    else if(command == 'h'){
        int distance = stoi(parameter);
        plotLine(currentRow, currentCol, distance, HORIZ, currentChar, fgbgStatus);
    }
    //Clear command
    else if(command == 'c'){
        fgbgStatus = 0;
        currentCol = 1;
        currentRow = 1;
        currentChar = '*';
        clearGrid();
    }
    //Foreground
    else if(command == 'f'){
        currentChar = parameter[0];
        fgbgStatus = 0;
    }
    //Background
    else if(command == 'b'){
        currentChar = parameter[0];
        fgbgStatus = 1;
    }
}
bool commandValidity(char command, string parameter){
    char validCommands[5] = {'v', 'h', 'f', 'b', 'c'};
    //Vertical line
    if(command == 'v' && (parameter.length() > 3)){
        return false;
    }
        //Horizontal line
    else if(command == 'h' && (parameter.length() > 3)){
        return false;
    }
        //Foreground
    else if(command == 'f' && parameter.length() > 1){
        return false;
    }
        //Background
    else if(command == 'b' && parameter.length() > 1){
        return false;
    }

    else if(find(begin(validCommands), end(validCommands), command)==end(validCommands)){
        return false;
    }
    else
        return true;
}
/*
 * This function tests whether the command string is valid
 * If not, set badPos
 * */

bool commandStrValidity(string commandString){
    for(int i = 0; i < commandString.length(); i++) {
        //convert command character to lowercase
        char command = tolower(commandString[i]);
        string parameter = "";
        i++;
        if (command != 'c') {
            parameter += commandString[i];
            while ((isdigit(commandString[i]) || (commandString[i] == '-')) && isdigit(commandString[i + 1])) {
                i++;
                parameter += commandString[i];
            }
            cout << "command : " << command << " parameter: " << parameter << endl;
        }
        if(commandValidity(command, parameter) == false){
            position = i-1;
            cout << "bad command : " << command << " at badPos: " << position << endl;
            return false;
        }
    }
    return true;
}
int performCommands(string commandString, char& plotChar, int& mode, int& badPos){
    badPos = position;
    if(commandString == "")
        return 0;
    /* If plotChar is a non-printable character (i.e., one for which isprint returns a value that tests as false) or
     * if mode does not have the value FG or BG, this function returns 2 without changing anything
     **/
    if((isprint(plotChar) == false) || ((mode != 1) && (mode != 0)))
        return 2;
    //Split the commandString into command-parameter pairs
    char command;
    string parameter;
    /*
     * The fist character must be a command
     * Add the second character to the parameter string regardless of whether it is an alphabet, number, or special character
     * If the value just added to parameter is a digit and the next character in commandString is a digit, append the new character to parameter
     * Once command and parameter are populated for a single instruction-- execute and repeat for the next
     * */
    if (commandStrValidity(commandString)== true){
        for(int i = 0; i < commandString.length(); i++){
            //convert command character to lowercase
            command = tolower(commandString[i]);
            //cout << "command: " << command << endl;
            i++;
            if(command != 'c'){
                parameter = commandString[i];
                while((isdigit(commandString[i]) || (commandString[i] == '-')) && isdigit(commandString[i+1])){
                    i++;
                    parameter += commandString[i];
                }
                //cout << "parameter: " << parameter << endl;
                performCommand(command, parameter);
            } else{
                performCommand(command, parameter);
                i++;
            }
        }
    } else
        return 1;
    return 0;
}

int main()
{for (;;)
    {
        cout << "Enter the number of grid rows and columns (max 30 each): ";
        int nRows;
        int nCols;
        cin >> nRows >> nCols;
        cin.ignore(10000, '\n');
        if (nRows >= 1  &&  nRows <= MAXROWS  &&  nCols >= 1  &&  nCols <= MAXCOLS)
        {
            setSize(nRows, nCols);
            break;
        }
        cout << "The numbers must be between 1 and 30." << endl;
    }
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        cout << "Enter a command string (empty line to quit): ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
            case 0:
                draw();
                break;
            case 1:
                cout << "Syntax error at position " << position << endl;
                break;
            case 2:
                if (!isprint(currentChar))
                    cout << "Current character is not printable" << endl;
                if (currentMode != FG  &&  currentMode != BG)
                    cout << "Current mode is " << currentMode << ", not FG or BG" << endl;
                break;
            case 3:
                cout << "Cannot perform command at position " << position << endl;
                break;
            default:
                // It should be impossible to get here.
                cout << "performCommands returned " << status << "!" << endl;
        }
    }
}