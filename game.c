#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 6
#define COLS 6
#define NUM_CANDY_TYPES 5
#define TARGET_SCORE 100
#define POINTS_PER_CANDY 10
#define MAX_STACK_SIZE 100
#define MAX_QUEUE_SIZE 100

// Definitions and Structures
typedef struct {
    int board[ROWS][COLS];
    int score;
} GameState;

typedef struct {
    GameState states[MAX_STACK_SIZE];
    int top;
} StateStack;

typedef struct {
    int candies[MAX_QUEUE_SIZE];
    int front, rear;
} CandyQueue;

typedef struct Node {
    int board[ROWS][COLS];
    int score;
    struct Node* next;
} Node;

Node* head = NULL;

// Stack Functions
void push(StateStack* stack, int board[ROWS][COLS], int score) {
    if (stack->top < MAX_STACK_SIZE - 1) {
        stack->top++;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                stack->states[stack->top].board[i][j] = board[i][j];
        stack->states[stack->top].score = score;
    }
}

GameState pop(StateStack* stack) {
    if (stack->top >= 0) {
        return stack->states[stack->top--];
    }
    return (GameState){{0}, 0};
}

// Queue Functions
void enqueue(CandyQueue* queue, int candy) {
    if ((queue->rear + 1) % MAX_QUEUE_SIZE != queue->front) {
        queue->candies[queue->rear] = candy;
        queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
    }
}

int dequeue(CandyQueue* queue) {
    if (queue->front != queue->rear) {
        int candy = queue->candies[queue->front];
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
        return candy;
    }
    return -1; // Queue empty
}

// Add Move to History
void addMove(int board[ROWS][COLS], int score) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            newNode->board[i][j] = board[i][j];
    newNode->score = score;
    newNode->next = head;
    head = newNode;
}

void freeMoves() {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Main Game Logic
int main() {
    int board[ROWS][COLS];
    StateStack stack = {.top = -1};
    CandyQueue candyQueue = {.front = 0, .rear = 0};
    int x1, y1, x2, y2;
    int score = 0;

    srand(time(0));

    // Initialize board with random candies
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = rand() % NUM_CANDY_TYPES + 1;
        }
    }

    printf("Initial Board:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }

    // Add random candies to the queue
    for (int i = 0; i < 10; i++) {
        enqueue(&candyQueue, rand() % NUM_CANDY_TYPES + 1);
    }

    // Game loop
    while (score < TARGET_SCORE) {
        printf("Score: %d / %d\n", score, TARGET_SCORE);
        printf("Enter the coordinates of two candies to swap (x1 y1 x2 y2) or -1 to undo:\n");
        scanf("%d", &x1);
        if (x1 == -1) {
            if (stack.top >= 0) {
                GameState previousState = pop(&stack);
                for (int i = 0; i < ROWS; i++)
                    for (int j = 0; j < COLS; j++)
                        board[i][j] = previousState.board[i][j];
                score = previousState.score;
                printf("Undo successful!\n");
                continue;
            } else {
                printf("No moves to undo!\n");
                continue;
            }
        }
        scanf("%d %d %d", &y1, &x2, &y2);
        // Further processing logic here...
    }

    printf("Congratulations! You've reached the target score of %d!\n", TARGET_SCORE);
    freeMoves();
    return 0;
}
