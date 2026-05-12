#include <stdio.h>
#include <string.h>
void flushLine(FILE *f) {
    int c;
    while ((c = fgetc(f)) != '\n' && c != EOF);
}

void flushStdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidInput(int min, int max) {
    int val;
    while (1) {
        if (scanf("%d", &val) == 1 && val >= min && val <= max) {
            flushStdin();
            return val;
        }
        flushStdin();
        printf("Invalid input. Please enter a number between %d and %d: ", min, max);
    }
}

void show5050(int correct, int ans,
              char *o1, char *o2, char *o3, char *o4) {

    char *opts[5] = {NULL, o1, o2, o3, o4};

    printf("50-50 used! Remaining options:\n");


    int wrongShown = 0;
    for (int i = 1; i <= 4; i++) {
        if (i == correct) {
            printf("%d. %s", i, opts[i]);
        } else if (i != ans && !wrongShown) {
            printf("%d. %s", i, opts[i]);
            wrongShown = 1;
        }
    }
    printf("\n");
}
void endGame(FILE *fp, FILE *qf,
             char *playerName, char *categoryName,
             int score, int prizes[]) {

    printf("\n==========================================\n");
    printf("  Player  : %s\n", playerName);
    printf("  Category: %s\n", categoryName);
    printf("  Score   : %d/5\n", score);
    printf("  Prize   : %d Taka\n", score > 0 ? prizes[score] : 0);
    printf("==========================================\n");

    fprintf(fp, "Player: %s | Category: %s | Score: %d/5 | Prize: %d Taka\n",
            playerName, categoryName, score, score > 0 ? prizes[score] : 0);

    fclose(fp);
    fclose(qf);
}

int main() {
    printf("  Welcome to Hot Seat Challenge!\n");
    printf("  Organized by Prionta, Huzaifa and Ismile\n");
    printf("============================================\n\n");

    printf("Basic Rules:\n");
    printf("1. Answer 5 questions correctly to win.\n");
    printf("2. Wrong answer? You get ONE 50-50 lifeline.\n");
    printf("3. 50-50 removes 2 wrong options. Wrong again = Game Over.\n");
    printf("4. More correct answers = more prize money!\n\n");


    int prizes[6] = {0, 100, 200, 300, 400, 500};

    printf("Press 1 to play, 0 to quit: ");
    int press = getValidInput(0, 1);

    if (press != 1) {

        return 0;
    }


    char playerName[50];
    printf("\nEnter your name: ");
    if (fgets(playerName, sizeof(playerName), stdin) != NULL) {

        size_t len = strlen(playerName);
        if (len > 0 && playerName[len - 1] == '\n')
            playerName[len - 1] = '\0';
    }
    printf("Hello, %s! Let's play!\n\n", playerName);


    char filename[50];
    char categoryName[30];

    printf("Choose a Category:\n");
    printf("1. Science\n");
    printf("2. History\n");
    printf("3. Sports\n");
    printf("4. Bangladesh GK\n");
    printf("Enter choice (1-4): ");
    int catChoice = getValidInput(1, 4);

    switch (catChoice) {
        case 1:
            strcpy(filename, "science.txt");
            strcpy(categoryName, "Science");
            break;
        case 2:
            strcpy(filename, "history.txt");
            strcpy(categoryName, "History");
            break;
        case 3:
            strcpy(filename, "sports.txt");
            strcpy(categoryName, "Sports");
            break;
        case 4:
            strcpy(filename, "bangladesh.txt");
            strcpy(categoryName, "Bangladesh GK");
            break;
        default:
            strcpy(filename, "science.txt");
            strcpy(categoryName, "Science");
    }

    printf("\nCategory selected: %s\n", categoryName);
    printf("Good luck, %s!\n\n", playerName);


    FILE *fp = fopen("result.txt", "a");
    FILE *qf = fopen(filename, "r");

    if (fp == NULL || qf == NULL) {
        printf("File error! Make sure question files exist.\n");
        if (fp) fclose(fp);
        if (qf) fclose(qf);
        return 1;
    }

    int ans, score = 0;
    int used50 = 0;
    int life;
    char q[300], o1[200], o2[200], o3[200], o4[200];
    int correct;

    for (int i = 1; i <= 5; i++) {

        if (fgets(q, sizeof(q), qf) == NULL) break;
        fgets(o1, sizeof(o1), qf);
        fgets(o2, sizeof(o2), qf);
        fgets(o3, sizeof(o3), qf);
        fgets(o4, sizeof(o4), qf);
        fscanf(qf, "%d", &correct);
        flushLine(qf);
        printf("------------------------------------------\n");
        printf("Question %d of 5 | Prize if correct: %d Taka\n", i, prizes[i]);
        printf("------------------------------------------\n");
        printf("%s", q);
        printf("1. %s", o1);
        printf("2. %s", o2);
        printf("3. %s", o3);
        printf("4. %s", o4);

        // Show lifeline status
        if (used50 == 0)
            printf("[Lifeline available: 50-50]\n");
        else
            printf("[No lifelines remaining]\n");

        printf("Enter answer (1-4): ");
        ans = getValidInput(1, 4);

        if (ans == correct) {
            score++;
            printf("[CORRECT] You earned %d Taka!\n", prizes[i]);
            printf("Total score so far: %d/5\n\n", score);
        } else {

            if (used50 == 0) {
                printf("[WRONG] Do you want to use your 50-50 lifeline? (1=Yes, 0=No): ");
                life = getValidInput(0, 1);

                if (life == 1) {
                    used50 = 1;
                    show5050(correct, ans, o1, o2, o3, o4);

                    printf("Enter your new answer: ");
                    ans = getValidInput(1, 4);

                    if (ans == correct) {
                        score++;
                        printf("[CORRECT] You survived!\n");
                        printf("Total score so far: %d/5\n\n", score);
                    } else {
                        printf("[WRONG] Game Over!\n");
                        endGame(fp, qf, playerName, categoryName, score, prizes);
                        return 0;
                    }
                } else {
                    printf("[WRONG] Game Over!\n");
                    endGame(fp, qf, playerName, categoryName, score, prizes);
                    return 0;
                }
            } else {
                printf("[WRONG] No lifelines left. Game Over!\n");
                endGame(fp, qf, playerName, categoryName, score, prizes);
                return 0;
            }
        }
    }

    printf("\n==========================================\n");
    printf("  CONGRATULATIONS, %s!\n", playerName);
    printf("  You completed all 5 questions!\n");
    printf("  Category: %s\n", categoryName);
    printf("  Score   : %d/5\n", score);
    printf("  Prize   : %d Taka\n", prizes[score]);
    printf("==========================================\n");

    fprintf(fp, "Player: %s | Category: %s | Score: %d/5 | Prize: %d Taka\n",
            playerName, categoryName, score, prizes[score]);

    fclose(fp);
    fclose(qf);

    return 0;
}
