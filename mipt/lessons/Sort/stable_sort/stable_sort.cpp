#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;

template < class T >
void merge_sort(T l, T r) {
  if (r - l < 1) return;
  T m = l + (r - l) / 2;
  merge_sort(l, m);
  merge_sort(m + 1, r);
  T pnt1 = l;
  T pnt2 = (m+1);
  int cnt = 0;
  vector <typeof(*l)> tmp;
  while (pnt1 != (m+1) && pnt2 != (r+1)) {
    if (*pnt1 < *pnt2)
      tmp.push_back(*(pnt1++));
    else
      tmp.push_back(*(pnt2++));
  }
  for(; pnt1 != (m+1); ++pnt1) tmp.push_back(*pnt1);
  for(; pnt2 != (r+1); ++pnt2) tmp.push_back(*pnt2);
  copy(tmp.begin(), tmp.end(), l);
}

void test_ms_numbers() {
  int n = 0;
  scanf("%d", &n);
  int arr[n];
  for(int i = 0; i < n; i++) {
    scanf("%d", &arr[i]);
  }
  merge_sort(arr, arr + n - 1);
  for(int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void test_ms_strings() {
  int n = 0;
  scanf("%d", &n);
  string arr[n];
  for(int i = 0; i < n; i++) {
    cin >> arr[i];
  }
  merge_sort(arr, arr + n - 1);
  for(int i = 0; i < n; i++) {
    cout << arr[i] << endl;
  }
  printf("\n");
}

int main() {
  freopen("input.txt", "r", stdin);  freopen("output.txt", "w", stdout);
  test_ms_numbers();
  //test_ms_strings();
  printf("Execution time: %lf\n", clock() / 1000.0);
  return 0;
}
