#include <omp.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>

using namespace std;

void func()
{
  for (int i = 0; i < 500000; i++)
    rand();
}
int main()
{
  setlocale(LC_ALL, "rus");
  omp_set_num_threads(2);
  cout << "FOR:\n\n";
#pragma omp parallel for
  for (int i = 0; i < 100; i++)
  {
    cout << "thread(" << omp_get_thread_num() << ") " << i << " | num of threads: " << omp_get_num_threads() << "\n"; func();
  }
  cout << "\nSECTIONS:\n\n";
#pragma omp parallel
  {
#pragma omp sections nowait
    {
#pragma omp section
    for (int i = 0; i < 10; i++)
    {
     cout << "thread(" << omp_get_thread_num() << ") " << 1 << " | num of threads: " << omp_get_num_threads() << "\n"; func();
    }
#pragma omp section
    for (int i = 0; i < 20; i++)
    {
    cout << "thread(" << omp_get_thread_num() << ") " << 2 << " | num of threads: " << omp_get_num_threads() << "\n"; func();
    }
    }
#pragma omp barrier
  cout << "\nBARRIER:\n\n";
  for (int i = 0; i < 10; i++)
  {
    cout << "thread(" << omp_get_thread_num() << ") " << 3 << " | num of threads: " << omp_get_num_threads() << "\n";
    func();
  }
  }
  cout << endl;
  int i;
  int max;
  int a[10] = { 5, 6, 7, 8, 9, 1, 2, 3, 4, 0 };
  max = a[0];
#pragma omp parallel
  {
#pragma omp for
    for (i = 1; i < 10; i++) {
      if (a[i] > max) {
#pragma omp critical
        {
          if (a[i] > max) {
            max = a[i];
          }
        }
      }
    }
#pragma omp single
    cout << max << "\n";
  }
  omp_set_num_threads(2);
  omp_lock_t lock;
  int n;
  omp_init_lock(&lock);
#pragma omp parallel
  {
#pragma omp sections nowait
    {
#pragma omp section
      {
        omp_set_lock(&lock);
        cout << "thread(" << omp_get_thread_num() << ") " << "Working\n"; func();
        Sleep(5);
        omp_unset_lock(&lock);
      }
      
#pragma omp section
      {
        while (!omp_test_lock(&lock)) {
          cout << "thread(" << omp_get_thread_num() << ") " << "Waiting...\n"; func();
        }
        cout << "thread(" << omp_get_thread_num() << ") " << "Working\n"; func();
      }
      
    }
  }
}

