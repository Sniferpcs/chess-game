#include <iostream>
#include <string>
#include <cmath>
using namespace std;
char board[8][8] = 
{
    {'r','n','b','q','k','b','n','r'},
    {'p','p','p','p','p','p','p','p'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'P','P','P','P','P','P','P','P'},
    {'R','N','B','Q','K','B','N','R'}
};
void printBoard() 
{
    cout << "  a b c d e f g h" << endl;
    cout << "  ----------------" << endl;
    for (int i = 0; i < 8; i++) 
    {
        cout << 8 - i << "|";
        for (int j = 0; j < 8; j++) 
        {
            cout << board[i][j] << " ";
        }
        cout << "|" << 8 - i << endl;
    }
    cout << "  ----------------" << endl;
    cout << "  a b c d e f g h" << endl << endl;
}
bool pathclear(int srow, int scol, int erow, int ecol) 
{
    int rowstep = 0;
    int colstep = 0;
    if (erow > srow) rowstep = 1;
    else if (erow < srow) rowstep = -1;
    if (ecol > scol) colstep = 1;
    else if (ecol < scol) colstep = -1;
    int currentrow = srow + rowstep;
    int currentcol = scol + colstep;
    while (currentrow != erow || currentcol != ecol) 
    {
        if (board[currentrow][currentcol] != '.') 
        {
            return false; 
        }
        currentrow += rowstep;
        currentcol += colstep;
    }
    return true;
}
bool validmove(char piece, int srow, int scol, int erow, int ecol, bool whiteturn) 
{
    if (srow == erow && scol == ecol) return false;
    char target = board[erow][ecol];
    if (target != '.') 
    {
        bool twhite = (target >= 'A' && target <= 'Z');
        if (whiteturn == twhite) return false; 
    }
    int rowdiff = erow - srow;
    int coldiff = ecol - scol;
    char lowerpiece = tolower(piece);
    if (lowerpiece == 'p') 
    { 
        if (whiteturn) 
        {
            if (coldiff == 0 && rowdiff == -1 && target == '.') return true;
            if (coldiff == 0 && srow == 6 && rowdiff == -2 && board[5][scol] == '.' && target == '.') return true;
            if (rowdiff == -1 && abs(coldiff) == 1 && target != '.') return true;
        } else 
        {
            if (coldiff == 0 && rowdiff == 1 && target == '.') return true;
            if (coldiff == 0 && srow == 1 && rowdiff == 2 && board[2][scol] == '.' && target == '.') return true;
            if (rowdiff == 1 && abs(coldiff) == 1 && target != '.') return true;
        }
        return false;
    }
    if (lowerpiece == 'n') 
    { 
        return (abs(rowdiff) == 2 && abs(coldiff) == 1) || (abs(rowdiff) == 1 && abs(coldiff) == 2);
    }
    if (lowerpiece == 'k') 
    { 
        return (abs(rowdiff) <= 1 && abs(coldiff) <= 1);
    }
    if (lowerpiece == 'r') 
    {
        if (rowdiff == 0 || coldiff == 0) 
        {
            return pathclear(srow, scol, erow, ecol);
        }
        return false;
    }
    if (lowerpiece == 'b') 
    {
        if (abs(rowdiff) == abs(coldiff)) 
        {
            return pathclear(srow, scol, erow, ecol);
        }
        return false;
    }
    if (lowerpiece == 'q') 
    {
        if (rowdiff == 0 || coldiff == 0 || abs(rowdiff) == abs(coldiff)) 
        {
            return pathclear(srow, scol, erow, ecol);
        }
        return false;
    }
    return false; 
}
bool check(bool whiteking) 
{
    int krow = -1, kcol = -1;
    char targetking = whiteking ? 'K' : 'k';
    for (int r = 0; r < 8; r++) 
    {
        for (int c = 0; c < 8; c++) 
        {
            if (board[r][c] == targetking) 
            {
                krow = r;
                kcol = c;
                break;
            }
        }
        if (krow != -1) break;
    }
    for (int r = 0; r < 8; r++) 
    {
        for (int c = 0; c < 8; c++) 
        {
            char piece = board[r][c];
            if (piece != '.' && ((piece >= 'A' && piece <= 'Z') != whiteking)) 
            {
                if (validmove(piece, r, c, krow, kcol, !whiteking)) 
                {
                    return true;
                }
            }
        }
    }
    return false;
}
bool testmoverescues(int srow, int scol, int erow, int ecol, bool whiteturn) 
{
    char originalsource = board[srow][scol];
    char originaltarget = board[erow][ecol];
    board[erow][ecol] = originalsource;
    board[srow][scol] = '.';
    bool safe = !check(whiteturn);
    board[srow][scol] = originalsource;
    board[erow][ecol] = originaltarget;
    return safe;
}
bool legalmoves(bool whiteturn) 
{
    for (int sr = 0; sr < 8; sr++) 
    {
        for (int sc = 0; sc < 8; sc++) 
        {
            char piece = board[sr][sc];
            if (piece == '.' || ((piece >= 'A' && piece <= 'Z') != whiteturn)) continue;

            for (int er = 0; er < 8; er++) 
            {
                for (int ec = 0; ec < 8; ec++) 
                {
                    if (validmove(piece, sr, sc, er, ec, whiteturn)) 
                    {
                        if (testmoverescues(sr, sc, er, ec, whiteturn)) 
                        {
                            return true; 
                        }
                    }
                }
            }
        }
    }
    return false;
}
int main() 
{
    bool whiteturn = true;
    cout << "Enter moves in standard format (example, 'e2 e4' or 'g1 f3')." << endl;
    cout << "Type 'exit' to exit the game." << endl;
    while (true) 
    {
        printBoard();
        bool currentCheck = check(whiteturn);
        bool legalmovesexist = legalmoves(whiteturn);
        if (currentCheck) 
        {
            if (!legalmovesexist) 
            {
                cout << "CHECKMATE! " << (whiteturn ? "Black" : "White") << " wins the game!" << endl;
                break;
            }
            cout << "WARNING: Your King is in CHECK!" << endl;
        } 
        else if (!legalmovesexist) 
        {
            cout << "STALEMATE! The game ends in a draw." << endl;
            break;
        }
        if (whiteturn) 
        {
            cout << "White's turn (UPPERCASE): ";
        } else 
        {
            cout << "Black's turn (lowercase): ";
        }
        string startpos, endpos;
        cin >> startpos;
        if (startpos == "exit") break;
        cin >> endpos;
        if (startpos.length() < 2 || endpos.length() < 2) 
        {
            cout << "Invalid format! Use 'e2 e4'." << endl;
            continue;
        }
        int startcol = startpos[0] - 'a';
        int startrow = 8 - (startpos[1] - '0');
        int endcol = endpos[0] - 'a';
        int endrow = 8 - (endpos[1] - '0');
        if (startrow < 0 || startrow > 7 || startcol < 0 || startcol > 7 || endrow < 0 || endrow > 7 || endcol < 0 || endcol > 7) 
        {
            cout << "Invalid position! Try again." << endl;
            continue;
        }
        char piece = board[startrow][startcol];
        if (piece == '.') 
        {
            cout << "There is no piece at that starting position! Try again." << endl;
            continue;
        }
        if (whiteturn && (piece < 'A' || piece > 'Z')) 
        {
            cout << "It's White's turn! You must move an UPPERCASE piece." << endl;
            continue;
        }
        if (!whiteturn && (piece < 'a' || piece > 'z')) 
        {
            cout << "It's Black's turn! You must move a lowercase piece." << endl;
            continue;
        }
        if (!validmove(piece, startrow, startcol, endrow, endcol, whiteturn)) 
        {
            cout << "Illegal move for that piece! Try again." << endl;
            continue;
        }
        if (!testmoverescues(startrow, startcol, endrow, endcol, whiteturn)) 
        {
            cout << "Illegal move! You cannot leave or put your King in check. Try again." << endl;
            continue;
        }
        board[endrow][endcol] = piece;
        board[startrow][startcol] = '.';
        whiteturn = !whiteturn;
    }
    cout << "Thanks for playing!" << endl;
    return 0;
}