#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BOYUT 20
#define MAX_HAMLELER 500
#define MAX_KULLANICILAR 20
#define MAX_ISIM_UZUNLUK 50

struct Kullanici {
    int id;
    char isim[MAX_ISIM_UZUNLUK];
    int kullaniciPuani;
};

typedef struct {
    int Satir;
    int Sutun;
    int Yeni_Satir;
    int Yeni_Sutun;
} Pozisyon;

typedef struct {
    Pozisyon Pozisyonlar[MAX_HAMLELER];
    int Hamle_Sayisi;
} OyunDurumu;

struct hamleler {
    char Eslesme_Dizisi[MAX_BOYUT];
    int Oyun_Matrisi[MAX_BOYUT][MAX_BOYUT];
};

int hamleYap(int Satir, int Sutun, int Yeni_Satir, int Yeni_Sutun, int Oyun_Alani[MAX_BOYUT][MAX_BOYUT], int Matris_Boyutu);

void tahtayiCiz(int Oyun_Alani[MAX_BOYUT][MAX_BOYUT], int N);

void dosyadanOku(int Oyun_Alani[][MAX_BOYUT], char *Dosya_Adi);

int puanHesapla(int Matris_Boyutu, int Geri_Al, int Rasgele_Sayi, int Zaman);




int main(){
	int Game_Time,Match_Flag,Control_Flag = 1,Move_Count = 0 , Temp_Point , Undo_Flag = 0 , Match_Array_Temp[MAX_SIZE];
    int i, j, Operation_Value, Game_Zone[MAX_SIZE][MAX_SIZE], Matrix_Size, Row, Column, New_Row, New_Column;
    int userCount = 0;
	int in_game , N;

	clock_t Start_Time, End_Time;


    struct User USERS[MAX_USERS];
    struct moves MOVES[MAX_MOVES];

    char Name_Array[20];
    char File_Name[20];
    char In_Game_Action_Flag;


    printf("Printing all users...\n");
    for ( i = 0; i < userCount; i++){
        printf("User ID: %d, User name: %s\n", USERS[i].id, USERS[i].name);
    }
    //Resetting user points to avoid collapse in struct
    for ( i = 0; i < MAX_USERS; i++){
        USERS[i].userpoint = 0;
    }


    srand(time(NULL));


    do{

        int Temp_Id;//holds user name index
        int Temp_Point;//holds user temp point

        if ( userCount == 0){
            printf("Enter 4 to create user\n");
        }

        else{

            printf("Enter user id to play game (or create first by  pressing 4)\n");
            scanf("%d" , &Temp_Id);

            printf("Welcome %s\n" , USERS[Temp_Id-1].name);
        }


        printf("\nEnter operation vale.\n");
        printf("Enter 1 to play with Random ganerated map.\n");
        printf("Enter 2 to play with built-in maps.\n");
        printf("Enter 3 for checking score\n");
        printf("Enter 4 to create user\n");
        printf("Enter 0 for quit\n");
        scanf("%d", &Operation_Value);


        switch (Operation_Value)
        {
            case 1:

                printf("Enter matrix size\n");
                scanf("%d", &Matrix_Size);

                for(i=0;i<Matrix_Size;i++){
                    Match_Array_Temp[i]=0;
                }

                for(i = 0  ; i < Matrix_Size ; i++){
                    for ( j = 0 ; j < Matrix_Size  ; j++){
                        //resetting map to 0 first to avoid collision
                        Game_Zone[i][j] = 0;
                    }
                }

                for(i = 1  ; i < Matrix_Size+1 ; i++){
                    for ( j = 0 ; j < 2  ; j++){
                        //generating random coordinates to put values in map
                        int x,y;
                        x = rand()%Matrix_Size;
                        y = rand()%Matrix_Size;
                        //this block puts start and end point to map and it checks to avoid overwriting another number
                        if(Game_Zone[x][y] == 0){
                            Game_Zone[x][y] = i;
                        }
                        else{
                            j = j-1;
                        }
                    }
                }

                printf("Printing matrix!!!!!\n");
                in_game = 1;
                Start_Time = clock();//takes beggining time
                while (in_game)
                {


                    printf("\n-------------------------------------\n");
                    drawBoard(Game_Zone, Matrix_Size);
                    printf("Enter 'm' to make a move, 'u' to Undo a move, or 'q' to quit: ");
                    scanf(" %c", &In_Game_Action_Flag);

                    if (In_Game_Action_Flag == 'm'){
                        if (Move_Count >= MAX_MOVES){
                            printf("You have reached the maximum number of moves.\n");
                            continue;
                        }

                        for (i = 0; i < Matrix_Size; i++){
                            for (j = 0; j < Matrix_Size; j++)
                            {
                                MOVES[Move_Count].Game_Matrix[i][j]=Game_Zone[i][j];
                            }
                        }

                        for(i=0;i<Matrix_Size;i++){
                            MOVES[Move_Count].Match_Array[i]=Match_Array_Temp[i];
                        }

                        Move_Count = Move_Count +1;

                        printf("\nEnter the Row and Column to make a move (e.g., 0 1): ");
                        scanf("%d %d", &Row, &Column);

                        printf("\nEnter the new Row and Column  (e.g., 0 1): ");
                        scanf("%d %d", &New_Row, &New_Column);

                        Match_Flag=makeMove(Row, Column,New_Row, New_Column, Game_Zone, N);

                        if(Match_Flag!=0){

                            printf("Number %d's Match_Flag complated, you are going good :)", Match_Flag );
                            Match_Array_Temp[Match_Flag-1]=1;

                        }
                    }
                    else if (In_Game_Action_Flag == 'u'){

                        if(Move_Count<=0){
                            printf("\nYou cant go back!!!!!!!");
                        }
                        else{
                            Undo_Flag += 1;
                            Move_Count = Move_Count -1;

                        for (i = 0; i < Matrix_Size; i++){
                            for (j = 0; j < Matrix_Size; j++){

                                Game_Zone[i][j]=MOVES[Move_Count].Game_Matrix[i][j];

                            }

                        }
                        }

                    }
                    else if (In_Game_Action_Flag == 'q'){
                        End_Time = clock();//takes game time
                        Game_Time=(int)(End_Time - Start_Time) / CLOCKS_PER_SEC;

                        Temp_Point = calculatepoint(Matrix_Size , Undo_Flag , 2, Game_Time);//calculates user point per round

                        printf("\ngame over!!\nYour game point: ");
                        printf("%d\n" , Temp_Point);
                        printf("Your Time: %d seconds!!\n", Game_Time);

                        USERS[Temp_Id-1].userpoint += Temp_Point;//ads temp point to total point
                        printf("\nYour total point is: %d\n" , USERS[Temp_Id-1].userpoint);

                        Move_Count=0;

                        break;
                    }
                    else{
                        printf("Invalid action. Please try again.\n");
                    }

                    for (i = 0 ; i < Matrix_Size ; i++){
                        for(j = 0 ; j < Matrix_Size ; j++){
                            if(Game_Zone[i][j] == 0){
                                Control_Flag = 0;
                                //checks map to determine if user won or not
                            }
                        }
                    }

                    for(i=0;i<Matrix_Size;i++){
                        if(Match_Array_Temp[i]==0){
                            Control_Flag = 0;
                        }
                    }

                    if (Control_Flag == 1){

                        End_Time = clock();
                        drawBoard(Game_Zone, N);
                        printf("You won!!!!!\n");

                        Game_Time=(int)(End_Time - Start_Time) / CLOCKS_PER_SEC;
                        Temp_Point = calculatepoint(Matrix_Size , Undo_Flag , 2, Game_Time);

                        printf("Your Time: %d", Game_Time);
                        printf("Your game point:  %d" , Temp_Point );

                        USERS[Temp_Id-1].userpoint += Temp_Point;

                        printf("\nYour total point: %d\n" , USERS[Temp_Id-1].userpoint);

                        break;
                    }
                    Control_Flag = 1;
                }

                break;
            case 2:
                printf("Enter matrix size: ");
                scanf("%d", &N);

                printf("Enter file name: ");
                scanf("%s", &File_Name);

                for(i=0;i<N;i++){
                    Match_Array_Temp[i]=0;
                }

                printf("Printing matrix!!!!!\n");

                for(i = 0  ; i < N ; i++)
                {
                    for ( j = 0 ; j < N  ; j++)
                    {
                        Game_Zone[i][j] = 0;
                    }
                }

                readFromFile(Game_Zone, File_Name);

                in_game = 1;

                while (in_game){

                    printf("\n-------------------------------------\n");

                    drawBoard(Game_Zone, N);//tooks built-in map and prints to command prompt

                    printf("\nEnter 'm' to make a move, 'u' to Undo a move, or 'q' to quit: ");
                    scanf(" %c", &In_Game_Action_Flag);

                    if (In_Game_Action_Flag == 'm'){
                        if (Move_Count >= MAX_MOVES){
                            printf("You have reached the maximum number of moves.\n");
                            continue;//I couldnt find another solution to pass this block
                        }

                        for (i = 0; i < N; i++){
                            for (j = 0; j < N; j++){

                                MOVES[Move_Count].Game_Matrix[i][j]=Game_Zone[i][j];
                            }
                        }

                        Move_Count=Move_Count+1;

                        printf("Enter the Row and Column to make a move (e.g., 0 1): ");
                        scanf("%d %d", &Row, &Column);

                        printf("\nEnter the new Row and Column  (e.g., 0 1): ");
                        scanf("%d %d", &New_Row, &New_Column);

                        Match_Flag=makeMove(Row, Column,New_Row, New_Column, Game_Zone, N);

                        if(Match_Flag!=0){
                            printf("Number %d's Match_Flag complated, you are going good :)", Match_Flag );
                            Match_Array_Temp[Match_Flag-1]=1;
                        }
                    }
                    else if (In_Game_Action_Flag == 'u'){

                        if(Move_Count<=0){
                            printf("You cant Undo!!!!!!!!!!!!!");
                        }
                        else{
                            Move_Count = Move_Count -1;

                        for (i = 0; i < N; i++){
                            for (j = 0; j < N; j++){

                                Game_Zone[i][j]=MOVES[Move_Count].Game_Matrix[i][j];

                            }

                        }

                        for(i=0;i<N;i++){
                            Match_Array_Temp[i]=MOVES[Move_Count].Match_Array[i];
                        }
                        }


                    }
                    else if (In_Game_Action_Flag == 'q'){

                        End_Time = clock();
                        Game_Time=(int)(End_Time - Start_Time) / CLOCKS_PER_SEC;

                        Temp_Point = calculatepoint(N , Undo_Flag , 1, Game_Time);

                        printf("Game over!!\nYour game point: %d", Temp_Point);
                        printf("Your Time: %d", Game_Time);

                        USERS[Temp_Id-1].userpoint += Temp_Point;

                        printf("\nYour total point is %d\n" , USERS[Temp_Id -1 ].userpoint);

                        Move_Count=0;

                        break;
                    }
                    else{
                        printf("Invalid action. Please try again.\n");
                    }

                    for (i = 0 ; i < N ; i++){
                        for(j = 0 ; j < N ; j++){
                            if(Game_Zone[i][j] == 0){
                                Control_Flag = 0;
                            }
                        }
                    }
                    for(i=0;i<N;i++){
                        if(Match_Array_Temp[i]==0){
                            Control_Flag = 0;
                        }
                    }
                    if (Control_Flag == 1){

                        End_Time = clock();
                        Game_Time=(int)(End_Time - Start_Time) / CLOCKS_PER_SEC;

                        drawBoard(Game_Zone, N);

                        printf("You won!!!!!\n");

                        Temp_Point = calculatepoint(N , Undo_Flag , 1, Game_Time);

                        printf("Your game point:  %d" , Temp_Point );
                        printf("Your Time: %d", Game_Time);

                        USERS[Temp_Id-1].userpoint += Temp_Point;

                        printf("\nYour total point: %d\n" , USERS[Temp_Id-1].userpoint);

                        break;
                    }

                    Control_Flag=1;
                }
                break;

            case 3:
                printf("User scores\n");
                break;
            case 4:

                while (1){
                    char name[MAX_NAME_LENGTH];

                    printf("\nEnter user name (press Q for quit)\n");
                    scanf("%s", name);

                    if (strcmp(name, "Q") == 0){
                        break;
                    }

                    if (userCount > 0){
                        USERS[userCount].id = USERS[userCount - 1].id + 1;
                    }
                    else{
                        USERS[userCount].id = 1;
                    }

                    strcpy(USERS[userCount].name, name);
                    userCount++;

                    printf("User ID: %d\n", USERS[userCount - 1].id);
                    printf("User  %s\n\n", USERS[userCount - 1].name);

                }

                printf("All user id's\n");

                for ( i = 0; i < userCount; i++){
                    printf("User ID: %d,User name: %s\n", USERS[i].id, USERS[i].name);
                }
                for ( i = 0; i < userCount; i++){
                    printf("User ID: %d\nUser name: %s\nUser point: %s\n", USERS[i].id, USERS[i].name , USERS[i].userpoint);
                }

                break;

            default:
                break;

        }

    }

    while (Operation_Value != 0);
    //this holds user to stay in game
    return 0;

}

//The rest is function buildings

int isValidMove(int Row, int Column,int New_Row, int New_Column, int Game_Zone[MAX_SIZE][MAX_SIZE])
{

    int i;
    if((Game_Zone[New_Row][New_Column]== Game_Zone[Row][Column] || Game_Zone[New_Row][New_Column]== 0) && Game_Zone[Row][Column]!= 0 ){

        if(New_Row-Row!=0 && New_Column==Column){

            if(New_Row-Row<0){

                for(i=New_Row+1; i<Row; i++){
                    //upside control
                    if(Game_Zone[i][Column]!=0)
                    {
                        return 0;
                    }
                }
            }

            else if(New_Row-Row>0){
                for(i=Row+1; i<New_Row; i++){
                    //downside control
                    if(Game_Zone[i][Column]!=0){
                        return 0;
                    }
                }
            }
        }
        else if (New_Column-Column!=0 && New_Row==Row){

            if(New_Column-Column<0){
                //left side control
                for(i=New_Column+1; i<Column; i++){

                    if(Game_Zone[New_Row][i]!=0){

                        return 0;
                    }

                }
            }
            else if(New_Column-Column>0){

                for(i=Column+1; i<New_Column; i++){
                    //right side control
                    if(Game_Zone[New_Row][i]!=0){

                        return 0;
                    }
                }
            }
        }
        else{

            return 0;
        }

        return 1;

    }
    else{
        return 0;
    }
}

int makeMove(int Row, int Column, int New_Row, int New_Column, int Game_Zone[MAX_SIZE][MAX_SIZE], int Matrix_Size){

    int i, Checker, j, Match_Flag=0;

    Position currentPosition;

    currentPosition.Row = Row;
    currentPosition.Column = Column;

    Checker = isValidMove(Row, Column, New_Row, New_Column, Game_Zone);

    if (Checker == 1){
    	if(Game_Zone[Row][Column]==Game_Zone[New_Row][New_Column]){
			Match_Flag=Game_Zone[Row][Column];
		}

        if (New_Row - Row != 0){

            if (New_Row - Row < 0){

                for (i = New_Row; i <= Row; i++){

                    Game_Zone[i][Column] = Game_Zone[Row][Column];
                }
            }
            else if (New_Row - Row > 0){

                for (i = Row; i <= New_Row; i++){

                    Game_Zone[i][Column] = Game_Zone[Row][Column];
                }
            }
        }
        else if (New_Column - Column != 0){

            if (New_Column - Column < 0){

                for (i = New_Column; i <= Column; i++){

                    Game_Zone[New_Row][i] = Game_Zone[Row][Column];
                }
            }
            else if (New_Column - Column > 0){

                for (i = Column; i <= New_Column; i++){

                    Game_Zone[New_Row][i] = Game_Zone[Row][Column];
                }
            }
        }

	}
    else{

        printf("Invalid move");

    }

    return Match_Flag;

}

void readFromFile(int Game_Zone[][MAX_SIZE], char *File_Name){
	int i,j, Temp;

	FILE *Data = fopen(File_Name,"r");

	if(!Data){
        printf("File could not opened!!!!!!");
		return;
    }

    while(!feof(Data)){
        fscanf(Data,"%d %d %d\n",&i,&j,&Temp);
		Game_Zone[i][j]=Temp;
    }
  	fclose(Data);

}

void drawBoard(int Game_Zone[][MAX_SIZE], int N){//when I use brackets it fails.I dont know why
    int i,j,k;

    for(i=0;i<N;i++){

        printf("\n");

        for (k=0;k<N;k++)

            printf("-------");
            printf("\n");

        for(j=0;j<N;j++)
            if (Game_Zone[i][j]!=0)
                printf("  %d   |",Game_Zone[i][j]);
            else
                 printf("      |",Game_Zone[i][j]);
    }
    printf("\n");
    for (k=0;k<N;k++)

            printf("-------");

    printf("\n\n");
}


int calculatepoint(int Matrix_Size , int Undo , int Random, int Time){//this function calculates user point per game
    int Total_Point = 0;
    int Target_Time;

    Target_Time=60*Matrix_Size;

    Total_Point=Target_Time-(5*Time);
    Total_Point += pow(Matrix_Size,2);
    Total_Point = Total_Point - 5*Undo;
    Total_Point *= Random;//if user chooses random map it multiplies with 2 else 1

    return Total_Point;
}
