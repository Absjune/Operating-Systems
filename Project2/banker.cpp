#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

int main() {
    const int n = 5;
    const int m = 3;
    char ch;

    //Resources each process has at start
    int allocate[n][m];
    //Max resources needed to complete its run
    int max[n][m]; 
    //remaining recourses 
    int available[m];

    std::ifstream table;
    table.open("bankTable.txt");
    if (!table.is_open()) return -1;
    
    //Fill
    if (!table.eof()) {
        int result[15];
        int keepTrack = 0;
        string str;
        getline(table, str, ';');
        for(int i = 0; i < str.length(); i++){
            if(str[i]!= '\n'){
                result[keepTrack] = int(char(str[i]-48));
                keepTrack++;
            }
        }
        keepTrack = 0;
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                allocate[i][j] = result[keepTrack];
                keepTrack++;
            }
        }
    } 

    if (!table.eof()) {
        int result[15];
        int keepTrack = 0;
        string str;
        getline(table, str, ';');
        for(int i = 0; i < str.length(); i++){
            if(str[i]!= '\n'){
                result[keepTrack] = int(char(str[i]-48));
                keepTrack++;
            }
        }
        keepTrack = 0;
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                max[i][j] = result[keepTrack];
                keepTrack++;
            }
        }
    } 

    if (!table.eof()) {
        string str;
        int result[3];
        int keepTrack = 0;
        table.get(ch);
        getline(table, str, ';');
        for(int i = 0; i < str.length(); i++){
            if(str[i]!= '\n'){
                result[keepTrack] = int(char(str[i]-48));
                keepTrack++;
            }
        }
        for(int i = 0; i < m; i++){
            available[i] = result[i];
        }
    }

    int f[n], ans[n], ind = 0;
    for (int k = 0; k < n; k++) {
        f[k] = 0;
    }

    int need[n][m];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        need[i][j] = max[i][j] - allocate[i][j];
    }

    int y = 0;
    for (int k = 0; k < 5; k++) {
        for (int i = 0; i < n; i++) {
            if (f[i] == 0) {
                int flag = 0;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > available[j]){
                        flag = 1;
                        break;
                    }
                }
 
                if (flag == 0) {
                    ans[ind++] = i;
                    for (y = 0; y < m; y++)
                    available[y] += allocate[i][y];
                    f[i] = 1;
                }
            }
        }
    }
   
    int flag = 1;
   
    // To check if sequence is safe or not
    for(int i = 0;i<n;i++){
        if(f[i]==0){
            flag = 0;
            cout << "The given sequence is not safe" << endl;
            break;
        }
        else{
            cout << "The sequence is safe." << endl;
        }
    }
 
    if(flag==1){
        cout << "The safe sequence is:" << endl;
        for (int i = 0; i < n - 1; i++)
        cout << " P" << ans[i] << " ->";
        cout << " P" << ans[n - 1] <<endl;
    }
 
    return (0);
}
