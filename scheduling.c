#include <stdio.h>

#define MAX 10  // Maximum size of the ready queue

// Process structure
typedef struct {
    int pid;          // Process ID
    int burst;        // CPU burst time
    int remaining;    // Remaining time (for RR)
    int waiting;      // Waiting time
    int turnaround;   // Turnaround time
} Process;

/* ---------- SJF (Non-preemptive) ---------- */
void sjf(Process p[], int n) {
    int total_wait = 0, total_turn = 0;
    Process temp;

    // Sort processes by burst time (ascending)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].burst > p[j].burst) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    // Calculate waiting time
    p[0].waiting = 0;
    for (int i = 1; i < n; i++) {
        p[i].waiting = p[i - 1].waiting + p[i - 1].burst;
    }

    // Calculate turnaround time
    for (int i = 0; i < n; i++) {
        p[i].turnaround = p[i].waiting + p[i].burst;
        total_wait += p[i].waiting;
        total_turn += p[i].turnaround;
    }

    // Print results
    printf("\n--- SJF Scheduling (Non-preemptive) ---\n");
    printf("PID\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t\t%d\n",
               p[i].pid, p[i].burst, p[i].waiting, p[i].turnaround);
    }
    printf("Average Waiting Time: %.2f\n", (float)total_wait / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turn / n);
}

/* ---------- Round Robin (Preemptive with limited buffer) ---------- */
void roundRobin(Process p[], int n, int quantum) {
    int time = 0;
    int remaining_processes = n;
    int context_switches = 0;

    // Initialize remaining times
    for (int i = 0; i < n; i++) {
        p[i].remaining = p[i].burst;
        p[i].waiting = 0;
    }

    int queue[MAX];  // Circular queue
    int front = 0, rear = 0;

    // Enqueue all processes initially
    for (int i = 0; i < n; i++) {
        queue[rear] = i;
        rear = (rear + 1) % MAX;
    }

    // Simulate RR scheduling
    while (remaining_processes > 0) {
        int idx = queue[front];
        front = (front + 1) % MAX;  // Dequeue

        if (p[idx].remaining > 0) {
            context_switches++;

            if (p[idx].remaining > quantum) {
                time += quantum;
                p[idx].remaining -= quantum;

                // Re-enqueue only if space available
                int next_rear = (rear + 1) % MAX;
                if (next_rear != front) {  // space available
                    queue[rear] = idx;
                    rear = next_rear;
                } else {
                    printf("Queue full! Process %d cannot be re-enqueued immediately.\n", p[idx].pid);
                }

            } else {
                time += p[idx].remaining;
                p[idx].waiting = time - p[idx].burst;
                p[idx].remaining = 0;
                remaining_processes--;
            }
        }
    }

    // Calculate turnaround time
    int total_wait = 0, total_turn = 0;
    for (int i = 0; i < n; i++) {
        p[i].turnaround = p[i].waiting + p[i].burst;
        total_wait += p[i].waiting;
        total_turn += p[i].turnaround;
    }

    // Print results
    printf("\n--- Round Robin Scheduling (Preemptive) ---\n");
    printf("PID\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t\t%d\n",
               p[i].pid, p[i].burst, p[i].waiting, p[i].turnaround);
    }
    printf("Average Waiting Time: %.2f\n", (float)total_wait / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turn / n);
    printf("Context Switches (CPU Utilisation Indicator): %d\n", context_switches);
}

/* ---------- MAIN ---------- */
int main() {
    Process p1[] = {{1, 6}, {2, 8}, {3, 7}, {4, 3}};  // For SJF
    Process p2[] = {{1, 6}, {2, 8}, {3, 7}, {4, 3}};  // For RR
    int n = 4;
    int quantum = 3;

    sjf(p1, n);                 // Run SJF
    roundRobin(p2, n, quantum); // Run Round Robin

    return 0;
}

