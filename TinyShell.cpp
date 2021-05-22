#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stdlib.h>
#include <windows.h>
#include <psapi.h>
using namespace std;

void helpSelection(string secondCommand){
     if (secondCommand != "") {
        printf("Bad command or filename...\n\n");
        return;
     }
    printf( "   WELCOME TO MY SHELL\n" );
    printf( "myShell supports the following commands:\n\n" );
    printf( "dir   : List the content of the entering direction\n" );
    printf( "list  : List all background process\n" );
    printf( "cmd   : Execute the program given by cmd + program_name\n" );
    printf( "date  : Display the current date\n" );
    printf( "power : Execute Shutdown, Restart or Log Off your computer\n" );
    printf( "help  : Print this help\n" );
    printf( "exit  : Exit my shell\n\n" );

}

void powerSelection(string secondCommand){
    if (secondCommand == ""){
        printf( "Syntax: power + command\n" );
        printf( "3 command option below: \n" );
        printf( "   shutdown  : Shuts down the system and turns off the power\n" );
        printf( "   restart   : Shuts down the system and then restarts it\n" );
        printf( "   logoff    : Log off your computer\n\n" );
    }
    else {
        if (secondCommand == "logoff") ExitWindowsEx(EWX_LOGOFF, 0);
        else if (secondCommand == "shutdown") system("C:\\Windows\\System32\\shutdown -s -t 0");
        else if (secondCommand == "restart") system("C:\\Windows\\System32\\shutdown -r -t 0");
        else printf( "Bad command or filename...\n\n" );
    }
}

void dateSelection(string secondCommand){
     if (secondCommand != "") {
        printf("Bad command or filename...\n\n");
        return;
     }

     time_t now = time(0);

     // convert now to string form
     char* dt = ctime(&now);
     printf( "The current date and time is: %s\n", dt );
     // convert now to tm struct for UTC
     //tm *gmtm = gmtime(&now);
     //dt = asctime(gmtm);
     //cout << "The current date and time is: "<< dt << '\n\n';
}

void showAllFilename(string secondCommand){
    vector<string> names;
    string search_path = secondCommand + "/*.*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(search_path.c_str(), &fd);
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            names.push_back(fd.cFileName);
            /*
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(fd.cFileName);
            }
            */
        }while(FindNextFile(hFind, &fd));
        FindClose(hFind);
    }
    cout << "List all names of file or folder: \n";
    for (string it : names) cout << '\t' << it << '\n';
    printf("\n");
}

void PrintProcessNameAndID( DWORD processID )
{
    char szProcessName[MAX_PATH] = "noname Process";

    // Get a handle to the process.
    HANDLE handleProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, processID );
    if (handleProcess == NULL) return;

    // Get the process name.
    DWORD cbNeeded;
    if ( EnumProcessModules( handleProcess, NULL, 0, &cbNeeded) ) {
        GetModuleBaseNameA( handleProcess, NULL, szProcessName, sizeof(szProcessName)/sizeof(char) );
    }
    printf( "%-40s  Process ID: %u\n", szProcessName, processID );

    CloseHandle( handleProcess );
}

void listSelection()
{
    DWORD Processes[1024], cbNeeded, length;

    if ( !EnumProcesses( Processes, sizeof(Processes), &cbNeeded ) ) {
        printf("No process\n");
        return;
    }

    length = cbNeeded / sizeof(DWORD);
    printf("Number of current processes = %d\n", length);
    for (int  i = 0; i < length; i++ ){
        if( Processes[i] != 0 ) PrintProcessNameAndID( Processes[i] );
    }
    printf("\n");
}

void cmdSelection(string secondCommand){
    if (secondCommand == ""){
        printf( "Syntax: cmd + command\n" );
        printf( "3 command option below: \n" );
        printf( "   calc        : Launch Calculator\n" );
        printf( "   paint       : Launch Microsoft Paint\n" );
        printf( "   tasklist    : Show the task list\n\n" );
    }
    else{
        if (secondCommand == "calc") system("C:\\Windows\\System32\\calc.exe");
        else if (secondCommand == "paint") system("C:\\Windows\\System32\\mspaint.exe");
        else if (secondCommand == "tasklist") system("C:\\Windows\\System32\\tasklist.exe");
        printf("\n");
    }

}


int main(){
    ios_base::sync_with_stdio(false);
    SetConsoleTitle("TinyShell");

    while(true){
        string firstCommand = "", secondCommand = "";
        cout << "Tiny Shell> ";
        string str;
        getline(cin, str);
        if (str == "exit" ) break;

        for (int i = 0; i < str.length(); i++){
            if (str[i] == ' '){
                for (int j = i + 1; j < str.length(); j++) secondCommand += str[j];
                break;
            }
            else firstCommand += str[i];
        }
        //cout << firstCommand << '\n' << secondCommand << '\n';
        //cout << secondCommand.length() << '\n';

        if (firstCommand == "help") helpSelection(secondCommand);
        else if (firstCommand == "cmd") cmdSelection(secondCommand);
        else if (firstCommand == "date") dateSelection(secondCommand);
        else if (firstCommand == "power") powerSelection(secondCommand);
        else if (firstCommand == "dir") showAllFilename(secondCommand);
        else if (firstCommand == "list") listSelection();
        else printf( "Bad command or filename...\n\n" );
    }
}
