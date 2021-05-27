#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <cassert>
using namespace std;

const int W = 30;
const int H = 30;

#ifdef ONLINE_JUDGE

class Tester
{
public:
    void init()
    {
    }

    void getInput(int *sy, int *sx, int *ty, int *tx)
    {
        cin>>*sy>>*sx>>*ty>>*tx;
    }

    int answer(string path)
    {
        cout<<path<<endl;
        int result;
        cin>>result;
        return result;
    }

    void printScore()
    {
    }
};

#else

class Tester
{
    vector<vector<int>> h, v;
    vector<int> sy, sx, ty, tx, a;
    vector<double> e;
    int k;
    double score;

public:
    void init()
    {
        h = vector<vector<int>>(H, vector<int>(W-1));
        for (int y=0; y<H; y++)
            for (int x=0; x<W-1; x++)
                cin>>h[y][x];
        v = vector<vector<int>>(H-1, vector<int>(W));
        for (int y=0; y<H-1; y++)
            for (int x=0; x<W; x++)
                cin>>v[y][x];

        sy = vector<int>(1000);
        sx = vector<int>(1000);
        ty = vector<int>(1000);
        tx = vector<int>(1000);
        a = vector<int>(1000);
        e = vector<double>(1000);
        for (int k=0; k<1000; k++)
            cin>>sy[k]>>sx[k]>>ty[k]>>tx[k]>>a[k]>>e[k];

        k = 0;
        score = 0;
    }

    void getInput(int *sy, int *sx, int *ty, int *tx)
    {
        *sy = this->sy[k];
        *sx = this->sx[k];
        *ty = this->ty[k];
        *tx = this->tx[k];
    }

    int answer(string path)
    {
        cout<<path<<endl;

        int y = sy[k];
        int x = sx[k];
        int b = 0;
        for (char c: path)
        {
            switch (c)
            {
            case 'U':
                b += v[y-1][x];
                y--;
                break;
            case 'D':
                b += v[y][x];
                y++;
                break;
            case 'L':
                b += h[y][x-1];
                x--;
                break;
            case 'R':
                b += h[y][x];
                x++;
                break;
            }
            assert(0<=x && x<W && 0<=y && y<H);
        }
        score = score*0.998+a[k]/b;
        int result = int(b*e[k]);
        k++;
        return result;
    }

    void printScore()
    {
        cout<<long long(2312311*score)<<endl;
    }
};

#endif

int main()
{
    Tester tester;
    tester.init();

    for (int k=0; k<1000; k++)
    {
        int sy, sx, ty, tx;
        tester.getInput(&sy, &sx, &ty, &tx);

        int oo = 99999999;
        vector<vector<int>> D(H, vector<int>(W, oo));
        vector<vector<char>> M(H, vector<char>(W, '.'));
        priority_queue<pair<int, pair<int, int>>> Q;
        vector<vector<bool>> F(H, vector<bool>(W));

        D[sy][sx] = 0;
        M[sy][sx] = '!';
        Q.push(make_pair(0, make_pair(sy, sx)));

        while (!Q.empty())
        {
            int y = Q.top().second.first;
            int x = Q.top().second.second;
            Q.pop();
            if (F[y][x])
                continue;
            F[y][x] = true;

            if (0<=y-1 && !F[y-1][x] && D[y][x]+1<D[y-1][x])
            {
                M[y-1][x] = 'U';
                D[y-1][x] = D[y][x]+1;
                Q.push({-D[y-1][x], {y-1, x}});
            }
            if (y+1<H && !F[y+1][x] && D[y][x]+1<D[y+1][x])
            {
                M[y+1][x] = 'D';
                D[y+1][x] = D[y][x]+1;
                Q.push({-D[y+1][x], {y+1, x}});
            }
            if (0<=x-1 && !F[y][x-1] && D[y][x]+1<D[y][x-1])
            {
                M[y][x-1] = 'L';
                D[y][x-1] = D[y][x]+1;
                Q.push({-D[y][x-1], {y, x-1}});
            }
            if (x+1<W && !F[y][x+1] && D[y][x]+1<D[y][x+1])
            {
                M[y][x+1] = 'R';
                D[y][x+1] = D[y][x]+1;
                Q.push({-D[y][x+1], {y, x+1}});
            }
        }

        int y = ty;
        int x = tx;
        string ans;
        while (M[y][x]!='!')
        {
            ans += M[y][x];
            switch (M[y][x])
            {
            case 'U': y++; break;
            case 'D': y--; break;
            case 'L': x++; break;
            case 'R': x--; break;
            }
        }
        reverse(ans.begin(), ans.end());
        tester.answer(ans);
    }
    tester.printScore();
}
