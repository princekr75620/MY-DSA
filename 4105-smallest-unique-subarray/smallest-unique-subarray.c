
#include <stdlib.h>

static int* rankArr;
static int n, gap;

int compareSuffix(const void* a, const void* b) {
    int i = *(int*)a;
    int j = *(int*)b;

    if (rankArr[i] != rankArr[j])
        return rankArr[i] < rankArr[j] ? -1 : 1;

    int x = (i + gap < n) ? rankArr[i + gap] : -1;
    int y = (j + gap < n) ? rankArr[j + gap] : -1;

    if (x != y)
        return x < y ? -1 : 1;

    return 0;
}

int smallestUniqueSubarray(int* nums, int numsSize) {
    n = numsSize;

    int* sa = malloc(n * sizeof(int));
    int* r = malloc(n * sizeof(int));
    int* nr = malloc(n * sizeof(int));
    int* pos = malloc(n * sizeof(int));
    int* lcp = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        sa[i] = i;
        r[i] = nums[i];
    }

    rankArr = r;

    for (gap = 1; gap < n; gap *= 2) {
        qsort(sa, n, sizeof(int), compareSuffix);

        nr[sa[0]] = 0;

        for (int i = 1; i < n; i++) {
            int a = sa[i - 1];
            int b = sa[i];

            int a1 = r[a];
            int b1 = r[b];

            int a2 = (a + gap < n) ? r[a + gap] : -1;
            int b2 = (b + gap < n) ? r[b + gap] : -1;

            nr[b] = nr[a] + (a1 != b1 || a2 != b2);
        }

        int* temp = r;
        r = nr;
        nr = temp;
        rankArr = r;

        if (r[sa[n - 1]] == n - 1)
            break;
    }

    for (int i = 0; i < n; i++)
        pos[sa[i]] = i;

    int h = 0;

    for (int i = 0; i < n; i++) {
        int k = pos[i];

        if (k == 0)
            continue;

        int j = sa[k - 1];

        while (i + h < n && j + h < n && nums[i + h] == nums[j + h]) {
            h++;
        }

        lcp[k] = h;

        if (h > 0)
            h--;
    }

    int ans = n;

    for (int k = 0; k < n; k++) {
        int i = sa[k];

        int best = lcp[k];

        if (k + 1 < n && lcp[k + 1] > best)
            best = lcp[k + 1];

        int len = best + 1;

        if (len <= n - i && len < ans)
            ans = len;
    }

    free(sa);
    free(r);
    free(nr);
    free(pos);
    free(lcp);

    return ans;
}
