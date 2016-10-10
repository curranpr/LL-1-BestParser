/* Curran Prasad
 * Brittany Reedman
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define FALSE 0
#define TRUE 1

using namespace std;

int  getTokenNumbersPerLine(string current);
void displayProductions(vector < vector <string > >, int, int);
void generateFirstSet(vector < vector <string > >, int, int);
void generateFollowSet(vector < vector <string > >, int, int);
void generateEpsilonSet(vector < vector <string > >, int, int);
void instantiateParseTable(int, int);
void displayParseTable();

vector<string> splitTokens(string &, char);

vector <vector <string> > productionRules;
vector <string> firstSet;
vector <string> followSet;
vector <string> inputs;
vector <string> epsilonSet;
vector <vector <string> > parseTable;

int rowDim;
int colDim;
int numInputs;


int main ()
{
    int i = 0;
    int j = 0;
    rowDim = 0;
    colDim = 0;
    numInputs = 12;


    inputs.resize(numInputs);

    inputs[0] = "id";
    inputs[1] = "number";
    inputs[2] = "read";
    inputs[3] = "write";
    inputs[4] = "becomes";
    inputs[5] = "lparen";
    inputs[6] = "rparen";
    inputs[7] = "+";
    inputs[8] = "-";
    inputs[9] = "*";
    inputs[10] = "/";
    inputs[11] = "$$";

    string line; //stores each line in the file
    
    ifstream inputFile;

    
    inputFile.open("Grammar.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error opening grammar file\n";
        exit(1);
    }
    if (inputFile.is_open()) {
        int row = 0;
        int col = 0;
        int temp;
        //calculates necessary row and column numbers
        while (getline (inputFile, line)) {
            row++;
            temp = getTokenNumbersPerLine(line);
            if(temp > col)
                col = temp;
            temp = 0;
        }
        //resets input file stream
        inputFile.clear();
        inputFile.seekg(0, ios::beg);
        
        //string stringProduction[row][col];
        rowDim = row;
        colDim = col;
        
        productionRules.resize(rowDim);
        
        for(i = 0; i<rowDim; i++){
            productionRules[i].resize(colDim);
        }
        
        
        row = 0;
        col = 0;
        //inserts all tokens into the vector
        while(getline(inputFile, line)) {
            //size_t foundIndex = 0;
            vector <string> currentProduction;
            
            // while ((foundIndex = line.find(" ")) != std::string::npos) {
            //     //stringProduction[row][col] = line.substr(0, foundIndex);
            //     productionRules[row][col] = line.substr(0, foundIndex);
            //     line.erase(0, foundIndex + 1);
            //     col++;
            
            // }
            //stringProduction[row][col] = line;
            //productionRules.at(row).at(col) = line;
            
            currentProduction = splitTokens(line, ' ');
            productionRules[row] = currentProduction;
            //cout << "Current col " << col + 1 << endl;
            col = 0;
            row++;
        }
        //closes input file stream
        inputFile.close();
    }
    else cout << "Unable to open file";
    
    //displayProductions(productionRules, rowDim, colDim);
    
    //cout << "Calling First for Prod " << rowDim << "Cols " << colDim << endl;
    
    generateFirstSet(productionRules, rowDim, colDim);
    generateFollowSet(productionRules, rowDim, colDim);
    generateEpsilonSet(productionRules, rowDim, colDim);
    instantiateParseTable(rowDim, colDim);
    displayParseTable();
    
    return 0;
    
    
}

vector<string> splitTokens(string &s, char delim)
{
    stringstream ss;
    string item;
    vector<string> tokens;
    
    ss.str(s);
    
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

int getTokenNumbersPerLine(string current) {
    //if the string is empty, return that there are 0 tokens
    if(current.compare("") == 0)
        return 0;
    int i = 0; //index of string
    int num = 1; //number tokens (if there is 0 spaces and the string isn't empty, there is 1 token)
    while(i < current.length() - 1) { //loops through the string
        //checks to see if there is a space at i (ensures we don't cout multiple spaces as multiple tokens)
        if(isspace(current.at(i)) && !isspace(current.at(i + 1)))
            num++;
        i++;
    }
    return num; //returns number of tokens in given string
}

void displayProductions(vector < vector < string > > rules, int r, int c){
    
    int m = 0;
    int n = 0;
    string temp = " ";
    
    for(m = 0; m<r; m++){
        
        for(n = 0; n<c; n++){
            
            if(rules[m][n].compare(" ") != 0)
                temp = temp + rules[m][n] + "\t";
        }
        
        //cout << "Current: " << temp << endl;
        temp = " ";
        
    }
    
}



void generateFirstSet(vector < vector < string > > prods, int rows, int cols)
{
    
    int l;
    int k = 0;
    int x = 0;
    int v = 0;
    string firstToken = " ";
    int repeatingUnique = FALSE;
    
    
    
    firstSet.resize(rows);
    
    //cout << "Elements in vector" << prods[0].size() << endl;
    
    // For each production in productions
    // (For each first token)
    
    
    for(l = 0; l<rows; l++)
    {
        
        firstToken = prods[l][2];



        firstSet[l] = "";
        
        // Base case:
        // Non-terminal (LHS) produces one terminal (RHS) equal to non-terminal
        // and,
        // the terminal is the only terminal
        
        if(prods[l][0].compare(firstToken) == 0 && (prods[l].size() == 3)) {
            firstSet[l] = firstSet[l] + firstToken;
            // cout << "-----------------" << endl;
            // cout << "Base case --> ";
            //cout << "1:First of " << prods[l][0] << " is " << firstToken << endl;
        }
        
        // Single terminal (1 RHS)
        else if((prods[l].size() == 3)){
            
            firstSet[l] = firstSet[l] + firstToken;
            // cout << "-----------------" << endl;
            // cout << "Single terminal \t";
            //cout << "2:First of " << prods[l][0] << " is " << firstToken << endl;
        }
        
        
        // Single epsilon case (one blank space)
        // LHS ->
        
        else if(prods[l].size() == 2) {
            firstSet[l] = firstSet[l] + "epsilon";
            // cout << " ----------" << endl;
            // cout << "Single epsilon --> ";
            //cout << "3:First of " << prods[l][0] << " is epsilon" << endl;
            // cout << "First(" << prods[l][0] << ")" << prods[l][lastTerminalPos]  << endl;
        }
        
        
        
        //else if()
        
        
        
        
        else 
        {
            // All terminals produce a single epsilon
            // NT -> T1 T2 T3 T4
            // T1 -> epsilon
            // T2 -> epsilon
            // T3 -> epsilon
            int colSz;
            int isUnique = TRUE;
            
            colSz = prods[l].size() - 1;
            int allEpsilons = FALSE;

            for (x =2; x < colSz; x++)
            {
                // find match of that terminal in non-terminal and determine if it produces epsilon
                // prods [l] -> match all terminals of prods[l]
                string terminal;
                
                terminal= prods[l][x];

                //cout << "enter loop A (terminal) " << terminal << "(non-terminal:)" << prods[l+1][0] << endl; 
                
                //cout << "In Loop" << "[l:" << l << "]" << "[x:" << x << "]" << "Colsz[" << colSz << "]" << endl;
                for (k = l+1; k < rows; k++)
                {
                 
                    //cout << "enter loop B (terminal) " << terminal << "(non-terminal:)" << prods[k][0] << endl; 
                    if (terminal.compare(prods[k][0]) == 0) // terminal matches non-terminal
                    {
                        isUnique = FALSE;
                        if (prods[k].size() == 2)
                        {

                            allEpsilons = TRUE;
                            continue;
                        }
                        
                        else
                        {
                            allEpsilons = FALSE;

                            // Check repeating sequences
                            v = k + 1;

                            for(v = 0; v < rows; v++){
                            	if(prods[k][2] != prods[v][0])
                            		repeatingUnique = TRUE;
                            	else
                                {
                                    repeatingUnique = FALSE;
                                    break;
                                }
                            		
                            }

                            if (repeatingUnique == TRUE)
				            {
				            	firstSet[l] = firstSet[l] + prods[k][2];

				            }


                        }	 
                    }
                    else // terminal does not match non-terminal
                    {
                        if (x == 2 && isUnique != FALSE) // only check for first terminal
                        {
                            isUnique = TRUE;
                        }

                        




                    }
                }
                // if (isUnique == TRUE) // terminal is unique
                    //cout << "4:Terminal is unique: " << terminal << endl;

                if(isUnique == TRUE && x == 2 ){
                	//cout << " Unique terminal  " << terminal << endl;
                	firstSet[l] = firstSet[l] + terminal + " ";

            		}
            
	            if (allEpsilons == TRUE)
	            {
	                int lastTerminalPos;
	                lastTerminalPos = prods[l].size() - 1;
	                
	                firstSet[l] = firstSet[l] + prods[l][lastTerminalPos];
	            }

            //cout << "TBD --> Repeating terminals " <<  prods[l][0] << " with first token (" << firstToken  << ")" << endl;	
            
            
        	}
        
        
        
        // cout << ")" << endl;
    
    	}
    

    }
    //int p;

    // for (p = 0; p < firstSet.size(); ++p)
    //         cout << firstSet[p];



   
    
    for (k = 0; k < rows; k++){
    	cout << prods[k][0] << "\t {First: " << firstSet[k] <<  "}" <<endl;
    }

   


    
    
 }

 void generateFollowSet(vector < vector <string> > productions, int row, int col){


    int b = 0;
    int t = 0;
    int z = 0;
    int d = 0;
    int r = 0;



   
    followSet.resize(row);

    for (d = 0; d< row; d++){
        followSet[d] = followSet[d] + "";
    }


    // For all productions

    for (b  =0 ; b < row; b++)
    {

       
        // NT -> T1 T2 T3
        if(productions[b].size() == 4){

            //cout << "Size 4 " << endl;
            // T2
            string secondlastTerminal = productions[b][productions[b].size()-2];
            //cout << secondlastTerminal << endl;

            // T3
            string lastTerminal = productions[b][productions[b].size()-1];
            //cout << lastTerminal << endl;
            for (t = 1; t < row; t++){

                // Add string_First(T3) to Follow(T2)
                if(productions[t][0].compare(secondlastTerminal) == 0){

                    int rowToStore = t;
                    // string_First(T3)
                    for (z = 0; z <firstSet.size(); z++){

                        // Add First(T3) to Follow(T2)
                        if(productions[z][0].compare(lastTerminal) == 0){
                            
                            followSet[rowToStore] = followSet[rowToStore] + firstSet[z];
                        
                        }

                    }


                }


            }

        }
        // end if block


        // NT -> T1 T2
        if(productions[b].size() == 3)
        {
            
            string firstterminal = productions[b][productions[b].size() - 2];
            string secondterminal = productions[b][productions[b].size() - 1];

            int c = 0;

            // Add Follow(T1) to Follow(T2)


            for (c = 0; c<row; c++){

                string terminalToAdd;
                int n = 0;

                for(n = 0; n<productions[c].size(); n++){

                    if(productions[c][n].compare(firstterminal) == 0){

                        terminalToAdd = productions[c][n+1];
                        
                        // Find T2

                        for (r = 0; r < row; r++){

                            if(productions[r][0].compare(secondterminal) == 0){
                                followSet[r] = followSet[r] + terminalToAdd;
                            }
                            
                        }


                    }
                }


            }

        } 
        // end if block

        // NT -> T1 T2 T3 where string_EPS(T3) is TRUE

         if(productions[b].size() == 4)
         {
            string lastTerminalTwo = productions[b][productions[b].size() - 1];
            string secondToLastTerminal = productions[b][productions[b].size() - 2];
            string nonTerminal = productions[b][0];

            int f = 0;

            for (f = 0; f<row; f++){

                if(productions[f][0].compare(lastTerminalTwo) == 0){

                    if(productions[f].size() == 2){


                        int o = 0;
                        int p = 0;

                        for(o = 0; o<row; o ++){

                            for(p = 0; p<productions[o].size(); p++){

                                // Add Follow(NT) to Follow(2ndlastTerminal)
                                if(productions[o][p].compare(nonTerminal) == 0){

                                string addedTerminal = "";
                                    
                                    if(p > 1)
                                        addedTerminal = productions[o][p+1];

 
                                    int q = 0;

                                    for(q = 0; q < row; q++){
                                        if(productions[q][0].compare(secondToLastTerminal) == 0)
                                        {
                                            followSet[q] = followSet[q] + addedTerminal;
                                        }
                                    }
                                }
                            }
                        }

                    }

                }

            }
           

        }

        // end if block


        // Follow(T#) = First(T# + 1)

    
        





          




    }

    int x = 0;
    cout << "--------------------------------------------------" << endl;
    
    for (x = 0; x < row; x++){
        cout << productions[x][0] << "\t {Follow: " << followSet[x] <<  "}" <<endl;
    }







 }

 void generateEpsilonSet(vector < vector <string> > rules, int r, int c)
 {

    int i;
    int j;
    int l;


    epsilonSet.resize(r);

    for(l = 0; l<r; l++)
    {
        epsilonSet[l] = "";
    }



    for(i = 0; i<r; i++)
    {
        if(rules[i].size() == 2)
        {
            epsilonSet[i] = epsilonSet[i] + rules[i][0];
        }
    }

    for(j = 0; j<r; j++)
    {

        cout << "Current epsilon element: " << epsilonSet[j] << endl;


    }

 }	

void instantiateParseTable(int r, int c)
{
    int e = 0;
    int m = 0;
    int t = 0;
    int y = 0;
    int z = 0;
    int n = 0;
    int l  = 0;
    int endIndex = 0;
    int toUpdate = FALSE;

    parseTable.resize(r+1);

    for(t = 0; t<r+1; t++)
    {
        parseTable[t].resize(c+1);
    }

    for (y = 0; y<r+1; y++)
    {
        for(z = 0; z<c+1; z++)
        {
            parseTable[y][z] = "";
        }
    }

    // for(n = 0; n < 1; n++)
    // {
    //     for(l = 0; l<numInputs; l++)
    //     {
    //         parseTable[n][l] = parseTable[n][l] + inputs[l];
    //     }
    // }

    parseTable[0][1] = inputs[0];
    parseTable[0][2] = inputs[1];
    parseTable[0][3] = inputs[2];
    parseTable[0][4] = inputs[3];
    parseTable[0][5] = inputs[4];
    parseTable[0][6] = inputs[5];
    parseTable[0][7] = inputs[6];
    parseTable[0][8] = inputs[7];
    parseTable[0][9] = inputs[8];
    parseTable[0][10] = inputs[9];
    parseTable[0][11] = inputs[10];
    parseTable[0][12] = inputs[11];


    for(e = 0; e< r; e++)
    {
    
        for(m = 0; m<r; m++)
        {
            if(productionRules[e][0].compare(parseTable[m][0]) == 0)
            {
                toUpdate = FALSE;
                break;
            }
                
            else
            {
                toUpdate = TRUE;
            }
                

        }

        if(toUpdate == TRUE)
        {
            parseTable[e+1][0] = parseTable[e+1][0] + productionRules[e][0];
        }


    }

}

void displayParseTable()
{
    int parseRow = 0;
    int parseCol = 0;

    for(parseRow = 0; parseRow <rowDim+1; parseRow++)
    {
        for(parseCol = 0; parseCol <colDim; parseCol ++)
        {
            cout << parseTable[parseRow][parseCol] << "\t";
        }

        cout << endl;
    }
}


