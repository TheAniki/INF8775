#include "./headers/Includes.h"

// This method is used to parse the arguments.
void argumentsParsing(int argc, const char* argv[], 
string& filename,unsigned int& nbCirconscription, bool& print){

    for (int i = 1; i < argc-1; i++)
    {
        if (!strcmp(argv[i], "-e"))
            filename = argv[i + 1];
        if (!strcmp(argv[i+1], "-p"))
            print = true;
		if (!(strcmp(argv[i],"-c"))){
            stringstream strValue;
            strValue << argv[i+1];
            strValue>>nbCirconscription;
        }	
    }
}

int main(int argc, const char*argv[]){

    // Arguments parsing.
	string fileName;
    bool print = false;
    unsigned int nbCirconscription;
    argumentsParsing(argc, argv, fileName, nbCirconscription, print);

    // Initial print of the arguments
    cout<<nbCirconscription<<endl;
    cout<<fileName<<endl;
    cout<<print<<endl;

    
    return 0;
}