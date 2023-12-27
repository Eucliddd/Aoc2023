#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <vector>

using namespace std;

const int maxn = 256;

char buf1[maxn], buf2[maxn];
char mp[maxn];

int main() {
  int sum = 0;
  FILE* file = fopen("input.txt", "r");
  while (fscanf(file, "%s %s", buf1, buf2) == 2) {
    vector<int> parts;
    for (int i = 0, len = strlen(buf2); i < len; i++) {
      int j = i;
      while (i < len && '0' <= buf2[i] && buf2[i] <= '9') i++;
      buf2[i] = 0;
      parts.push_back(atoi(buf2 + j));
    }

    int n = strlen(buf1), cnt = 0, ans = 0;
    mp[n] = 0;
    for (int i = 0; i < n; i++) if (buf1[i] == '?') cnt++;
    for (int s = 0; s < (1 << cnt); s++) {
      bool flag = true;
      int ptr = 0, cnt = 0;
      for (int i = 0, c = 0; i < n; i++) {
        if (buf1[i] == '?') {
          mp[i] = (s >> c & 1) ? '#' : '.';
          c++;
        } else {
          mp[i] = buf1[i];
        }
        // maintain
        if (mp[i] == '#') {
          cnt++;
        }
        if (mp[i] == '.' || i + 1 == n) {
          if (cnt) {
            // printf("%d ", cnt);
            if (ptr < parts.size() && cnt == parts[ptr]) {
              ptr++;
            } else {
              flag = false;
              // break;
            }
            cnt = 0;
          }
        }
      }
      if (flag && ptr == parts.size()) ans++;
      // puts("");
      // puts(mp);
    }
    printf("%d\n", ans);
    sum += ans;
  }
  printf("%d\n", sum);
  return 0;
}
