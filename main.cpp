#include <iostream>
#include <ctime>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <locale>

using namespace std;

enum RequestType {firstType = 1, secType = 2}; // Тип заявки.

struct Request
{
    RequestType ReqType;
};

const double BegInterval = 0; // Начало каждого из интервалов генерации и обработки.
const double EndGenerateFReq = 5; // Максимальное время генерации заявки 1 типа.
const double UseFReq = 4; // Максимальное время обработки заявки 1 типа.
const double UseSReq = 4; // Максимальное время обработки заявки 2 типа.
const int    MaxSReqPos = 4; // Не далее этого элемента заявка второго типа вернётся в очередь.

// Генерирует случайное число.
double RandTime()
{
    double Val = (rand() * 1.0) / RAND_MAX;
    return Val;
}

// Какое время меньше.
double MinTime(double T1, double T2)
{
    return ((T1 > T2) ? T2 : T1);
}

// Генерирует время прихода следующей заявки 1 типа.
double GenAddFReq()
{
    double Val = RandTime();
    double Time = (EndGenerateFReq - BegInterval) * Val + BegInterval;
    return Time;
}

// Генерирует время обработки следующей заявки 1 типа.
double GenUseFReq()
{
    double Val = RandTime();
    double Time = (UseFReq - BegInterval) * Val + BegInterval;
    return Time;
}

// Генерирует время обработки заявки 2 типа.
double GenUseSReq()
{
    double Val = RandTime();
    double Time = (UseSReq - BegInterval) * Val + BegInterval;
    return Time;
}

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));
    list<RequestType> RequestQueue;
    RequestType STypeReq = secType;
    RequestType FTypeReq = firstType;
    RequestQueue.push_back(STypeReq);
    int CountFReqs = 0; // Количество обработанных заявок 1 типа.
    int CountUseFReqs = 0;
    int Enter2QueueFReqs = 0; // Количество заявок 1 типа вошедших в систему.
    double Time = 0.0;
    double TimeEnter = 0.0; // Время прихода заявки.
    double TimeClearService = 0.0; // Время обработки заявки
    double TimeClear = 0.0; //Общее время обработки заявок
    double NextEventTime = 0.0; // Время следующего события.
    double TotalTime = 0.0; // Общее время моделирования.
    list<RequestType>::iterator PredSReq = RequestQueue.begin(); // Указывает на заявку, перед которой надо вставить заявку 2го типа.
    list<RequestType>::iterator tmp = RequestQueue.begin();
    int CountOfCalcs =   0; // Количество вычислений.
    double MiddleLenQueue      = 0.0; // Средняя длина после каждого вычисления.
    int countSecReqs =   0; // Количество заявок 2 типа.
    int SumOfCalcs   =   0;
    while(true)
    {
        if( CountUseFReqs == 1000)
            break;
        TimeEnter = GenAddFReq();
        if(RequestQueue.front() == 2)
            TimeClearService = GenUseSReq();
        else
            TimeClearService = GenUseFReq();
        Time = MinTime(TimeEnter, TimeClearService);

        if(Time == TimeEnter)
        {
            // Добавляем заявку первого типа.
            RequestQueue.push_back(FTypeReq);
            CountFReqs++;       // Увеличиваем счётчик заявок 1 типа
            Enter2QueueFReqs++; // Увеличиваем общее количество заявок вошедших в систему.
            TotalTime += Time;
            continue;
        }
        else
        {
            TimeClear += Time;
            // Обрабатываем заявку.
            if(secType == RequestQueue.front())
            {
                // Обрабатываем заявку 2-го типа.
                countSecReqs++;
                RequestQueue.pop_front();
                if(MaxSReqPos >= RequestQueue.size())
                {
                    RequestQueue.push_back(STypeReq);
                    continue;
                }
                else
                {
                    PredSReq = RequestQueue.begin();
                    for(int i = 0; i != 4; i++) PredSReq++;

                    RequestQueue.insert(PredSReq, STypeReq);
                    continue;
                }
            }
            else
            {
                // Обрабатываем заявку 1 типа.
                TotalTime += Time;
                CountUseFReqs++;
                RequestQueue.pop_front();
                SumOfCalcs += static_cast<int>(RequestQueue.size());
                CountOfCalcs++;

                if((0 == CountUseFReqs % 100) && (0 != CountUseFReqs))
                {
                    MiddleLenQueue = (SumOfCalcs * 1.0) / CountOfCalcs;
                    cout << endl;
                    cout << "Count of requests: " << CountUseFReqs << endl;
                    cout << "Now length of queue: " << static_cast<unsigned int>(RequestQueue.size()) << endl;
                    cout << "Now middle length of queue: " << MiddleLenQueue << endl;
                    cout << "countSecReqs: " << countSecReqs << endl;
                }
            }
        }
    }
    cout <<  endl << "Total modeling time: " << TotalTime << endl;
    cout << endl << "Downtime: " << TotalTime-TimeClear << endl;
    cout << "1 type requests enter: " << Enter2QueueFReqs << endl;
    cout << "1 type requests out: " << CountUseFReqs << endl;
    cout << "countSecReqs: " << countSecReqs << endl;
    return 0;
}

