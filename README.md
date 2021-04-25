# Planning for SO second midterm

### Data Structures
#### Process: storage of a process properties | G
    - pid
    - priority
    - arrival time
    - burst time
    - remaining time
    - response time
    - completion time

#### schedule: storage of processes, is a priority queue | S
    - processes
    - arrival time line
    - ready queue
    - compare function
    - elapsed time

### Functions

#### turnAroundT: | G
Input:
    - process
Output:
    - Time from moment of arrival to ready queue to completion moment of a
    completed process.
Description:
turn around time = completion time - arrival time

#### waitT: | S
Input:
    - process
Output:
    - Time from arrival to completion that wasn't execution time, of a
    completed process
Description:
wait time = turn around time - burst time

#### responseT: | G
Input:
    - process
Output:
    - Time from arrival to first time where CPU was given to the process.
Description:
responde time

#### avgTurnAT: | S
Input:
    - schedule
Output:
    - average of turn around times of a sequence of processes completed.
Description:
avg(turnAroundT(processI))

#### avgWaitT: | G
Input:
    - schedule
Output:
    - average of wait times of a sequence of processes completed.
Description:
avg(waitT(processI))

#### avgResponseT: | S
Input:
    - schedule
Output:
    - average of response times of a sequence of processes completed.
Description:
avg(responseT(processI))

### priorityFCFS: | G
Input:
    - process A
    - process B
Output:
    - Value that represents the order of A and B from the FCFS perspective.
Description:
Priority is inversely proportional to arrival time.
- -1    IF A priority is less than B priority.
- 0     IF A priority is equal to B priority.
- 1     IF A priority is greater than B priority.

### prioritySJF: | S
Input:
    - process A
    - process B
Output:
    - Value that represents the order of A and B from the SJF perspective.
Description:
Priority is inversely proportional to remaining time.
- -1    IF A priority is less than B priority.
- 0     IF A priority is equal to B priority.
- 1     IF A priority is greater than B priority.

### priorityP: | G
Input:
    - process A
    - process B
Output:
    - Value that represents the order of A and B from the Priority perspective.
Description:
Priority is directly proportional to priority.
- -1    IF A priority is less than B priority.
- 0     IF A priority is equal to B priority.
- 1     IF A priority is greater than B priority.

### Processes

#### caller: | G
Input:
    - schedule
Description:
By the elapsed time in the schedule, enqueue the process from the schedule that
have already arrived into the ready queue.

#### executeSchedule: | G S
Input:
    - isPreemptive
    - schedule
    - priorityX (a compare function for 2 processes)
Description:
We have an actual process that refers some process.
- Set schedule elapsed time to 0
- While ready queue is not empty do the following:
    - Call the process for actual moment.
    - If there is no actual process or (isPreemptive and the front of the queue is different to the actual process):
        Asign front of the queue to actual process. 
    - If there is an actual process:
        - If there still has remaining time:
            - Reduce the remaining time by 1
            - If it doesn't have response time yet, asign it to actual time.
        - Else:
            - Asign actual time to completion time.
            - Pop the queue
            - Asign null to actual processs.
    - Increase elapsed time
