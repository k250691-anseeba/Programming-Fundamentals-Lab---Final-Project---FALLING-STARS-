//FINALLL CODE
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>  


/*to keep game within a particular bounded area in console*/
#define SCREEN_HEIGHT 23
#define SCREEN_WIDTH  60

   /*Clear screen 
    necessary for making star fall
    Clearing screen -- printing new lines -- redraw the star
    so star visible to next line 
    If removed then we can only see a line of stars printed and not the falling animation */
    void clearScreen(){
        system("cls");
    }     
    
    // gotoxy function to position cursor
    void gotoxy(int x, int y){
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    /*Display rules and how to play instructions*/

    void displayIntro(){
        clearScreen();
        printf("\n\n\n");
        printf("\t\t\t\t 'FALLING STARS' \n\t\t\t\t   \033[33m* * * * * *\033[0m\n");
        printf("\n");
        printf("   GAME RULES:\n");
        printf("   - Catch falling stars with the container to score points.\n");
        printf("   - Use arrow keys (< or >) to move the container left or right.\n");
        printf("   - If you miss a star and you lose a life.\n");
        printf("   - Reach the required score for each level to advance.\n");
        printf("   - With each level the difficulty level rises (Star fall speed increases and container size shrinks).\n");
        printf("   - You have a total of 3 lives. Good luck!\n");
        printf("\n");
        printf("   Press any key to start.\n");
        getch();  
    }

    // Draw header (Lives, Score, Level)
    void drawHeader(int lives, int levelScore, int level){
        int i;
        printf("=================== \033[33mFALLING STAR GAME\033[0m ===================\n");

        printf("Lives: ");
        for (i = 0; i < 3; i++){
            if (i < lives){
                printf("\033[31m%c\033[0m ", 220);  
            }
            else{
                printf("  ");        // Empty space for lost lives (ensures space alignment)
            }
        }
        printf("\t\tScore: %d", levelScore);
        printf("\t\tLevel: %d\n", level);
        printf("=========================================================\n\n");
        
    }
    
    
    // Draw game area and its features (container, star)
    void drawScreen(int starRow, int starCol, int containerRow, int containerLeft, int containerRight){

    int r, c;
    for (r = 0; r < SCREEN_HEIGHT; r++){
        for (c = 0; c < SCREEN_WIDTH; c++){
            if (r == starRow && c == starCol){
                printf("\033[33m*\033[0m");
            }
            else if (r == containerRow && c >= containerLeft && c <= containerRight){
                if (c == containerLeft || c == containerRight){
                    printf("|"); 
                }
                else{
                    printf("_"); 
                }
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }

   /*Ensure smooth redrawing of screen 
   If this is removed star might not redraw again instantly 
   and so the flow of the game may lack smoothness*/
    fflush(stdout);
    }

    int main() {
        

        // Hide console cursor (Windows API)
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

        displayIntro();

        int lives = 3;
        int totalScore = 0;
        int level = 1;
        int requiredScore;
        int fallSpeed;
        int containerWidth;
        int levelScore;

    //goto function -- start_level:
    start_level:
    	
    levelScore = 0; 

    if (level == 1){ 
        requiredScore = 5; 
        fallSpeed = 100;     
        containerWidth = 14; 
    }
    else if(level == 2){ 
        requiredScore = 7; 
        fallSpeed = 80;     
        containerWidth = 12; 
    }
    else if(level == 3){ 
        requiredScore = 12; 
        fallSpeed = 50;     
        containerWidth = 10; 
    }


 
    int containerRow = SCREEN_HEIGHT - 2; // container prints a little above the bottom edge
    int containerLeft = (SCREEN_WIDTH / 2); 
    int containerRight = containerLeft + containerWidth; 
    
    clearScreen();
    drawHeader(lives, levelScore, level);
    while (levelScore < requiredScore && lives > 0){
    	
        int starRow = 4;
        
        //to ensure random numbers in a range: rand() % (max - min + 1) + min;
        //Range of star column = 1 to SCREEN_WIDTH - 2
        /*range taken from 1st column than 0th to avoid star falling from 
          absolute edge which container might not able to catch*/
        int starCol = (rand() % (SCREEN_WIDTH-2)) + 1;  

    //key is detected to move the container and the star falls to new line each iteration
        while (starRow < containerRow){
        	
            if (kbhit()){
                int key = getch();
                
                if (key == 224){
                    key = getch();
                    if (key == 75 && containerLeft > 1){ 
				        containerLeft -= 2; containerRight -= 2; 
					}
				    else if(key == 77 && containerRight < SCREEN_WIDTH - 2){
					    containerLeft += 2; 
						containerRight += 2; 
					}
                } 
                
			}
            
            // Position cursor to the start of the game screen (after header) and redraw only the game area
            gotoxy(0, 5);  // Assuming header ends at line 4 (0-based), so game starts at line 5
            
            drawScreen(starRow, starCol, containerRow, containerLeft, containerRight);
            Sleep(fallSpeed);
            starRow++;
        }


        // Check if star caught
        if(starCol >= containerLeft && starCol <= containerRight){
            levelScore++;
            totalScore++;
            // Update header after score change
            gotoxy(0, 0);
            drawHeader(lives, levelScore, level);
        } 
		else{
			lives--;
            // Update header after life loss
            gotoxy(0, 0);
            drawHeader(lives, levelScore, level);
        }
    }

    if (lives <= 0){
        clearScreen();
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t  GAME OVER!\n");
        printf("\t\t\tYour Score: %d\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", totalScore);
        return 0;
    }

    if (level < 3){
        clearScreen();
        printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\tLEVEL %d COMPLETE!\n\n\n\n\n\n\n\n\n\n\n\n\n", level);
        Sleep(1500);
        level++;
        goto start_level;
    }

    clearScreen();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t  YOU WIN THE GAME!\n");
    printf("\t\t\tFinal Score: %d\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", totalScore);
    
    return 0;
}
