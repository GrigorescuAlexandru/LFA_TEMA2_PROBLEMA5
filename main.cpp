#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Production
{
    char name;
    string operations;
};

/**
 * @brief Read the input from the file and create the array of productions
 *
 * @param P is an array that has elements of type "struct Production"
 * @param numberOfProductions is an int that represents the number of the elements of array P
 */
void createProductions(vector<Production> &P, int &numberOfProductions)
{
    ifstream fin("..\\Regular Grammar.txt");
    int i;
    char unusedArrowCharacter;

    fin >> numberOfProductions;

    P.resize(numberOfProductions);

    for (i = 0; i < numberOfProductions; i++)
    {
        fin >> P[i].name;

        fin >> unusedArrowCharacter;
        fin >> unusedArrowCharacter;

        fin >> P[i].operations;
    }

    fin.close();
}

/**
 * @brief Verify if at least one of the productions with the name "operationName" can produce lambda ( character '0')
 *
 * @param P is an array that has elements of type "struct Production"
 * @param numberOfProductions is an int that represents the number of the elements of array P
 * @param operationName is a character that represents the name of the operation that will be verified
 * @return 1 if at least one operation with the name "operationName" can produce lambda, else return 0
 */
bool operationCanProduceLambda(vector<Production> P, int numberOfProductions, char operationName)
{
    int i;

    for (i = 0; i < numberOfProductions; i++)
        if (P[i].name == operationName)
        {
            if (P[i].operations == "0")
                return 1; // this return will happen only when operationName produces lambda
        }
    return 0; // this return will happen only when operationName doesn't produce lambda
}

/**
 * @brief Verify if the production with index "productionID" produces only terminals (characters between 'a' and 'z')
 *
 * @param P is an array that has elements of type "struct Production"
 * @param numberOfProductions is an int that represents the number of the elements of array P
 * @param productionID is an int that represents the index of the production from the array that will be verified
 * @return 1 if the production with index "productionID" produces only terminals, else return 0
 */
bool thisProductionProducesOnlyTerminals(vector<Production> P, int numberOfProductions, int productionID)
{
    int j;

    for (j = 0; j < P[productionID].operations.size(); j++)
        if (('A' <= P[productionID].operations[j]) && (P[productionID].operations[j] <= 'Z'))
            return 0; // this return will happen only when the production P[productionID] produces at least one non-terminal (characters between 'A' and 'Z')

    return 1; // this return will happen only when P[productionID] produces only terminals (characters between 'a' and 'z')
}

/**
 * @brief Verify if the production with index "productionID" produces lambda
 *
 * @param P is an array that has elements of type "struct Production"
 * @param numberOfProductions is an int that represents the number of the elements of array P
 * @param productionID is an int that represents the index of the production from the array that will be verified
 * @return 1 if the production with index "productionID" produces lambda, else return 0
 */
bool thisProductionProducesLambda(vector<Production> P, int numberOfProductions, int productionID)
{
    if (P[productionID].operations == "0")
        return 1;
    return 0;
}

/**
 * @brief Implements the generation of the word using the given regular grammar
 *        Will execute recursively all the operations produced by "operationName"
 *
 * @param P is an array that has elements of type "struct Production"
 * @param numberOfProductions is an int that represents the number of the elements of array P
 * @param operationName is the name of the production who will produce terminals and non-terminals
 * @param word is a string that stores what word has been generated until the current call of the function
 * @param wordLength is an int that represents the number of characters of string "word"
 * @param maxLength is an int that represents the number of characters of the generated words that must be printed
 */
void
executeThisOperation(vector<Production> P, int numberOfProductions, char operationName, string word, int wordLength,
                     int maxLength)
{
    int i, j;
    string wordGeneratedOnlyByThisProduction;

    if ((wordLength == maxLength) && operationCanProduceLambda(P, numberOfProductions, operationName))
    {
        cout << word << ' ';
        return;
    }

    if (wordLength < maxLength)
        for (i = 0; i < numberOfProductions; i++)
            if ((P[i].name == operationName) && (!thisProductionProducesLambda(P, numberOfProductions, i)))
            {
                wordGeneratedOnlyByThisProduction = "";

                for (j = 0; j < P[i].operations.size(); j++)
                    if (('a' <= P[i].operations[j]) && (P[i].operations[j] <= 'z'))
                        wordGeneratedOnlyByThisProduction = wordGeneratedOnlyByThisProduction + P[i].operations[j];
                    else
                        executeThisOperation(P, numberOfProductions, P[i].operations[j],
                                             word + wordGeneratedOnlyByThisProduction,
                                             (word + wordGeneratedOnlyByThisProduction).size(), maxLength);

                // this if is for productions that look like this A -> a  (productions that produce only terminals)
                if (thisProductionProducesOnlyTerminals(P, numberOfProductions, i) &&
                        (word + wordGeneratedOnlyByThisProduction).size() == maxLength)
                    cout << word + wordGeneratedOnlyByThisProduction << ' ';
            }
}

int main()
{
    vector<Production> P;
    int numberOfProductions = 0;
    int maxLength;

    createProductions(P, numberOfProductions);

    cout << "Maximum length = ";
    cin >> maxLength;

    executeThisOperation(P, numberOfProductions, 'S', "", 0, maxLength);

    cout << '\n';
    return 0;
}
