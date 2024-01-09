#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

struct Process {
  int attributes[6];
};

void insert(struct Process *processes, int currentTime, int count) {
  for (int i = 0; i < count; i++) {
    int *attributes = processes[i].attributes;
    if (attributes[5] == 0 && attributes[1] == currentTime) {
      attributes[5] = 1;
    }
  }
}

void executeProcess(struct Process *executionProcesses, int processIndex, int count) {
  for (int i = 0; i < count; i++) {
    int *attributes = executionProcesses[i].attributes;
    if (i == processIndex && attributes[4] == 0) {
      attributes[0] = attributes[0] - 1;
      if (attributes[0] == 0) {
        attributes[4] = 1;
      }
    } else if (attributes[5] == 1 && attributes[4] == 0) {
      attributes[3] = attributes[3] + 1;
    }
  }
}

int selectProcessToExecute(struct Process *processes, int type, int count) {
  int selectedProcessIndex = -1;
  int lowestProcessValue = 0;
  int isProcessSelected = 0;

  for (int k = 0; k < count; k++) {
    int *attributes = processes[k].attributes;

    if ((isProcessSelected == 0 && attributes[5] == 1 && attributes[4] == 0) ||
        (isProcessSelected == 1 && attributes[5] == 1 && attributes[4] == 0 &&
         attributes[type] < lowestProcessValue)) {
      lowestProcessValue = attributes[type];
      selectedProcessIndex = k;
      isProcessSelected = 1;
    }
  }

  return selectedProcessIndex;
}

void preemptiveScheduling(struct Process *pProcesses, int type, int count) {
  int isRunning = 1;
  int currentTime = 0;
  printf("Preemptive Scheduling starts\n");
  while (isRunning == 1) {
    insert(pProcesses, currentTime, count);
    int processToExecute = selectProcessToExecute(pProcesses, type, count);
    if (processToExecute == -1) {
      isRunning = 0;
    } else {
      executeProcess(pProcesses, processToExecute, count);
      currentTime++;
    }
  }
  printf("Scheduling Finish\n");
  printf("Total Time: %d time\n", currentTime);
}

void nonPreemptiveScheduling(struct Process *pProcesses, int type, int count) {
  int isRunning = 1;
  int currentTime = 0;
  printf("Non-Preemptive Scheduling starts\n");
  insert(pProcesses, currentTime, count);
  while (isRunning == 1) {
    int processToExecute = selectProcessToExecute(pProcesses, type, count);
    if (processToExecute == -1) {
      isRunning = 0;
    } else {
      int *attributes = pProcesses[processToExecute].attributes;
      while (attributes[4] == 0) {
        executeProcess(pProcesses, processToExecute, count);
        currentTime++;
        insert(pProcesses, currentTime, count);
      }
    }
  }
  printf("Scheduling Finish\n");
  printf("Total Time: %d time\n", currentTime);
}

void roundRobinScheduling(struct Process *pProcesses, int quantum, int count) {
  int isRunning = 1;
  int currentTime = 0;
  printf("Round Robin Scheduling start\n");
  insert(pProcesses, currentTime, count);
  int processListSize = count;
  int currentProcessIndex = 0;
  while (isRunning == 1) {
    for (int k = 0; k < quantum; k++) {
      int processToExecute = selectProcessToExecute(pProcesses, 1, count);
      int *attributes = pProcesses[currentProcessIndex].attributes;
      if (processToExecute == -1) {
        isRunning = 0;
        break;
      } else if (attributes[2] == 1) {
        break;
      } else {
        executeProcess(pProcesses, currentProcessIndex, count);
        insert(pProcesses, currentTime, count);
        currentTime++;
      }
    }
    currentProcessIndex++;
    if (currentProcessIndex == processListSize) {
      currentProcessIndex = 0;
    }
  }
}

float calculateAverageWaitingTime(struct Process *pProcesses, int count) {
  int sum = 0;
  for (int i = 0; i < count; i++) {
    int *attributes = pProcesses[i].attributes;
    sum = sum + attributes[3];
  }
  return (float)sum / count;
}

void displayWaitingTime(struct Process *pProcesses, int count) {
  for (int i = 0; i < count; i++) {
    int *attributes = pProcesses[i].attributes;
    printf("P%d: %dms\n", i + 1, attributes[3]);
  }
}

void writeToFile(char *output) {
  FILE *file = fopen(output, "w");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  printf("Enter your message: ");
  fprintf(file, "Average: ms\n");

  fclose(file);
}

char *inputFileName = NULL;
char *outputFileName = NULL;

int main(int argc, char *argv[]) {
  int option;
  if (argc < 5) {
    printf("You should introduce 2 arguments\n");
    exit(EXIT_FAILURE);
  }
  while ((option = getopt(argc, argv, "f:o:")) != -1) {
    switch (option) {
    case 'f':
      inputFileName = optarg;
      break;
    case 'o':
      outputFileName = optarg;
      break;
    default:
      break;
    }
  }
  if (inputFileName == NULL || outputFileName == NULL) {
    printf("Use the parameter -f for input and -o for output\n");
    exit(EXIT_FAILURE);
  }
  int processCount = 0;
  struct Process tempArray[100]; // Assuming a maximum of 100 processes
  FILE *file = fopen(inputFileName, "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  char line[100];
  while (fgets(line, sizeof(line), file) != NULL) {
    int attributes[6];
    sscanf(line, "%d:%d:%d:%d:%d:%d", &attributes[0], &attributes[1], &attributes[2],
           &attributes[3], &attributes[4], &attributes[5]);
    struct Process process = {{attributes[0], attributes[1], attributes[2],
                               attributes[3], attributes[4], attributes[5]}};
    tempArray[processCount] = process;
    processCount++;
  }
  fclose(file);

  struct Process processes[processCount];
  for (int j = 0; j < processCount; j++) {
    processes[j] = tempArray[j];
  }

  int userChoice;
  int isFinished = 0;
  while (isFinished == 0) {
    printf("Choose Action: \n");
    printf("1. Preemptive Scheduling\n");
    printf("2. Non-Preemptive Scheduling\n");
    printf("3. Show Result\n");
    printf("4. End Program\n");
    scanf("%d", &userChoice);
    switch (userChoice) {
    case 1: {
      int preemptiveSchedulingType;
      printf("Choose Between the five Preemptive Scheduling Types \n");
      printf("1. None\n");
      printf("2. First Come, First Served Scheduling\n");
      printf("3. Shortest-Job-First Scheduling\n");
      printf("4. Priority Scheduling\n");
      printf("5. Round-Robin Scheduling\n");
      scanf("%d", &preemptiveSchedulingType);
      switch (preemptiveSchedulingType) {
      case 1:
        printf("None\n");
        break;
      case 2:
        printf("First Come, First Served Scheduling\n");
        preemptiveScheduling(processes, 1, processCount);
        break;
      case 3:
        printf("Shortest-Job-First Scheduling\n");
        preemptiveScheduling(processes, 0, processCount);
        break;
      case 4:
        printf("Priority Scheduling\n");
        preemptiveScheduling(processes, 2, processCount);
        break;
      default:
        printf("You have to choose between 1 - 5\n");
        break;
      }
    } break;
    case 2: {
      int nonPreemptiveSchedulingType;
      printf("Choose Between the five Non-Preemptive Scheduling Types \n");
      printf("1. None\n");
      printf("2. First Come, First Served Scheduling\n");
      printf("3. Shortest-Job-First Scheduling\n");
      printf("4. Priority Scheduling\n");
      scanf("%d", &nonPreemptiveSchedulingType);
      switch (nonPreemptiveSchedulingType) {
      case 1:
        printf("None Scheduling method chosen try again \n");
        break;
      case 2:
        printf("First Come, First Served Scheduling\n");
        nonPreemptiveScheduling(processes, 1, processCount);
        break;
      case 3:
        printf("Shortest-Job-First Scheduling\n");
        nonPreemptiveScheduling(processes, 0, processCount);
        break;
      case 4:
        printf("Priority Scheduling\n");
        nonPreemptiveScheduling(processes, 2, processCount);
        break;
      case 5:
        printf("Round-Robin Scheduling\n");
        printf("Insert Quantum\n");
        int quantum;
        scanf("%d", &quantum);
        roundRobinScheduling(processes, quantum, processCount);
        break;
      default:
        printf("You have to choose between 1 - 4\n");
        break;
      }
    } break;
    case 3: {
      printf("The Result is:\n");
      printf("Processes Waiting Time \n");
      float average;
      average = calculateAverageWaitingTime(processes, processCount);
      displayWaitingTime(processes, processCount);
      printf("Average: %fms\n", average);
    } break;
    case 4: {
      printf("Exit Program, Thank you  \n");
      float averages;
      averages = calculateAverageWaitingTime(processes, processCount);
      FILE *file = fopen(outputFileName, "w");
      if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
      }
      fprintf(file, "Processes Waiting Time \n");
      for (int i = 0; i < processCount; i++) {
        int *attributes = processes[i].attributes;
        fprintf(file, "P%d: %dms\n", i + 1, attributes[3]);
      }
      fprintf(file, "Average: %fms\n", averages);
      fclose(file);
      isFinished = 1;
    } break;
    default:
      printf("You have to choose between 1 - 4\n");
      break;
    }
  }
  return 0;
}
