Project 1: Personal Profile Webpage

A simple personal profile website.

Features you can use:

Headings for sections

Images for your photo

Lists for hobbies & skills

Links to your social media

Inline CSS styles

Paragraphs

Div with class/id for layout
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
using System.Threading.Tasks;
int studyTime = 1500;
int breakTime = 300;
int totalTimeInSeconds = 0;
int sessionCount = 0;
int totalStudyMinutes = 0;
int totalBreakMinutes = 0;
int productivityScore = 0;
int dailyGoal = 120; 
int dailyProgress = 0;
int streak = 0;
typedef struct {
    char task[100];
    int completed;
}
Task;
Task tasks[50];
int taskCount = 0;
char* breakSuggestions[] = {
    "Take a short walk",
    "Drink a glass of water",
    "Do some quick stretches",
    "Close your eyes and breathe deeply",
    "Look away from the screen for 20 seconds"
};
char* motivationalQuotes[] = {
    "Stay focused, stay strong!",
    "You’re doing great, keep it up!",
    "Discipline beats motivation every time!",
    "Every minute counts towards success!",
    "You’ve got this!"
};
void saveTasks();
void loadTasks();
void addTask();
void viewTasks();
void completeTask();
void runTimer(int duration, int isStudy);
void showStats();
void exportStatsCSV();
void showBreakSuggestion();
void showMotivationalQuote();
void showProgressBar(int elapsed, int total);
void formatTime(int totalSeconds, char* buffer);
void saveTasks()
{
    FILE* f = fopen("tasks.txt", "w");
    if (!f) return;
    for (int i = 0; i < taskCount; i++)
    {
        fprintf(f, "%s;%d\n", tasks[i].task, tasks[i].completed);
    }
    fclose(f);
}

void loadTasks()
{
    FILE* f = fopen("tasks.txt", "r");
    if (!f) return;
    while (fscanf(f, "%99[^;];%d\n", tasks[taskCount].task, &tasks[taskCount].completed) == 2)
    {
        taskCount++;
        if (taskCount >= 50) break;
    }
    fclose(f);
}
void formatTime(int totalSeconds, char* buffer)
{
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    sprintf(buffer, "%02d:%02d", minutes, seconds);
}

void showProgressBar(int elapsed, int total)
{
    int width = 30;
    int filled = (elapsed * width) / total;
    printf("[");
    for (int i = 0; i < width; i++)
    {
        if (i < filled)
            printf("#");
        else
            printf(" ");
    }
    printf("]");
    fflush(stdout);
}

void showBreakSuggestion()
{
    int index = rand() % 5;
    printf("\n⏸ Break Time! Suggestion: %s\n", breakSuggestions[index]);
}

void showMotivationalQuote()
{
    int index = rand() % 5;
    printf("\n💡 Motivation: %s\n", motivationalQuotes[index]);
}

void runTimer(int duration, int isStudy)
{
    char buffer[10];
    totalTimeInSeconds = duration;
    int elapsed = 0;

    while (totalTimeInSeconds >= 0)
    {
        formatTime(totalTimeInSeconds, buffer);
        printf("\r⏳ Timer: %s ", buffer);
        showProgressBar(elapsed, duration);
        fflush(stdout);

        if (totalTimeInSeconds == 0) break;

        Sleep(1000);
        totalTimeInSeconds--;
        elapsed++;
    }
    printf("\r⏳ Timer: 00:00 ");
    showProgressBar(duration, duration);
    printf("\n⏰ Time's up!\n");

    Beep(1000, 500);

    if (isStudy)
    {
        sessionCount++;
        totalStudyMinutes += duration / 60;
        dailyProgress += duration / 60;
        productivityScore += 10;
        showMotivationalQuote();

        FILE* logFile = fopen("history.txt", "a");
        if (logFile)
        {
            time_t now = time(NULL);
            fprintf(logFile, "Study session completed at %s", ctime(&now));
            fclose(logFile);
        }
    }
    else
    {
        totalBreakMinutes += duration / 60;
        showBreakSuggestion();
    }
}
void addTask()
{
    if (taskCount >= 50)
    {
        printf("⚠️ Task list is full!\n");
        return;
    }
    printf("Enter task: ");
    fgets(tasks[taskCount].task, sizeof(tasks[taskCount].task), stdin);
    tasks[taskCount].task[strcspn(tasks[taskCount].task, "\n")] = 0;
    tasks[taskCount].completed = 0;
    taskCount++;
    saveTasks();
    printf("✅ Task added successfully!\n");
}

void viewTasks()
{
    printf("\n📋 Tasks:\n");
    if (taskCount == 0)
    {
        printf("No tasks yet.\n");
        return;
    }
    for (int i = 0; i < taskCount; i++)
    {
        printf("%d. %s [%s]\n", i + 1, tasks[i].task,
               tasks[i].completed ? "✔ Done" : "❌ Pending");
    }
}

void completeTask()
{
    int num;
    viewTasks();
    if (taskCount == 0) return;
    printf("Enter task number to mark complete: ");
    scanf("%d", &num);
    getchar();
    if (num > 0 && num <= taskCount)
    {
        tasks[num - 1].completed = 1;
        saveTasks();
        printf("✅ Task marked as completed!\n");
    }
    else
    {
        printf("❌ Invalid task number!\n");
    }
}
void showStats()
{
    int completed = 0;
    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].completed)
            completed++;
    }
    float successRate = (taskCount > 0) ? (completed * 100.0f / taskCount) : 0;
    float goalPercent = (dailyGoal > 0) ? (dailyProgress * 100.0f / dailyGoal) : 0;

    printf("\n📊 Stats:\n");
    printf("Sessions: %d | Study: %d min | Breaks: %d min\n",
           sessionCount, totalStudyMinutes, totalBreakMinutes);
    printf("Tasks: %d | Completed: %d | Success Rate: %.1f%%\n",
           taskCount, completed, successRate);
    printf("Productivity Score: %d pts\n", productivityScore);
    printf("Daily Goal: %d min | Progress: %d min (%.1f%%)\n",
           dailyGoal, dailyProgress, goalPercent);
    printf("🔥 Streak: %d days in a row!\n", streak);
}

void exportStatsCSV()
{
    FILE* f = fopen("stats.csv", "w");
    if (!f)
    {
        printf("❌ Could not export stats.\n");
        return;
    }
    fprintf(f, "Sessions,TotalStudyMinutes,TotalBreakMinutes,ProductivityScore,Streak,DailyGoal,DailyProgress\n");
    fprintf(f, "%d,%d,%d,%d,%d,%d,%d\n",
            sessionCount, totalStudyMinutes, totalBreakMinutes,
            productivityScore, streak, dailyGoal, dailyProgress);
    fclose(f);
    printf("📁 Stats exported to stats.csv successfully!\n");
}
int main()
{
    srand((unsigned int)time(NULL));
    int choice;
    loadTasks();

    printf("=== SMART STUDY TIMER (FINAL VERSION) ===\n");

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Set Study/Break Time\n");
        printf("2. Start Study Session\n");
        printf("3. Add Task\n");
        printf("4. View Tasks\n");
        printf("5. Mark Task Complete\n");
        printf("6. View Stats\n");
        printf("7. Export Stats to CSV\n");
        printf("8. Set Daily Goal\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
        {
            printf("Enter study time (minutes): ");
            scanf("%d", &studyTime);
            printf("Enter break time (minutes): ");
            scanf("%d", &breakTime);
            getchar();
            studyTime *= 60;
            breakTime *= 60;
            printf("✅ Updated times successfully!\n");
        }
        else if (choice == 2)
        {
            runTimer(studyTime, 1);
            if (sessionCount % 4 == 0)
            {
                printf("\n🎉 Long Break (15 min)\n");
                runTimer(15 * 60, 0);
            }
            else
            {
                runTimer(breakTime, 0);
            }
        }
        else if (choice == 3)
        {
            addTask();
        }
        else if (choice == 4)
        {
            viewTasks();
        }
        else if (choice == 5)
        {
            completeTask();
        }
        else if (choice == 6)
        {
            showStats();
        }
        else if (choice == 7)
        {
            exportStatsCSV();
        }
        else if (choice == 8)
        {
            printf("Enter new daily goal (minutes): ");
            scanf("%d", &dailyGoal);
            getchar();
            printf("✅ Daily goal set to %d minutes!\n", dailyGoal);
        }
        else if (choice == 9)
        {
            printf("Exiting... Stay productive! 🚀\n");
            break;
        }
        else
        {
            printf("❌ Invalid choice!\n");
        }
    }

    return 0;
}
