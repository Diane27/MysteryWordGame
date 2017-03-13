#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_REPONSE         5


using namespace std;

///prototype======================================================================================

    void info_games ( unsigned int whatInfo ) ;
    void windows_clear_screen( short x, short y ) ;
    bool recuperation_data_files ( const std::string& path, std::string& data  ) ;
    string pioche_mot ( string& bufferFile ) ;
    string melange_mot( string& mot ) ;
    void gestion_majuscule ( string& buffer ) ;

int main()
{
    ///variables==================================================================================

        string nbJoueur , bufferFile , motPiocher , motMelanger , reponse ;
        bool endWhile(false) , endProg(false) ;
        unsigned int repMax(0);

    do
    {
            endWhile = false ;
            endProg = false ;
            nbJoueur.clear() ;
            bufferFile.clear() ;
            motPiocher.clear() ;
            motMelanger.clear() ;
            reponse.clear() ;
            repMax = MAX_REPONSE ;
         ///asking how many players===========================================================

            info_games(0);
            while ( !endWhile )
            {
                ///info

                    info_games(1);

                ///user's answer

                    cin >> nbJoueur ;
                    cin.ignore();

                    if(nbJoueur == "1")
                    {
                        ///getting word from the file
                            if ( !recuperation_data_files("dico.txt",bufferFile) )
                            {

                                ///clear screen

                                    windows_clear_screen(0,5) ;

                                ///if error

                                    cout << "Warning !! Couldn't find the file.\n\nExit ...";
                                    Sleep(2000);
                                    return EXIT_FAILURE ;

                            }

                            motPiocher = pioche_mot( bufferFile );

                            endWhile = true ;

                    }
                    else if(nbJoueur == "2")
                    {

                        windows_clear_screen(0,5);
                        cout << "Enter the Mystery Word : " ;
                        cin >> motPiocher ;
                        cin.ignore() ;
                        endWhile = true ;

                    }
                    else
                    {
                        windows_clear_screen(0,5);
                    }

            }
        ///scramble letters=============================================================================

            windows_clear_screen(0,5);
            motMelanger = melange_mot(motPiocher) ;

        ///user's answer=====================================================================

            ///set to zero

                endWhile = false ;

            ///getting in the game

                while(!endWhile)
                {
                    ///info

                        info_games(2);
                        cout << "\t" << motMelanger << "\n\nAnswer : " ;
                        cin >> reponse ;
                        cin.ignore();

                    ///change uppercase letters to lowercase

                        gestion_majuscule(motPiocher);
                        gestion_majuscule(reponse) ;

                    ///check answer

                        if( reponse == motPiocher )
                        {
                            ///set to zero

                                windows_clear_screen(0,5);
                                reponse.clear();

                            ///victory

                                cout << "Congratulations!!! You found the Mystery Word !!\n" ;
                                Sleep(1500);
                                windows_clear_screen(0,5);

                            ///play again ?

                                cout << "Would you like to play again ?(y/n) : " ;
                                cin >> reponse ;

                                if ( reponse == "n" )
                                    endProg = true ;
                                else
                                    windows_clear_screen(0,0);

                                endWhile = true ;

                        }
                        else
                        {
                            ///set to zero

                                reponse.clear() ;
                                endWhile = false ;
                                windows_clear_screen(0,5);

                            ///info

                                info_games(4);
                                Sleep(1000);

                            ///clear scree

                                windows_clear_screen(0,5);


                                repMax--;
                                if(repMax <= 0 )
                                {
                                    ///info

                                        info_games(3);
                                        cout << "\t" << motPiocher ;
                                        reponse.clear();

                                    ///play again?

                                        cout << "\n\nWould you like to play again ?(y/n) : " ;
                                        cin >> reponse ;
                                        if ( reponse == "n" )
                                            endProg = true ;
                                        else
                                            windows_clear_screen(0,0);

                                        endWhile = true ;
                                }
                                else
                                {
                                    ///info
                                    cout << "You have : " << repMax << " turn(s) .\n\n" ;
                                }

                        }

                }

    }while(!endProg);
    return 0;
}

void info_games ( unsigned int whatInfo )
{
    switch ( whatInfo )
    {
        case 0 :
                cout << " --------------------------------------------------------------- \n" ;
                cout << "|                                                               |\n" ;
                cout << "|                         MYSTERY WORD                          |\n" ;
                cout << "|                                                               |\n" ;
                cout << " --------------------------------------------------------------- \n" ;
        break;

        case 1 :
                cout << "Number of players (2 players max) : " ;
        break;

        case 2 :
                cout << "Guess The Mystery Word : \n\n" ;
        break;

        case 3 :
                cout << "You Lost!!! The mystery word was : \n\n" ;
        break;

        case 4 :
                cout << "Sorry you got it wrong !!\n" ;
        break;
    }
}

void windows_clear_screen( short x, short y )
{
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO Info;
    DWORD NbOctetsEcrits;
    COORD Debut = {x, y};

    /* STD_OUTPUT_HANDLE sets the default output to the console */
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    /* read data  */
    GetConsoleScreenBufferInfo(hConsole, &Info);

    /* fill up with space */
    FillConsoleOutputCharacter(hConsole, ' ', Info.dwSize.X*Info.dwSize.Y, Debut, &NbOctetsEcrits);

    /* go back to the beginning */
    SetConsoleCursorPosition(hConsole, Debut);
}

bool recuperation_data_files ( const std::string& path, std::string& data  )
{

    ///opening file
    std::ifstream fichier( path.c_str() );
    std::string buf ;
    if(!fichier)
        return false ;
    else
    {
        ///getting file's data
        while(std::getline(fichier,buf))
            data += buf  + "\n" ;

        fichier.close() ;

        return true ;
    }

    return false ;

}
string pioche_mot ( string& bufferFile )
{
    ///variables=====================================================================================

        size_t compteMot(0) , nbMot(0) , nbAlleatoire(0) ;
        unsigned int i(0) , y(0) , a(0) , b(0) ;
        string motPiocher ;
        bool whileEnd(false) ;

    ///getting the number f word in the file====================================================
        while( compteMot != bufferFile.size() )
        {
                if( bufferFile[compteMot] == '\n' )
                    nbMot++ ;
                compteMot++ ;
        }
    ///getting random number======================================================================
        srand(time(0));
        nbMot-- ;//prend en copmte le zero
        ///1st random number
        a = 0+rand()%100 ;
        ///2nd random number
        b = 0+rand()%10 ;
        ///final random number
        nbAlleatoire = time(NULL);
        nbAlleatoire = (a*nbAlleatoire+b)%nbMot ;
    ///getting the word===========================================================================

        while ( !whileEnd )
        {

            if( y == nbAlleatoire )
                whileEnd = true ;

            if( bufferFile[i] == '\n' )
                    y++ ;
            i++ ;
        }
        i-- ;

        while ( bufferFile[i] != '\n' )
        {
            motPiocher += bufferFile[i];
            i++ ;
        }

        ///return
        return motPiocher ;

}
string melange_mot( string& mot )
{
    ///variables

        unsigned int const taille = mot.size() ;
        vector <int> sizeTab(taille,-1) ;
        unsigned int i(0) , buffer(0) , y(0) ;
        bool findChar(false) ;
        string motMelanger ;

        srand(time(NULL));

        while( i != taille )
        {
            buffer = ( rand()% ( ( taille - 1 ) + 1 ) ) + 0  ;

            for ( y = 0 ; y < taille ; y++ )
            {
                if( buffer == sizeTab[y] )
                {
                    findChar = true ;
                }
            }

            if(!findChar)
            {
                sizeTab[i] = buffer ;
                i++ ;
            }
            else
            {
                findChar = false ;
            }


        }
     ///scramble letters

        for ( i = 0 ; i < taille ; i++ )
        {
            y = sizeTab[i] ;

            motMelanger += mot[y] ;

        }

    ///return

        return motMelanger ;

}
void gestion_majuscule ( string& buffer )
{
    ///variables

        string majuscule ("ABCDEFGHIJKLMNOPQRSTUVWXYZ") , minuscule ("abcdefghijklmnopqrstuvwxyz") ;
        unsigned int i(0) , y(0) ;

        while( i != buffer.size() )
        {
            ///looking for uppercase occurrences

                while( y != majuscule.size() )
                {
                    ///if uppercase make lowercase

                        if( buffer[i] == majuscule[y] )
                            buffer[i] = minuscule[y] ;
                        y++;

                }

            ///set to zero

                y = 0 ;
                i++ ;

        }

}
