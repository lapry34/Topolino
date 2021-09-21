#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include<bits/stdc++.h>

#define SIZE 1024

int N = NULL,
    ostc = NULL,
    mosse = NULL;    //inizializziamo i valori globali della grandezza matrice, numero ostacoli e mosse

using namespace std;

//matrice di grandezza fissa 1024, ne useremo solo una parte.
char campo[SIZE][SIZE] = {NULL};
char percorso[SIZE][SIZE] = {NULL};

//valori da escludere alla generazione dei fiori
vector<pair<int,int>> escl;

//topolino
pair<int,int> topolino = make_pair(0, 0);

//gestione matrice
void print_matrix(void);
void create_matrix(void);

void create_path(void);
void print_path(void);
bool solve_path(int x=0, int y=0);

//gestione di sistema windows
void showCursor(bool);
void cls(void);
void setColor(int);
//ricerca automatica percorso

int main(void){

    //impostiamo il colore a default
    setColor(7);

    //prendiamo la grandezza del campo e degli ostacoli
    while(true){
        cout << "Quanto vuoi fare grande il campo? \n--> ";
        cin >> N;

        //controllo di input
        if(cin.fail()){
            cin.clear();
            cin.ignore(256, '\n');
            cls();
            continue;
        } else break;
    }

    while(true){
        cout << "Quanti ostacoli vuoi generare? \n--> ";
        cin >> ostc;

        //controllo di input
        if(cin.fail()){
            cin.clear();
            cin.ignore(256, '\n');
            cls();
            continue;
        } else break;
    }

    create_matrix();

    //leviamo il cursore per motivi di grafica solo dopo la fase di input, e puliamo la chat precedente.
    showCursor(false);
    cls();
    //seme dei numeri casuali
    srand(time(NULL));

    //Generiamo casualmente i ostc fiori, escludendo la fine ed evitando di bloccare la strada
    escl.push_back(make_pair(N-1,N-1));
    escl.push_back(make_pair(0,0));

    //dobbiamo evitare di chiudere topolino o la fine dentro i fiori
    vector<pair<int,int>> diff1;
    vector<pair<int,int>> diff2;

    //variabili usate per i confronti e la creazione dei fiori/inserimento nelle esclusioni
    pair<int,int> p = make_pair(0, 0);
    bool b = true;

    for(size_t i = 0; i < ostc; i++){
        while(true){
            b = true;
            //numero da 0 a N-1 contenente la coordinata x o y dove mettere il fiore
            p.first = rand() % N;
            p.second = rand() % N;

            //cerca nelle esclusioni il p, e cambia b così che non venga inserito nella matrice.
            if(find(escl.begin(), escl.end(), p) != escl.end())
                b = false;

            if(b){

                //conta i fiori che vengono generati intorno a topolino, se sono 3 la partita è impossibile
                if(p == make_pair(1, 1) || p == make_pair(1, 0) || p == make_pair(0, 1)){
                    diff1.push_back(p);
                }

                //conta i fiori che vengono generati intorno alla fine, se sono 3 la partita è impossibile
                if(p == make_pair(N-1, N-2) || p == make_pair(N-2, N-2) || p == make_pair(N-2, N-1)){
                    diff2.push_back(p);
                }


                if(diff1.size() == 2){
                    //scorre tutto il container cercando le varie coordinate, se non trovata viene aggiutna alle escl
                    if(find(diff1.begin(), diff1.end(), make_pair(1, 1)) != diff1.end());
                    else escl.push_back(make_pair(1, 1));

                    if(find(diff1.begin(), diff1.end(), make_pair(1, 0)) != diff1.end());
                    else escl.push_back(make_pair(1, 0));

                    if(find(diff1.begin(), diff1.end(), make_pair(0, 1)) != diff1.end());
                    else escl.push_back(make_pair(0, 1));

                }

                if(diff2.size() == 2){
                    if(find(diff2.begin(), diff2.end(), make_pair(N-1, N-2)) != diff2.end());
                    else escl.push_back(make_pair(N-1, N-2));

                    if(find(diff2.begin(), diff2.end(), make_pair(N-2, N-1)) != diff2.end());
                    else escl.push_back(make_pair(N-2, N-1));

                    if(find(diff2.begin(), diff2.end(), make_pair(N-2, N-2)) != diff2.end());
                    else escl.push_back(make_pair(N-2, N-2));

                }


                campo[p.first][p.second] = '+';
                escl.push_back(p);
                break;
            }
        }
    }

    create_path();
    solve_path();
    char c = NULL;

    while(topolino != make_pair(N-1,N-1)){
        print_matrix();
        cout << "usare Q W E A S D Z C per muoversi." << endl;
        cout << "Hai fatto " << mosse << " mosse!" << endl;
        cout << endl << "Soluzione" <<  endl << endl;
        print_path();
        c = getch();
        mosse++;

        switch(c){

        case 'q':
            //Non deve sforare il bordo di n-1 muovendosi in diagonale in basso a sinistra
            if(topolino.first == 0 || topolino.second == 0 || campo[topolino.first-1][topolino.second-1] == '+')
                break;
            campo[topolino.first][topolino.second] = '*';
            topolino.first--;
            topolino.second--;
            campo[topolino.first][topolino.second] = 'O';

            break;

        case 'e':
            //Non deve sforare il bordo di n-1 muovendosi in diagonale in alto a destra
            if(topolino.first == 0 || topolino.second == N-1 || campo[topolino.first-1][topolino.second+1] == '+')
                break;
            campo[topolino.first][topolino.second] = '*';
            topolino.first--;
            topolino.second++;
            campo[topolino.first][topolino.second] = 'O';

            break;

        case 'a':   //evita lo scontro con la sinistra
            if(topolino.second == 0 || campo[topolino.first][topolino.second-1] == '+')
                break;
            campo[topolino.first][topolino.second] = '*';
            topolino.second--;
            campo[topolino.first][topolino.second] = 'O';

            break;

        case 'd':   //evita lo scontro con la destra
            if(topolino.second == N-1 || campo[topolino.first][topolino.second+1] == '+')
                break;
            campo[topolino.first][topolino.second] = '*';
            topolino.second++;
            campo[topolino.first][topolino.second] = 'O';

            break;

        case 'w': //evita lo scontro con l'alto
            if(topolino.first == 0 || campo[topolino.first-1][topolino.second] == '+')
                break;
            campo[topolino.first][topolino.second] = '*';
            topolino.first--;
            campo[topolino.first][topolino.second] = 'O';

            break;

        case 's': //evita lo scontro con il basso
            if(topolino.first == N-1 || campo[topolino.first+1][topolino.second] == '+')
                break;
            campo[topolino.first][topolino.second] = '*';
            topolino.first++;
            campo[topolino.first][topolino.second] = 'O';

            break;

        case 'z':
            //non deve sforare il bordo di N-1 e 0, muovendosi in diagonale in basso a sinistra
            if(topolino.first == N-1 || topolino.second == 0 || campo[topolino.first+1][topolino.second-1] == '+')
                break;
            campo[topolino.first][topolino.second] = '*';
            topolino.first++;
            topolino.second--;
            campo[topolino.first][topolino.second] = 'O';

            break;

        case 'c':
            //Non deve sforare il bordo di n-1 muovendosi in diagonale in basso a destra
            if(topolino.first == N-1 || topolino.second == N-1 || campo[topolino.first+1][topolino.second+1] == '+')
                break;
            campo[topolino.first][topolino.second] = '*';
            topolino.first++;
            topolino.second++;
            campo[topolino.first][topolino.second] = 'O';

            break;

        default:
            break;
        }

        cls();
    }


    cout << "Hai vinto!!!!! con un totale di " << mosse << " mosse" << endl;

    getch();

    return 0;
}


//stampa la matrice del campo, senza effettuare nessuna modifica
void print_matrix(void){

    for(size_t i = 0; i < N; i++){
        for(size_t j = 0; j < N; j++){
            cout << campo[i][j] << ' ';
        }
        cout << endl;
    }

    return;
}

//crea la matrice del campo, mettendo a 0,0 topolino e a N-1,N-1 la fine, i fiori vengono generati fuori dalla funzione.
void create_matrix(void){

    for(size_t i = 0; i < N; i++){
        for(size_t j = 0; j < N; j++){
            campo[i][j] = '*';
        }
    }

    campo[0][0] = 'O';
    campo[N-1][N-1] = 'F';

    return;
}

void create_path(void){

    for(size_t i = 0; i < N; i++){
        for(size_t j = 0; j < N; j++){
            percorso[i][j] = campo[i][j];
        }
    }

}

bool solve_path(int x, int y){

    //se arrivati
    if(x == N -1 && y == N -1){
        percorso[0][0] = 'O';
        percorso[x][y] = 'A';
        percorso[N-1][N-1] = 'F';
        return true;
    }

    if((x >= 0 && x < N) && (y >= 0 && y < N) && percorso[x][y] != '+'){
        percorso[x][y] = 'A';

        if(solve_path(x+1,y+1))
            return true;

        if(solve_path(x+1,y))
            return true;
        if(solve_path(x,y+1))
            return true;

        percorso[x][y] = '*';
        return false;
    }
    return false;
}

void print_path(void){

    for(size_t i = 0; i < N; i++){
        for(size_t j = 0; j < N; j++){

            if(percorso[i][j] == 'A' && campo[i][j] != 'F'){
                setColor(4);
                cout << '*' << ' ';
            }
            else {
                setColor(7);
                cout << percorso[i][j] << ' ';
            }
        }
        cout << endl;
    }

    return;
}

//cancella la console senza l'ausilio dei comandi di sistema
void cls(void){

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);

    return;
}

//elimina il cursore della riga di comando
void showCursor(bool visible) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(out, &cursorInfo);

    return;
}

//imposta il colore della console, usato per segnare il percorso
void setColor(int color){

    //7 default, 4 rosso scuro, 12 rosso, 8 grigio, 15 bianco.

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);

    return;
}
